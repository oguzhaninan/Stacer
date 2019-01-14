#include <QDebug>
#include "system_cleaner_md_preset.h"

SystemCleanerMDPreset::SystemCleanerMDPreset(const QByteArray& jsondata)
    : QJsonDocument(QJsonDocument::fromJson(jsondata)), QSharedData(), pSelf(nullptr),
      pMediaDirs(nullptr),
      pSzDataArray(0)
{
    qDebug("\t\tLoaded JSON File.");
}

SystemCleanerMDPreset::SystemCleanerMDPreset(const SystemCleanerMDPreset& other)
    : QJsonDocument(other)
{
    pSelf = QSharedDataPointer<SystemCleanerMDPreset>(const_cast<SystemCleanerMDPreset*>(other.pSelf.data()));
    pMediaDirs = other.pMediaDirs;
    pSzDataArray = other.pSzDataArray;
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
    QVariantMap         documap;
    QStringList         directories;
    QStringList         filters;
    size_t              mdd_size  = 0;
    MediaDirData      **mdd_array = nullptr;

    if (this->mediaDirs() == nullptr)
        throw QException();

    if (document.canConvert(QMetaType::QVariantList))       // array base element
    {
        doculist = document.toList();
        if (doculist.isEmpty())
            throw QException();
    }
    else if (document.canConvert(QMetaType::QVariantMap))   // object base element
    {
        documap  = document.toMap();
        if (documap.empty())
            throw QException();
        
        if (!documap["defaults"].canConvert(QMetaType::QVariantList))
            throw QException();
        
        doculist = documap["defaults"].toList();
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
            for (int j = 0; j < val.toArray().count(); j++)
            {
                filters << val.toArray().takeAt(j).toString();
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
        if (!directories[i].contains('~'))
        {
            mdd_array[i]->set_data(directories[i], filters);
        }
        else
        {
            QString expanded = FileUtil::expandHomePath(directories[i]);
            mdd_array[i]->set_data(expanded,
                                   filters);
        }
    }
    /** setup the pad MDD if it's supposed 2 exist **/
    if (mdd_size > directories.count())
    {
        mdd_array[mdd_size-1] = new MediaDirData();
    }
    
    pSzDataArray = mdd_size;
    return mdd_array;
}

void SystemCleanerMDPreset::cleanupData(MediaDirData **datas, const size_t num_datas)
{
    for (int i = 0; i < num_datas; i++)
    {
        delete datas[i];
    }
    
    delete[] datas;
}

void SystemCleanerMDPreset::setMediaDirs(SystemCleanerMediaDir **media_dirs)
{
    pMediaDirs = *media_dirs;
}

SystemCleanerMediaDir* SystemCleanerMDPreset::mediaDirs() const
{
    return pMediaDirs;
}

const size_t SystemCleanerMDPreset::sizeofData() const
{
    return pSzDataArray;
}

