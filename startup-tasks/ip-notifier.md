On boot, transmit IP to specified email address.
Specification is in 'luxe/config/svc.ip-notifier.toml'
Example specification:
```toml
[svc.ip-notifier]
sender.address = "rpi-noreply@example.com"
sender.password = "asupersecretpassword"
to = "my-inbox@example.com"
```

# TODO

To enable/disable, find `enable.svc.ip-notifier` in `services.toml`, and set it to `true` or `false`.
