#!/bin/sh
fbgrab /tmp/background.png
env QT_QPA_PLATFORM=kobo chroot /kobo /usr/local/bin/charger-controller.bin
fbink -g file=/tmp/background.png