QT              += widgets dbus
TEMPLATE        = lib
CONFIG          += c++11 plugin link_pkgconfig
QT              += dtkgui
QT              += dtkwidget
PKGCONFIG += dframeworkdbus

TARGET          = $$qtLibraryTarget(shot-start-plugin)
DISTFILES       += shotstart.json \
    res/screen-recording.svg

HEADERS += \
    shotstartplugin.h \
    iconwidget.h \
    tipswidget.h \
    quickpanelwidget.h \
    commoniconbutton.h

SOURCES += \
    shotstartplugin.cpp \
    iconwidget.cpp \
    tipswidget.cpp \
    quickpanelwidget.cpp \
    commoniconbutton.cpp

isEmpty(PREFIX) {
    PREFIX = /usr
}
target.path = $$PREFIX/lib/dde-dock/plugins/
file.files += $$PWD/com.deepin.dde.dock.module.shot-start-plugin.gschema.xml
file.path += $$PREFIX/share/glib-2.0/schemas/

INSTALLS += target file
RESOURCES += res.qrc
