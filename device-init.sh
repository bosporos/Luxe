#!/usr/bin/env bash

echo "CONTROLS: LX_HOSTNAME, LSG_USER"

if [[ "x${LX_HOSTNAME}" = "x" ]] ; then
  LX_HOSTNAME="luxe-device"
fi

# Step one, hostname [default] [can-disable]
if ! [[ "$*" =~ .*no-update-hostname.* ]] ; then
  if [[ "x$(hostname)" != "x${LX_HOSTNAME}" ]] ; then
    lx_original_hostname=$(hostname)
    sudo hostnamectl set-hostname ${LX_HOSTNAME}
    let sand_trap=0
    printf "127.0.0.1 ${LX_HOSTNAME}\n" | sudo tee -a /etc/hosts || let sand_trap=1
    if [[ "x$(hostname)" != "x${LX_HOSTNAME}" ]] ; then
      printf "Could not change hostname, reseting...\n"
      sudo hostnamectl set-hostname ${lx_original_hostname}
      if $(exit sand_trap) ; then
        lx_tmp_hostsf=$(mktemp || hosts.tmp)
        head -n -1 /etc/hosts > ${lx_tmp_hostsf}
        sudo sh -c 'mv ${lx_tmp_hostsf} /etc/hosts'
        rm ${lx_tmp_hostsf}
      fi
    fi
  fi
fi

# Step two, update repo [optional]
if [[ "$*" =~ .*update.* ]] ; then
  printf "Command [update] detected...\nAttempting to update...\n"
  if [ "${_IP}" ] ; then
    printf "Has IP address [${_IP}], attempting to connect to upstream\n"
    git pull
  fi
fi

# Step three, install [optional]
if [[ "$*" =~ .*install.* ]] ; then
  printf "Command [install] detected...\nAttempting to install...\n"
  sh "$(cd "$(dirname "${BASH_SOURCE[0]}")" > /dev/null 2>&1 ; pwd)/install.sh"
fi
