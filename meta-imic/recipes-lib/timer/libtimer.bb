# Recipe created by recipetool
# This is the basis of a recipe and may need further editing in order to be fully functional.
# (Feel free to remove these comments when editing.)

# Unable to find any files that looked like license statements. Check the accompanying
# documentation and source headers and set LICENSE and LIC_FILES_CHKSUM accordingly.
#
# NOTE: LICENSE is being set to "CLOSED" to allow you to at least start building - if
# this is not accurate with respect to the licensing of the software being built (it
# will not be in most cases) you must specify the correct value before using this
# recipe for anything other than initial testing/development!
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://timer.c	\
		file://timer.h"

S = "${WORKDIR}"

# NOTE: no Makefile found, unable to determine what needs to be done

do_configure () {
	# Specify any needed configure commands here
	:
}

do_compile () {
	# 
	${CC} -I. -c timer.c -o timer.o
	${CC} -shared -o libtimer.so timer.o
}

do_install () {
    	install -d ${D}${libdir}
	install -m 0755 libtimer.so ${D}${libdir}/libtimer.so.1.0
    	ln -sf libtimer.so.1.0 ${D}${libdir}/libtimer.so.1
    	ln -sf libtimer.so.1 ${D}${libdir}/libtimer.so

    	install -d ${D}${includedir}
    	install -m 0644 timer.h ${D}${includedir}/
}

INSANE_SKIP:${PN} = "ldflags"
INSANE_SKIP:${PN}-dev = "ldflags"

FILES:${PN} += "${libdir}/libtimer.so*"
FILES:${PN}-dev += "${libdir}/libtimer.so"
