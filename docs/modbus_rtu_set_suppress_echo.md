# modbus_rtu_set_suppress_echo

## Name

modbus_rtu_set_suppress_echo - enable/disable local echo suppression

## Synopsis

```c
int modbus_rtu_set_suppress_echo(modbus_t *ctx, bool on)
```

## Description

The `modbus_rtu_set_suppress_echo()` function shall enable or disable
local echo suppression workaround of the libmodbus context `ctx`.

Enable this on hardware platforms which locally echo all bytes which
are transmitted. When enabled, it will immediately receive the self-made
packet to clean up the incoming buffer.

Do not enable it on hardware platforms which do _not_ echo or when
local echo is turned off. Otherwise valid response packets are wrongly
removed from incoming buffer.

This function can only be used with a context using a RTU backend.

## Return value

The `modbus_rtu_set_suppress_echo()` function shall return 0 if successful.
Otherwise it shall return -1 and set errno.

## Errors

- *EINVAL*, the libmodbus backend is not RTU.

## See also

- [modbus_rtu_get_suppress_echo](modbus_rtu_get_suppress_echo.md)
