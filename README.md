SW:
WSL2 - Ubuntu 20.04 LTS
WinSCP: https://winscp.net/eng/download.php 
Tera Term: https://github.com/TeraTermProject/teraterm/releases
VirtualBox: https://www.virtualbox.org/wiki/Downloads
VSCode: https://code.visualstudio.com/
MobaXterm: https://mobaxterm.mobatek.net/download.html

HW
Beaglebone Black: https://www.proe.vn/beaglebone-black-rve-c-development-board-beagle-bone
USB to uart: https://www.thegioiic.com/ft232rl-cap-chuyen-doi-usb-to-ttl-uart
SD Card (>4G)
Oled LCD: https://www.thegioiic.com/lcd-oled-ssd1306-0-91inch-128x32-chu-xanh-duong-4-chan-giao-tiep-iic
ADXL345: https://www.thegioiic.com/adxl345-mach-cam-bien-gia-toc-3-truc
Leds, Dây bẹ, Trở, Cáp mạng, Breadboard, Button...

Yocto building steps:
$ source poky/oe-init-build-env bbb_full_cmd
enable MACHINE in conf/local.conf
MACHINE ?= "beaglebone-yocto"
$ bitbake core-image-full-cmdline

