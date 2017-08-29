#ifndef FILEUTIL_H
#define FILEUTIL_H

#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QDirIterator>
#include <QStandardPaths>

#include <QStandardPaths>
#include <QSharedPointer>

#include "stacer-core_global.h"

class STACERCORESHARED_EXPORT FileUtil
{
public:
  static QString readStringFromFile(QString path, QIODevice::OpenMode mode = QIODevice::ReadOnly);
  static QStringList readListFromFile(QString path, QIODevice::OpenMode mode = QIODevice::ReadOnly);

  static bool writeFile(QString path, QString content, QIODevice::OpenMode mode = QIODevice::ReadWrite);
  static QStringList directoryList(QString path);
  static quint64 getFileSize(QString path);

private:
  FileUtil();
};

#endif // FILEUTIL_H
