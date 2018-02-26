#include "file_util.h"

FileUtil::FileUtil()
{

}

QString FileUtil::readStringFromFile(const QString &path, const QIODevice::OpenMode &mode)
{
    QSharedPointer<QFile> file(new QFile(path));

    QString data;

    if(file->open(mode)) {

      data = file->readAll();

      file->close();
    }

    return data;
}

QStringList FileUtil::readListFromFile(const QString &path, const QIODevice::OpenMode &mode)
{
    QStringList list = FileUtil::readStringFromFile(path, mode).trimmed().split("\n");

    return list;
}

bool FileUtil::writeFile(const QString &path, const QString &content, const QIODevice::OpenMode &mode)
{
    QFile file(path);

    if(file.open(mode))
    {
        QTextStream stream(&file);
        stream << content.toUtf8() << endl;

        file.close();

        return true;
    }

    return false;
}

QStringList FileUtil::directoryList(const QString &path)
{
    QDir dir(path);

    QStringList list;

    for (const QFileInfo &info : dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files))
        list << info.fileName();

    return list;
}

quint64 FileUtil::getFileSize(const QString &path)
{
    quint64 totalSize = 0;

    QFileInfo info(path);

    if (info.exists())
    {
        if (info.isFile()) {
            totalSize += info.size();
        }
        else if (info.isDir()) {

            QDir dir(path);

            for (const QFileInfo &i : dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs)) {
                totalSize += getFileSize(i.absoluteFilePath());
            }
        }
    }

    return totalSize;
}


