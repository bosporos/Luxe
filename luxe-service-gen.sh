#!/usr/bin/env bash

lsg_wdir=$(cd "$(dirname "${BASH_SOURCE[0]}")" > /dev/null 2>&1 ; pwd)
[[ "x${LSG_USER}" != "x" ]] && lsg_user="${LSG_USER}" || lsg_user=$(whoami)

echo "[Unit]
Description=LUXE
After=network.target

[Service]
ExecStart=/usr/bin/python3 -u luxe-launcher.py
WorkingDirectory=${lsg_wdir}
StandardOutput=inherit
StandardError=inherit
Restart=always
User=${lsg_user}

[Install]
WantedBy=multi-user.target
"
