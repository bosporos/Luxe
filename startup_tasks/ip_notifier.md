On boot, transmit IP to specified email address.
Specification is in 'luxe/config/svc.ip_notifier.toml'
Example specification:
```toml
[svc.ip_notifier]
  sender.address = "rpi-noreply@example.com"
  sender.password = "asupersecretpassword"
  to = "my-inbox@example.com"
  smtp.host = "smtp.example.com"
  smtp.port = 587
```

# TODO

To enable/disable, find `enable.svc.ip_notifier` in `services.toml`, and set it to `true` or `false`.
