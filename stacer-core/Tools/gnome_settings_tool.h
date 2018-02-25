#ifndef GNOME_SETTINGS_TOOL_H
#define GNOME_SETTINGS_TOOL_H

#include <QVariant>
#include "gnome_schema.h"

#include "stacer-core_global.h"

class STACERCORESHARED_EXPORT GnomeSettingsTool
{
public:
    static GnomeSettingsTool& ins() {
        static GnomeSettingsTool instance;
        return instance;
    }

    bool checkGSettings();
    bool checkUnityAvailable();

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
