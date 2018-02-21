#include "apt_source_tool.h"
#include "Utils/command_util.h"
#include "Utils/file_util.h"
#include <QDebug>

bool AptSourceTool::checkSourceRepository()
{
    QDir sourceList(APT_SOURCES_LIST_D_PATH);

    bool isExists = sourceList.exists();

    return isExists;
}

void AptSourceTool::removeAPTSource(const QString source)
{
    QStringList args = { "-r", "-y", source };
    try {
        CommandUtil::sudoExec("add-apt-repository", args);
    } catch(QString &ex) {
        qDebug() << ex;
    }
}

void AptSourceTool::changeStatus(const APTSourcePtr aptSource, const bool status)
{
    QStringList sourceFileContent = FileUtil::readListFromFile(aptSource->filePath);

    int pos = -1;
    for (int i = 0; i < sourceFileContent.count(); ++i) {
        int _pos = sourceFileContent[i].indexOf(aptSource->source);
        if (_pos != -1) {
            pos = i;
            break;
        }
    }

    if (pos != -1) {
        QString line = sourceFileContent.at(pos);

        line.replace("#", "");

        if (status) {
            sourceFileContent.replace(pos, line.trimmed());
        } else {
            sourceFileContent.replace(pos, "# " + line.trimmed());
        }
    }

    QStringList args = { aptSource->filePath };

    QByteArray data = sourceFileContent.join('\n').toUtf8();

    CommandUtil::sudoExec("tee", args, data);
}

QList<APTSourcePtr> AptSourceTool::getSourceList()
{
    QList<APTSourcePtr> aptSourceList;

    QDir aptSourceListDir(APT_SOURCES_LIST_D_PATH);

    QFileInfoList infoList = aptSourceListDir.entryInfoList({ "*.list"/*, "*.save"*/ },
                                                            QDir::Files, QDir::Name);
    infoList.append(QFileInfo(APT_SOURCES_LIST_PATH)); // sources.list

    for (const QFileInfo &info : infoList) {

        QStringList fileContent = FileUtil::readListFromFile(info.absoluteFilePath());

        for (const QString &line : fileContent) {
            QString _line = line.trimmed();

            if (! _line.isEmpty()) {

                APTSourcePtr aptSource(new APTSource);
                aptSource->filePath = info.absoluteFilePath();

                if (_line.startsWith(QChar('#'))) { // is deactive
                    aptSource->isActive = false;
                    _line.replace("#", "");
                } else {
                    aptSource->isActive = true;
                }

                QStringList _lineList = _line.trimmed().split(QChar(' '));

                bool isBinary = _lineList.first() == "deb";
                bool isSource = _lineList.first() == "deb-src";

                // example "deb [arch=amd64] http://packages.microsoft.com/repos/vscode stable main"
                if (isBinary || isSource) {
                    aptSource->source = _lineList.join(' ');
                    aptSource->isSource = isSource;
                    aptSourceList.append(aptSource);
                }
            }
        }
    }

    return aptSourceList;
}


