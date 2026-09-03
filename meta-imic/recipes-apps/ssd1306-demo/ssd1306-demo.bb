SUMMARY = "SSD1306 I2C userspace demonstration"
DESCRIPTION = "Port of the xmc2go SSD1306 userspace application for BeagleBone Black"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = " \
    file://main.c \
    file://ssd1306.c \
    file://ssd1306.h \
    file://ssd1306_conf.h \
    file://ssd1306_fonts.c \
    file://ssd1306_fonts.h \
"

S = "${WORKDIR}"

do_compile() {
    ${CC} ${CFLAGS} ${LDFLAGS} -I${S} \
        main.c ssd1306.c ssd1306_fonts.c \
        -o ssd1306-demo
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 ssd1306-demo ${D}${bindir}/ssd1306-demo
}
