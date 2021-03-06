
QT += multimedia-private network
CONFIG += no_private_qt_headers_warning

qtHaveModule(widgets) {
    QT += widgets multimediawidgets-private
    DEFINES += HAVE_WIDGETS
}

LIBS += -lqgsttools_p

CONFIG += link_pkgconfig

PKGCONFIG += \
    gstreamer-$$GST_VERSION \
    gstreamer-base-$$GST_VERSION \
    gstreamer-audio-$$GST_VERSION \
    gstreamer-video-$$GST_VERSION \
    gstreamer-pbutils-$$GST_VERSION

maemo*:PKGCONFIG +=gstreamer-plugins-bad-$$GST_VERSION

mir: {
    DEFINES += HAVE_MIR
}

config_resourcepolicy {
    DEFINES += HAVE_RESOURCE_POLICY
    PKGCONFIG += libresourceqt1
}

config_xvideo:qtHaveModule(widgets) {
    DEFINES += HAVE_XVIDEO
    LIBS += -lXv -lX11 -lXext
}

config_gstreamer_appsrc {
    PKGCONFIG += gstreamer-app-$$GST_VERSION
    DEFINES += HAVE_GST_APPSRC
    LIBS += -lgstapp-$$GST_VERSION
}

