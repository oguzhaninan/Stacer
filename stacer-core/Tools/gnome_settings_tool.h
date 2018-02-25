#ifndef GNOME_SETTINGS_TOOL_H
#define GNOME_SETTINGS_TOOL_H

#include <QVariant>

namespace GSchemaPaths {
    const QString Unity = "/org/compiz/profiles/unity/plugins/unityshell/";
    const QString UnityLauncher = "/com/canonical/unity/launcher/";

}

namespace GSchemas {
    namespace Unity {
        const QString Shell = "org.compiz.unityshell";
        const QString Launcher = "com.canonical.Unity.Launcher";
        const QString Lens = "com.canonical.Unity.Lenses";
        const QString AppLens = "com.canonical.Unity.ApplicationsLens";
        const QString FileLens = "com.canonical.Unity.FilesLens";
        const QString DateTime = "com.canonical.indicator.datetime";
        const QString Sound = "com.canonical.indicator.sound";
        const QString Session = "com.canonical.indicator.session";
    }
}

namespace GSchemaKeys {
    namespace Unity {
        const QString LauncherHideMode = "launcher-hide-mode";
        const QString RevealTrigger = "reveal-trigger";
        const QString EdgeResponsiveness = "edge-responsiveness";
        const QString LauncherMinimizeApp = "launcher-minimize-window";
        const QString LauncherOpacity = "launcher-opacity";
        const QString LauncherVisibility = "num-launchers";
        const QString LauncherPosition = "launcher-position";
        const QString LauncherIconSize = "icon-size";
        const QString DashBlur = "dash-blur-experimental";
        const QString SearchOnlineResource = "remote-content-search";
        const QString DisplayAvailableApps = "display-available-apps";
        const QString DisplayRecentApps = "display-recent-apps";
        const QString EnableSearchFile = "use-locate";
        const QString PanelOpacity = "panel-opacity";
        const QString ShowDateTime = "show-clock";
        const QString TimeFormat = "time-format";
        const QString ShowSeconds = "show-seconds";
        const QString ShowDate = "show-date";
        const QString ShowDay = "show-day";
        const QString ShowCalendar = "show-calendar";
        const QString ShowVolume = "visible";
        const QString ShowMyName = "show-real-name-on-panel";
    }
}

namespace GValues {
    enum RevealLocation {
        Left,
        TopLeft
    };

    enum LauncherVisibility {
        AllDesktop,
        PrimaryDesktop
    };
}

class GnomeSettingsTool
{
public:
    static GnomeSettingsTool& ins() {
        static GnomeSettingsTool instance;
        return instance;
    }

    bool checkGSettings();
    QVariant getValue(const QString schema, const QString key, const QString schemaPath = QString());
    void setValue(const QString schema, const QString key, const QVariant value, const QString schemaPath = QString());

    QString getValueS(const QString schema, const QString key, const QString schemaPath = QString());
    bool getValueB(const QString schema, const QString key, const QString schemaPath = QString());
    int getValueI(const QString schema, const QString key, const QString schemaPath = QString());
    float getValueF(const QString schema, const QString key, const QString schemaPath = QString());

    void setValueS(const QString schema, const QString key, const QString value, const QString schemaPath = QString());
    void setValueB(const QString schema, const QString key, const bool value, const QString schemaPath = QString());
    void setValueI(const QString schema, const QString key, const int value, const QString schemaPath = QString());
    void setValueF(const QString schema, const QString key, const float value, const QString schemaPath = QString());
};

#endif // GNOME_SETTINGS_TOOL_H
