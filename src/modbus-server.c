/*
 * Copyright © 2013-2015 Michael Heimpold <mhei@heimpold.de>
 *
 * SPDX-License-Identifier: LGPL-2.1+
 */

#include <stdarg.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "modbus.h"
#include "modbus-server.h"

/*
 * Create a new Modbus server object.
 *
 * Examples: mb_server_new(MB_SRV_RTU, "/dev/ttyUSB0", 115200, 8, 'N', 1, 247);
 *           mb_server_new(MB_SRV_TCP, "127.0.0.1", 502);
 *           mb_server_new(MB_SRV_TCP_PI, "::1", "502");
 */
struct mb_server *mb_server_new(enum mb_srv type, ...)
{
    struct mb_server *srv = NULL;
    va_list ap;

    srv = (struct mb_server *)calloc(1, sizeof(*srv));
    if (!srv) {
        errno = ENOMEM;
        goto error1;
    }

    srv->type = type;

    va_start(ap, type);

    switch (type) {
        case MB_SRV_RTU: {
            char *device = va_arg(ap, char *);
            int baud = va_arg(ap, int);
            int databits = va_arg(ap, int);
            int parity = va_arg(ap, int);
            int stopbits = va_arg(ap, int);
            int slave = va_arg(ap, int);

            srv->ctx = modbus_new_rtu(device, baud, parity, databits, stopbits);
            if (srv->ctx == NULL)
                goto error2;
            if (modbus_set_slave(srv->ctx, slave) == -1)
                goto error2;
            if (modbus_connect(srv->ctx) == -1)
                goto error2;
            break;
        }

        case MB_SRV_TCP: {
            char *ip = va_arg(ap, char *);
            int port = va_arg(ap, int);
            int nb_connection = va_arg(ap, int);

            srv->ctx = modbus_new_tcp(ip, port);
            if (srv->ctx == NULL)
                goto error2;
            if (modbus_tcp_listen(srv->ctx, nb_connection) == -1)
                goto error2;
            break;
        }

        case MB_SRV_TCP_PI: {
            char *node = va_arg(ap, char *);
            char *service = va_arg(ap, char *);
            int nb_connection = va_arg(ap, int);

            srv->ctx = modbus_new_tcp_pi(node, service);
            if (srv->ctx == NULL)
                goto error2;
            if (modbus_tcp_pi_listen(srv->ctx, nb_connection) == -1)
                goto error2;
            break;
        }

        default:
            errno = EINVAL;
            goto error1;
    }

    va_end(ap);
    return srv;

error2:
    if (srv->ctx)
        modbus_free(srv->ctx);

error1:
    va_end(ap);

    if (srv)
        free(srv);
    return NULL;
}

void mb_server_free(struct mb_server *srv)
{
    if (!srv)
        return;

    if (srv->ctx)
        modbus_free(srv->ctx);

    free(srv);
}
