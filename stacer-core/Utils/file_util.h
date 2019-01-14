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
  static QStringList directoryList(const QString &path, const bool include_dir);
  static QStringList countFilesPerType(const QString &path, QString *opt_exten, long long *out_count, const bool include_dir=false);
  static QStringList getDesktopFiles(long long *out_totals = nullptr, const bool include_dir=false);
  static quint64 getFileSize(const QString &path);
  static QString expandHomePath(const QString &tilde_path);

private:
  FileUtil();
};

#endif // FILEUTIL_H
