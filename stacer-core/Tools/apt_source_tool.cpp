#include "apt_source_tool.h"
#include "Utils/file_util.h"

bool AptSourceTool::checkSourceRepository()
{
    QDir sourceList(APT_SOURCES_LIST_D_PATH);

    bool isExists = sourceList.exists();

    return isExists;
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

            APTSourcePtr aptSource(new APTSource);
            aptSource->filePath = info.absoluteFilePath();

            if (! _line.isEmpty()) {

                if (_line.startsWith(QChar('#'))) { // is deactive
                    aptSource->isActive = false;
                    aptSource->source = _line.replace("#", "").trimmed();
                }

                if (aptSource->source.startsWith("deb")) { // binary
                    aptSource->isSource = false;
                }
                else if(aptSource->source.startsWith("deb-src")) { // source
                    aptSource->isSource = true;
                }

                aptSourceList.append(aptSource);
            }
        }
    }

    return aptSourceList;
}


