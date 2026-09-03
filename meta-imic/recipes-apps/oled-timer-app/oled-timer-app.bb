SUMMARY = "OLED and hardware timer command line application"
DESCRIPTION = "Displays terminal text and hardware timer counter on SSD1306 OLED"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://oled_timer_app.c"

S = "${WORKDIR}"

do_compile() {
    ${CC} ${CFLAGS} ${LDFLAGS} \
    ${WORKDIR}/oled_timer_app.c -o oled-timer-app
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 oled-timer-app ${D}${bindir}/oled-timer-app
}