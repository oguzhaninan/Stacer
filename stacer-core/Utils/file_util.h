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
  static QString readStringFromFile(const QString &path, const QIODevice::OpenMode &mode = QIODevice::ReadOnly);
  static QStringList readListFromFile(const QString &path, const QIODevice::OpenMode &mode = QIODevice::ReadOnly);

  static bool writeFile(const QString &path, const QString &content, const QIODevice::OpenMode &mode = QIODevice::WriteOnly | QIODevice::Truncate);
  static QStringList directoryList(const QString &path);
  static quint64 getFileSize(const QString &path);

private:
  FileUtil();
};

#endif // FILEUTIL_H
