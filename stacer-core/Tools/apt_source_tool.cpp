#include "apt_source_tool.h"
#include "Utils/command_util.h"
#include "Utils/file_util.h"

#include <QRegularExpression>
#include <QDebug>

bool AptSourceTool::checkSourceRepository()
{
    QDir sourceList(APT_SOURCES_LIST_D_PATH);

    bool isExists = sourceList.exists();

    return isExists;
}

void AptSourceTool::removeAPTSource(const APTSourcePtr aptSource)
{
    changeSource(aptSource, "");
}

void AptSourceTool::addRepository(const QString &repository, const bool isSource)
{
    if (! repository.isEmpty()) {
        QStringList args = { "-y", repository };
        if (isSource) {
            args << "-s";
        }

        CommandUtil::sudoExec("add-apt-repository", args);
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
        if (newSource.isEmpty()) {
            sourceFileContent.removeAt(pos);
        } else {
            sourceFileContent.replace(pos, newSource);
        }

        QStringList args = { aptSource->filePath };

        QByteArray data = sourceFileContent.join('\n').append('\n').toUtf8();

        CommandUtil::sudoExec("tee", args, data);
    }
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

    QFileInfoList infoList = QDir(APT_SOURCES_LIST_D_PATH).entryInfoList({"*.list"}, QDir::Files, QDir::Time);
    infoList.append(QFileInfo(APT_SOURCES_LIST_PATH)); // sources.list

    // example "deb [arch=amd64] http://packages.microsoft.com/repos/vscode stable main"
    for (const QFileInfo &info : infoList) {

        QStringList fileContent = FileUtil::readListFromFile(info.absoluteFilePath()).filter(QRegularExpression("^\\s{0,}#{0,}\\s{0,}deb"));

        for (const QString &line : fileContent) {
            QString _line = line.trimmed();

            APTSourcePtr aptSource(new APTSource);
            aptSource->filePath = info.absoluteFilePath();

            aptSource->isActive = ! _line.startsWith(QChar('#'));

            _line.remove('#'); // remove comment

            // if has options
            QRegularExpression regexOption("(\\s[\\[]+.*[\\]]+)");
            QRegularExpressionMatch match;
            if (-1 != _line.indexOf(regexOption, 0, &match)) {
                if (match.capturedLength() > 0) {
                    aptSource->options = match.captured().trimmed();
                }
            }
            // remove options
            _line.remove(regexOption);

            QStringList sourceColumns = _line.trimmed().split(QRegularExpression("\\s+"));
            bool isBinary = sourceColumns.first() == "deb";
            bool isSource = sourceColumns.first() == "deb-src";

            if ((isBinary || isSource) && sourceColumns.count() > 2) {
                aptSource->isSource = isSource;
                aptSource->uri = sourceColumns.at(1);
                aptSource->distribution = sourceColumns.at(2);
                aptSource->components = sourceColumns.mid(3).join(' ');

                aptSource->source = line.trimmed().remove('#').trimmed();

                aptSourceList.append(aptSource);
            }
        }
    }

    return aptSourceList;
}


