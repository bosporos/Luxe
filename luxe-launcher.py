#!/usr/bin/env python3

def luxe_enable_services(lx_svc_cfg, lx_quiet):
    lx_svcls = lx_svc_cfg['enable']['svc']
    if 'ip_notifier' in lx_svcls and lx_svcls['ip_notifier']:
        from startup_tasks import ip_notifier
        ip_notifier.launch(lx_quiet)

def luxe_launch():
    import os

    lx_startup_path = os.path.dirname(os.path.realpath(__file__))
    lx_base_path = lx_startup_path
    lx_config_dir = lx_base_path + '/config'
    lx_config_file = lx_config_dir + '/services.toml'

    lx_quiet_startup = 'LX_QUIET_STARTUP' in os.environ
    if not ( os.path.exists(lx_config_file) and os.path.isfile(lx_config_file) ):
        if not lx_quiet_startup:
            print("Cannot find config 'services' at %s, dying" % lx_config_file)
        quit(1)

    import toml

    lx_svc_cfg = toml.load(lx_config_file)
    if 'enable' in lx_svc_cfg and 'svc' in lx_svc_cfg['enable']:
        luxe_enable_services(lx_svc_cfg, lx_quiet=lx_quiet_startup)

if __name__ == '__main__':
    luxe_launch()
