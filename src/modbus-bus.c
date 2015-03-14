/*
 * Copyright © 2013-2015 Michael Heimpold <mhei@heimpold.de>
 *
 * SPDX-License-Identifier: LGPL-2.1+
 */

#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "modbus-bus.h"

static inline void _modbus_bus_setbit(struct modbus_bus *bus, int address)
{
    int offset = address / (sizeof(int) * 8);
    int index = address % (sizeof(int) * 8);

    bus->bitmap[offset] |= 1 << index;
}

static inline void _modbus_bus_clearbit(struct modbus_bus *bus, int address)
{
     int offset = address / (sizeof(int) * 8);
     int index = address % (sizeof(int) * 8);

     bus->bitmap[offset] &= ~(1 << index);
}

static inline int _modbus_bus_testbit(struct modbus_bus *bus, int address)
{
    int offset = address / (sizeof(int) * 8);
    int index = address % (sizeof(int) * 8);

    return !!(bus->bitmap[offset] & (1 << index));
}

int modbus_bus_init(struct modbus_bus *bus)
{
    int address;

    if (bus == NULL) {
        errno = EINVAL;
        return -1;
    }

    /* mark all addresses as free */
    memset(bus->bitmap, 0xff, sizeof(bus->bitmap));

    /* address 0 is broadcast address */
    _modbus_bus_clearbit(bus, 0);

    /* addresses 248 - 255 are reserved */
    for (address = 248; address <= 255; address++)
        _modbus_bus_clearbit(bus, address);

    return 0;
}

int modbus_bus_request_address(struct modbus_bus *bus)
{
    unsigned int i;
    int address = -1;

    if (bus == NULL) {
        errno = EINVAL;
        return -1;
    }

    for (i = 0; i < MODBUS_BUS_BITMAPSIZE; i++) {
        /* find first set bit (indicates a free address) */
        int index = ffs(bus->bitmap[i]);

        if (index) {
            address = i * sizeof(int) * 8 + index - 1;
            break;
        }
    }

    if (address > 0)
        _modbus_bus_clearbit(bus, address);

    return address;
}

int modbus_bus_acquire_address(struct modbus_bus *bus, int address)
{
    if (bus == NULL || address < 0 || address > 247) {
        errno = EINVAL;
        return -1;
    }

    if (! _modbus_bus_testbit(bus, address))
        return 1;

    _modbus_bus_clearbit(bus, address);
    return 0;
}

int modbus_bus_release_address(struct modbus_bus *bus, int address)
{
    if (bus == NULL || address < 0 || address > 247) {
        errno = EINVAL;
        return -1;
    }

    _modbus_bus_setbit(bus, address);
    return 0;
}

int modbus_bus_dump(struct modbus_bus *bus)
{
    int i;

    if (bus == NULL) {
        errno = EINVAL;
        return -1;
    }

    for (i = MODBUS_BUS_BITMAPSIZE - 1; i >= 0; i--)
        fprintf(stderr, "0x%08x ", bus->bitmap[i]);
    fprintf(stderr, "\n");

    return 0;
}
