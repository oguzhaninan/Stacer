#include <QDebug>
#include "system_cleaner_md_preset.h"

SystemCleanerMDPreset::SystemCleanerMDPreset(const QByteArray& jsondata)
    : QJsonDocument(QJsonDocument::fromJson(jsondata)), QSharedData(), pSelf(nullptr),
      pMediaDirs(nullptr)
{
    qDebug("\t\tLoaded JSON File.");
}

SystemCleanerMDPreset::SystemCleanerMDPreset(const SystemCleanerMDPreset& other)
    : QJsonDocument(other)
{
    pSelf = QSharedDataPointer<SystemCleanerMDPreset>(const_cast<SystemCleanerMDPreset*>(other.pSelf.data()));
    pMediaDirs = other.pMediaDirs;
}

SystemCleanerMDPreset::~SystemCleanerMDPreset()
{

}

SystemCleanerMDPreset* SystemCleanerMDPreset::loadPreset(const QString& infile)
{
    QFile input(infile);
    QByteArray json_data;

    if(!input.open(QIODevice::ReadOnly | QIODevice::Text))
        throw;

    while (!input.atEnd()) {
        QByteArray line = input.readLine();
        json_data += line;
    }

    qDebug(QString("Loading JSON: %1").arg(infile).toStdString().c_str());

    SystemCleanerMDPreset* obj = new SystemCleanerMDPreset(json_data);
    decltype(pSelf) self_ptr = QSharedDataPointer<SystemCleanerMDPreset>(obj);
    obj->pSelf.swap(self_ptr);

    return const_cast<SystemCleanerMDPreset*>(obj->pSelf.constData());
}

MediaDirData** SystemCleanerMDPreset::getData()
{
    QVariant            document = this->toVariant();
    QList<QVariant>     doculist;
    QStringList         directories;
    QStringList         filters;
    size_t              mdd_size  = 0;
    MediaDirData      **mdd_array = nullptr;

    if (this->mediaDirs() == nullptr)
        throw QException();

    if (document.canConvert(QMetaType::QVariantList))
    {
        doculist = document.toList();
        if (doculist.isEmpty())
            throw QException();
    }
    else
    {
        throw QException();
    }
    
    for (const auto& v : doculist)
    {
        QJsonValue val = QJsonValue::fromVariant(v);
        
        if(val.isArray() != true && val.isString() == true)
        {
            directories << val.toString();
        }
        else if (val.isArray())
        {
            while (!val.toArray().isEmpty())
            {
                filters << val.toArray().takeAt(val.toArray().count()-1).toString();
            }
        }
    }
    /** count number of directories **/
    if (directories.count() == 1)
    {
        mdd_size = directories.count() + 1;
    }
    else
    {
        mdd_size = directories.count();
    }
    mdd_array = new MediaDirData*[mdd_size];
    /** set filters per directory **/
    for (int i = 0; i < directories.count(); i++)
    {
        mdd_array[i] = new MediaDirData();
        mdd_array[i]->set_data(directories[i], filters);
    }
    
    return mdd_array;
}

void SystemCleanerMDPreset::setMediaDirs(SystemCleanerMediaDir **media_dirs)
{
    pMediaDirs = *media_dirs;
}

SystemCleanerMediaDir* SystemCleanerMDPreset::mediaDirs() const
{
    return pMediaDirs;
}
