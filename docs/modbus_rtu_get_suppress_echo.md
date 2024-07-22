# modbus_rtu_get_suppress_echo

## Name

modbus_rtu_get_suppress_echo - get local echo suppression status

## Synopsis

```c
int modbus_rtu_get_suppress_echo(modbus_t *ctx)
```

## Description

The `modbus_rtu_get_suppress_echo()` function shall return the current
enabled/disable state of local echo suppression workaround of
the libmodbus context `ctx`.

This function can only be used with a context using a RTU backend.

## Return value

The `modbus_rtu_get_suppress_echo()` function shall return 0 if workaround
is disable, 1 when it is enabled.
Otherwise it shall return -1 and set errno.

## Errors

- *EINVAL*, the libmodbus backend is not RTU.

## See also

- [modbus_rtu_set_suppress_echo](modbus_rtu_set_suppress_echo.md)
