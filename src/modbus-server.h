/*
 * Copyright © 2013-2015 Michael Heimpold <mhei@heimpold.de>
 *
 * SPDX-License-Identifier: LGPL-2.1+
 */

#ifndef MODBUS_SERVER_H
#define MODBUS_SERVER_H

#include <stdarg.h>

#include "modbus.h"

enum mb_srv {
    MB_SRV_RTU,
    MB_SRV_TCP,
    MB_SRV_TCP_PI,
};

struct mb_server {
    enum mb_srv type;
    modbus_t *ctx;
};

struct mb_server *mb_server_new(enum mb_srv type, ...);
void mb_server_free(struct mb_server *srv);

#endif /* MODBUS_SERVER_H */
