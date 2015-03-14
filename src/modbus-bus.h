/*
 * Copyright © 2013-2015 Michael Heimpold <mhei@heimpold.de>
 *
 * SPDX-License-Identifier: LGPL-2.1+
 */

#ifndef MODBUS_BUS_H
#define MODBUS_BUS_H

#include "modbus.h"

MODBUS_BEGIN_DECLS

/* Count of integers needed to map each valid bus address to
 * one bit.
 */
#define MODBUS_BUS_BITMAPSIZE (256 / (sizeof(int) * 8))

/* Modell of bus */
struct modbus_bus {
    /* This bitmap marks every modbus address which is free (available to use)
     * with the corresponndig bit set.
     */
    int bitmap[MODBUS_BUS_BITMAPSIZE];
};

MODBUS_API int modbus_bus_init(struct modbus_bus *bus);
MODBUS_API int modbus_bus_request_address(struct modbus_bus *bus);
MODBUS_API int modbus_bus_acquire_address(struct modbus_bus *bus, int address);
MODBUS_API int modbus_bus_release_address(struct modbus_bus *bus, int address);
MODBUS_API int modbus_bus_dump(struct modbus_bus *bus);

MODBUS_END_DECLS

#endif /* MODBUS_BUS_H */
