#-------------------------------------------------
#
# Project created by QtCreator 2017-07-02T15:48:51
#
#-------------------------------------------------

QT       -= gui

QT       += core network

CONFIG += c++11

TARGET = stacer-core
TEMPLATE = lib

DEFINES += STACERCORE_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Utils/command_util.cpp \
    Utils/file_util.cpp \
    Info/network_info.cpp \
    Info/cpu_info.cpp \
    Info/disk_info.cpp \
    Info/memory_info.cpp \
    Info/system_info.cpp \
    Utils/format_util.cpp \
    Tools/service_tool.cpp \
    Tools/package_tool.cpp \
    Info/process_info.cpp \
    Info/process.cpp \
    Tools/apt_source_tool.cpp \
    Tools/gnome_settings_tool.cpp \
    Types/Applications/desktop.cpp

HEADERS += \
        stacer-core_global.h \ 
    Utils/command_util.h \
    Info/network_info.h \
    Info/cpu_info.h \
    Info/disk_info.h \
    Info/memory_info.h \
    Info/system_info.h \
    Utils/format_util.h \
    Utils/file_util.h \
    Tools/service_tool.h \
    Tools/package_tool.h \
    Info/process_info.h \
    Info/process.h \
    Tools/apt_source_tool.h \
    Tools/gnome_settings_tool.h \
    Tools/gnome_schema.h \
    Types/Applications/desktop.h

unix {
    target.path = /usr/lib
    LIBS     += $$[PCRECPP_LIBRARIES]
    INCLUDEPATH += $$[PCRECPP_INCLUDE_DIRS]
    INSTALLS += target
}
