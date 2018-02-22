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

void AptSourceTool::changeSource(const APTSourcePtr aptSource, const QString newSource)
{
    QStringList sourceFileContent = FileUtil::readListFromFile(aptSource->filePath);

    // find line index
    int pos = -1;
    for (int i = 0; i < sourceFileContent.count(); ++i) {
        int _pos = sourceFileContent[i].indexOf(aptSource->source);
        if (_pos != -1) {
            pos = i;
            break;
        }
    }

    if (pos != -1) {
        sourceFileContent.replace(pos, newSource);
    }

    QStringList args = { aptSource->filePath };

    QByteArray data = sourceFileContent.join('\n').toUtf8();

    CommandUtil::sudoExec("tee", args, data);
}

void AptSourceTool::changeStatus(const APTSourcePtr aptSource, const bool status)
{
    QString newSource = aptSource->source;

    newSource.replace("#", "");

    if (! status) { // if is deactive
        newSource = "# " + newSource.trimmed();
    }

    changeSource(aptSource, newSource);
}

QList<APTSourcePtr> AptSourceTool::getSourceList()
{
    QList<APTSourcePtr> aptSourceList;

    QDir aptSourceListDir(APT_SOURCES_LIST_D_PATH);

    QFileInfoList infoList = aptSourceListDir.entryInfoList({ "*.list"/*, "*.save"*/ },
                                                            QDir::Files, QDir::Time);
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
                    _line.replace("#", ""); // remove comment
                } else {
                    aptSource->isActive = true;
                }

                // if has options
                if (_line.contains('[')) {
                    int pos1 = _line.indexOf('['), pos2 = _line.indexOf(']');

                    aptSource->options = _line.mid(pos1, pos2-pos1+1);
                    _line.replace(aptSource->options, ""); // delete options section
                }

                QStringList sourceColumns = _line.trimmed().split(QRegExp("\\s+"));
                bool isBinary = sourceColumns.first() == "deb";
                bool isSource = sourceColumns.first() == "deb-src";

                // example "deb http://packages.microsoft.com/repos/vscode stable main"
                if (isBinary || isSource) {
                    aptSource->isSource = isSource;
                    aptSource->uri = sourceColumns.at(1);
                    aptSource->distribution = sourceColumns.at(2);
                    aptSource->components = sourceColumns.mid(3).join(' ');

                    aptSource->source = sourceColumns.mid(1).join(' ');

                    aptSourceList.append(aptSource);
                }
            }
        }
    }

    return aptSourceList;
}


