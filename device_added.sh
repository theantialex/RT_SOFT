#!/bin/bash
mkdir /media/usb
mount /dev/my_usb /media/usb
rsync -av /var/log/boot.log /media/usb/log.txt
umount /media/usb



