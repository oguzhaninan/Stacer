#ifndef GNOME_SETTINGS_TOOL_H
#define GNOME_SETTINGS_TOOL_H

#include <QVariant>

namespace GSchemaPaths {
    const QString Unity = "/org/compiz/profiles/unity/plugins/unityshell/";
}

namespace GSchemas {
    const QString UnityShell = "org.compiz.unityshell";
}

namespace GSchemaKeys {
    const QString LauncherHideMode = "launcher-hide-mode";
    const QString RevealTrigger = "reveal-trigger";
    const QString EdgeResponsiveness = "edge-responsiveness";
}

namespace GValues {
    enum RevealLocation {
        Left = 0,
        TopLeft = 1
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
    void setValue(const QString schema, const QString key, const QVariant value, const QString schemaPath);

    QString getValueS(const QString schema, const QString key, const QString schemaPath);
    bool getValueB(const QString schema, const QString key, const QString schemaPath);
    int getValueI(const QString schema, const QString key, const QString schemaPath);
    float getValueF(const QString schema, const QString key, const QString schemaPath);

    void setValueS(const QString schema, const QString key, const QString value, const QString schemaPath);
    void setValueB(const QString schema, const QString key, const bool value, const QString schemaPath);
    void setValueI(const QString schema, const QString key, const int value, const QString schemaPath);
    void setValueF(const QString schema, const QString key, const float value, const QString schemaPath);
};

#endif // GNOME_SETTINGS_TOOL_H
