DESCRIPTION = "A console-only image with more full-featured Linux system functionality installed."

require recipes-extended/images/core-image-full-cmdline.bb

#image add ssh
IMAGE_INSTALL:append = " openssh openssh-sftp-server"

#network
IMAGE_INSTALL:append = " ethtool iproute2 net-tools"

# systemd userspace
IMAGE_INSTALL:append = " systemd"

#Custom image
IMAGE_INSTALL:append = " hello my-network oled-timer-app ssd1306-demo"
