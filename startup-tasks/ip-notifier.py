#!/usr/bin/env python3
import os
import toml

lx_startup_path = os.path.dirname(os.path.realpath(__file__))
lx_base_path = os.path.dirname(lx_startup_path)
lx_config_path = lx_base_path + '/config'
lx_config_file = lx_config_path + '/svc.ip-notifier.toml'

lx_quiet_startup = 'LX_QUIET_STARTUP' in os.environ

if not ( os.path.exists(lx_config_file) and os.path.isfile(lx_config_file) ):
    if not lx_quiet_startup:
        print("Cannot find config svc.ip-notifier, dying")
    quit(1)
lx_ipn_cfg = toml.load(lx_config_file)
if not 'svc' in lx_ipn_cfg
    or not 'ip-notifier' in lx_ipn_cfg['svc']
    or not 'sender' in lx_ipn_cfg['svc']['ip-notifier']
    or not 'address' in lx_ipn_cfg['svc']['ip-notifier']['sender']
    or not 'password' in lx_ipn_cfg['svc']['ip-notifier']['sender']
    or not 'to' in lx_ipn_cfg['svc']['ip-notifier']
    or not 'smtp' in lx_ipn_cfg['svc']['ip-notifier']
    or not 'host' in lx_ipn_cfg['svc']['ip-notifier']['smtp']
    or not 'port' in lx_ipn_cfg['svc']['ip-notifier']['smtp']:
    if not lx_quiet_startup:
        print("Malformed configuration")
    exit(2)
lx_sndr_addr = lx_ipn_cfg['svc']['ip-notifier']['sender']['address']
lx_sndr_pw = lx_ipn_cfg['svc']['ip-notifier']['sender']['address']
lx_to = lx_ipn_cfg['svc']['ip-notifier']['to']
lx_smtp_host = lx_ipn_cfg['svc']['ip-notifier']['smtp']['host']
lx_smtp_port = lx_ipn_cfg['svc']['ip-notifier']['smtp']['port']

import subprocess

lx_proc = subprocess.Popen('hostname -i',shell=True,stdout=subprocess.PIPE)
lx_ip_data = p.communicate()
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
lx_msg['Subject'] = 'LXDEV IP, %s' % today.strftime('%Y.%m.%d.%H.%M.%S')
lx_msg['From'] = lx_sndr_addr
lx_msg['To'] = lx_to
lx_smtp_serv.sendmail(lx_sndr_addr, [lx_to], lx_msg.as_string())
lx_smtp_serv.quit()
