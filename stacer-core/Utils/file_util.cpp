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

QStringList FileUtil::directoryList(const QString &path, const bool include_dir)
{	
	QDir dir(path);
	QStringList list;

	if (!include_dir)
		return FileUtil::directoryList(path);
	
    for (const QFileInfo &info : dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files))
        list << info.absoluteFilePath();

    return list;
}

QStringList FileUtil::countFilesPerType(const QString &path, QString *opt_exten, long long *out_count, const bool include_dir)
{
	long long count = 0;
	
	QStringList unsorted_list = QStringList(FileUtil::directoryList(path,include_dir));
	QStringList the_list;
	
        for (const auto &str : unsorted_list)
        {
            if (opt_exten != nullptr && str.indexOf(*opt_exten) < 0)
                    continue;
			
            the_list << str;
        }

	if (out_count != nullptr)
	{
		*out_count = count;
	}
	
	return the_list;
}

QStringList FileUtil::getDesktopFiles(long long *out_totals, const bool include_dir)
{
	QStringList desktop_file_paths = QStandardPaths::standardLocations(QStandardPaths::StandardLocation::ApplicationsLocation);
	QStringList desktop_files;
	long long   total_desktop_files = 0;
	QString    *ddtop = new QString(".desktop");
	
	for (const auto &str : desktop_file_paths)
	{
		long long j;
		desktop_files << countFilesPerType(str, ddtop, &j, include_dir);
		total_desktop_files = total_desktop_files + j;
	}
	
	if (out_totals != nullptr)
	{
		*out_totals = total_desktop_files;
	}
	
	delete ddtop;
	return desktop_files;
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

QString FileUtil::expandHomePath(const QString &tilde_path)
{
    QString relapath(tilde_path);
    
    return std::move(QString("%1/%2").arg(QStandardPaths::writableLocation(QStandardPaths::HomeLocation))
                        .arg(relapath.remove('~')));
}
