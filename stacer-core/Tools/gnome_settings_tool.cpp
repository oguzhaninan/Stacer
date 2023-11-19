#include "gnome_settings_tool.h"
#include "Utils/command_util.h"
#include <QDebug>

bool GnomeSettingsTool::checkGSettings()
{
    return CommandUtil::isExecutable("gsettings");
}

bool GnomeSettingsTool::checkUnityAvailable()
{
    QStringList args = { "list-relocatable-schemas" };

    try {
        QString result = CommandUtil::exec("gsettings", args);
        QStringList schemas = result.split('\n').filter(QRegExp(GSchemas::Unity::Shell));

        return ! schemas.isEmpty();

    } catch(const QString &ex) {
        qWarning() << ex;
    }

    return false;
}

QVariant GnomeSettingsTool::getValue(const QString schema, const QString key, const QString schemaPath)
{
    QStringList args = { "get" };

    if (schemaPath.isEmpty()) {
        args << schema;
    } else {
        args << QString("%1:%2").arg(schema).arg(schemaPath);
    }
    args << key;

    QString result;
    try {
        result = CommandUtil::exec("gsettings", args);
    } catch (const QString &ex) {
        qDebug() << ex;
    }

    return QVariant(result.trimmed());
}

QString GnomeSettingsTool::getValueS(const QString schema, const QString key, const QString schemaPath)
{
    return getValue(schema, key, schemaPath).toString();
}

bool GnomeSettingsTool::getValueB(const QString schema, const QString key, const QString schemaPath)
{
    return getValue(schema, key, schemaPath).toBool();
}

int GnomeSettingsTool::getValueI(const QString schema, const QString key, const QString schemaPath)
{
    return getValue(schema, key, schemaPath).toInt();
}

float GnomeSettingsTool::getValueF(const QString schema, const QString key, const QString schemaPath)
{
    return getValue(schema, key, schemaPath).toFloat();
}

void GnomeSettingsTool::setValue(const QString schema, const QString key, const QVariant value, const QString schemaPath)
{
    QStringList args = { "set" };

    if (schemaPath.isEmpty()) {
        args << schema;
    } else {
        args << QString("%1:%2").arg(schema).arg(schemaPath);
    }
    args << key << value.toString();

    try {
        CommandUtil::exec("gsettings", args);
    } catch (const QString &ex) {
        qDebug() << ex;
    }
}

void GnomeSettingsTool::setValueS(const QString schema, const QString key, const QString value, const QString schemaPath)
{
    setValue(schema, key, QVariant(value), schemaPath);
}

void GnomeSettingsTool::setValueB(const QString schema, const QString key, const bool value, const QString schemaPath)
{
    setValue(schema, key, QVariant(value), schemaPath);
}

void GnomeSettingsTool::setValueI(const QString schema, const QString key, const int value, const QString schemaPath)
{
    setValue(schema, key, QVariant(value), schemaPath);
}

void GnomeSettingsTool::setValueF(const QString schema, const QString key, const float value, const QString schemaPath)
{
    setValue(schema, key, QVariant(value), schemaPath);
}
