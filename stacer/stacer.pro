QT       += core gui charts svg concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = stacer
TEMPLATE = app

CONFIG += c++11

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
        main.cpp \
    app.cpp \
    Pages/Dashboard/circlebar.cpp \
    Pages/Dashboard/linebar.cpp \
    Pages/StartupApps/startup_app.cpp \
    Pages/StartupApps/startup_app_edit.cpp \
    Pages/StartupApps/startup_apps_page.cpp \
    Pages/Services/service_item.cpp \
    Managers/app_manager.cpp \
    Managers/tool_manager.cpp \
    Managers/info_manager.cpp \
    Pages/Resources/history_chart.cpp \
    Pages/SystemCleaner/system_cleaner_page.cpp \
    Pages/Uninstaller/uninstaller_page.cpp \
    Pages/Services/services_page.cpp \
    Pages/Resources/resources_page.cpp \
    Pages/Dashboard/dashboard_page.cpp \
    Pages/Processes/processes_page.cpp \
    Pages/Settings/settings_page.cpp \
    Pages/AptSourceManager/apt_source_manager_page.cpp \
    Pages/AptSourceManager/apt_source_repository_item.cpp \
    Pages/GnomeSettings/gnome_settings_page.cpp \
    Pages/GnomeSettings/unity_settings.cpp \
    Pages/GnomeSettings/window_manager_settings.cpp \
    Pages/GnomeSettings/appearance_settings.cpp \
    slidingstackedwidget.cpp \
    feedback.cpp \
    Pages/AptSourceManager/apt_source_edit.cpp \
    Managers/setting_manager.cpp

HEADERS += \
    app.h \
    Pages/Dashboard/circlebar.h \
    Pages/Dashboard/linebar.h \
    Pages/StartupApps/startup_app.h \
    Pages/StartupApps/startup_app_edit.h \
    Pages/StartupApps/startup_apps_page.h \
    Pages/Services/service_item.h \
    Managers/app_manager.h \
    Managers/tool_manager.h \
    Managers/info_manager.h \
    Pages/Resources/history_chart.h \
    Pages/SystemCleaner/system_cleaner_page.h \
    Pages/Uninstaller/uninstaller_page.h \
    Pages/Resources/resources_page.h \
    Pages/Processes/processes_page.h \
    Pages/Dashboard/dashboard_page.h \
    Pages/Services/services_page.h \
    Pages/Settings/settings_page.h \
    Pages/AptSourceManager/apt_source_manager_page.h \
    Pages/AptSourceManager/apt_source_repository_item.h \
    Pages/GnomeSettings/gnome_settings_page.h \
    Pages/GnomeSettings/unity_settings.h \
    Pages/GnomeSettings/window_manager_settings.h \
    Pages/GnomeSettings/appearance_settings.h \
    slidingstackedwidget.h \
    utilities.h \
    feedback.h \
    Pages/AptSourceManager/apt_source_edit.h \
    Managers/setting_manager.h \
    signal_mapper.h

FORMS += \
    app.ui \
    Pages/Uninstaller/uninstallerpage.ui \
    Pages/Dashboard/circlebar.ui \
    Pages/Dashboard/linebar.ui \
    Pages/StartupApps/startup_app.ui \
    Pages/StartupApps/startup_app_edit.ui \
    Pages/StartupApps/startup_apps_page.ui \
    Pages/Services/service_item.ui \
    Pages/Resources/history_chart.ui \
    Pages/SystemCleaner/system_cleaner_page.ui \
    Pages/Dashboard/dashboard_page.ui \
    Pages/Processes/processes_page.ui \
    Pages/Resources/resources_page.ui \
    Pages/Services/services_page.ui \
    Pages/Settings/settings_page.ui \
    Pages/AptSourceManager/apt_source_manager_page.ui \
    Pages/AptSourceManager/apt_source_repository_item.ui \
    Pages/GnomeSettings/gnome_settings_page.ui \
    Pages/GnomeSettings/unity_settings.ui \
    Pages/GnomeSettings/window_manager_settings.ui \
    Pages/GnomeSettings/appearance_settings.ui \
    feedback.ui \
    Pages/AptSourceManager/apt_source_edit.ui

TRANSLATIONS += \
    ../translations/stacer_ar.ts \
    ../translations/stacer_de.ts \
    ../translations/stacer_en.ts \
    ../translations/stacer_ca-es.ts \
    ../translations/stacer_es.ts \
    ../translations/stacer_fr.ts \
    ../translations/stacer_hi.ts \	
    ../translations/stacer_kn.ts \
    ../translations/stacer_oc.ts \
    ../translations/stacer_pl.ts \
    ../translations/stacer_pt.ts \
    ../translations/stacer_ru.ts \
    ../translations/stacer_sv.ts \
    ../translations/stacer_tr.ts \
    ../translations/stacer_zh-cn.ts

RESOURCES += \
    static.qrc

unix:!macx: LIBS += -L$$OUT_PWD/../stacer-core/ -lstacer-core

INCLUDEPATH += $$PWD/../stacer-core
DEPENDPATH += $$PWD/../stacer-core
