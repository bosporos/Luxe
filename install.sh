#!/usr/bin/env bash

# Create configuration
echo "Creating configuration directory"
mkdir -p ./config
echo "Copying configuration templates..."
cp -vn config-template/* config

# Create & register service
echo "Generating service..."
GENSVC="luxe-$(date +%Y.%m.%d.%H.%M.%S).service"
./luxe-service-gen.sh > ./${GENSVC} || rm ./${GENSVC}
sudo cp ./${GENSVC} /etc/systemd/system/luxe.service
read -p "Enable service {luxe.service}? [Y/n] (Y) " -n 1 -r
if [[ $REPLY =~ ^[Yy]$ ]] ; then
  echo "Enabling service..."
  sudo systemctl enable luxe.service
else
  echo "Doing nothing..."
fi

sudo apt install python3-pip
pip3 install toml

# Utility (f.e., if you want to edit the configs)
read -p "Reboot? [Y/n] (n) " -n 1 -r
if [[ $REPLY =~ ^[Yy]$ ]] ; then
  echo "Rebooting..."
  sudo shutdown --reboot +0
else
  echo "Not rebooting... use \`\`sudo shutdown --reboot +0'' to reboot."
fi

echo "Fin."
exit 0
