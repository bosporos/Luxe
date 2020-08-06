#!/usr/bin/env python3

def launch(lx_quiet=None):
    import os

    lx_startup_path = os.path.dirname(os.path.realpath(__file__))
    lx_base_path = os.path.dirname(lx_startup_path)
    lx_config_dir = lx_base_path + '/config'
    lx_config_file = lx_config_dir + '/svc.ip_notifier.toml'

    lx_quiet_startup = lx_quiet if lx_quiet != None else 'LX_QUIET_STARTUP' in os.environ

    if not ( os.path.exists(lx_config_file) and os.path.isfile(lx_config_file) ):
        if not lx_quiet_startup:
            print("Cannot find config svc.ip_notifier at %s, dying" % lx_config_file)
        quit(1)

    import toml

    lx_ipn_cfg = toml.load(lx_config_file)
    if ( not 'svc' in lx_ipn_cfg
        or not 'ip_notifier' in lx_ipn_cfg['svc']
        or not 'sender' in lx_ipn_cfg['svc']['ip_notifier']
        or not 'address' in lx_ipn_cfg['svc']['ip_notifier']['sender']
        or not 'password' in lx_ipn_cfg['svc']['ip_notifier']['sender']
        or not 'to' in lx_ipn_cfg['svc']['ip_notifier']
        or not 'smtp' in lx_ipn_cfg['svc']['ip_notifier']
        or not 'host' in lx_ipn_cfg['svc']['ip_notifier']['smtp']
        or not 'port' in lx_ipn_cfg['svc']['ip_notifier']['smtp'] ):
        if not lx_quiet_startup:
            print("Malformed configuration")
        quit(2)
    lx_sndr_addr = lx_ipn_cfg['svc']['ip_notifier']['sender']['address']
    lx_sndr_pw = lx_ipn_cfg['svc']['ip_notifier']['sender']['password']
    lx_to = lx_ipn_cfg['svc']['ip_notifier']['to']
    lx_smtp_host = lx_ipn_cfg['svc']['ip_notifier']['smtp']['host']
    lx_smtp_port = lx_ipn_cfg['svc']['ip_notifier']['smtp']['port']

    import subprocess

    lx_ip_data = subprocess.Popen('hostname -I',shell=True,stdout=subprocess.PIPE).communicate()
    lx_lxdev_ip = str(lx_ip_data[0],'utf-8').strip()

    import datetime

    lx_smtp_today = datetime.date.today()

    import smtplib
    from email.mime.text import MIMEText

    lx_smtp_serv = smtplib.SMTP(lx_smtp_host, lx_smtp_port)
    lx_smtp_serv.ehlo()
    lx_smtp_serv.starttls()
    lx_smtp_serv.ehlo()
    lx_smtp_serv.login(lx_sndr_addr, lx_sndr_pw)
    lx_msg = MIMEText(lx_lxdev_ip + "\n")
    lx_msg['Subject'] = 'LXDEV IP, %s' % lx_smtp_today.strftime('%Y.%m.%d.%H.%M.%S')
    lx_msg['From'] = lx_sndr_addr
    lx_msg['To'] = lx_to
    lx_smtp_serv.sendmail(lx_sndr_addr, [lx_to], lx_msg.as_string())
    lx_smtp_serv.quit()

if __name__ == '__main__':
    launch()
