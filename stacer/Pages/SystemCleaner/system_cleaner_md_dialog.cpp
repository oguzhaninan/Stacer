#include "system_cleaner_md_dialog.h"
#include "ui_system_cleaner_md_dialog.h"
#include "app.h"

dialogMediaFiles::dialogMediaFiles(QWidget *parent)
    : QDialog(parent, Qt::Dialog | Qt::Tool),
    ui(new Ui::dialogMediaFiles),
    scmd(nullptr)
{
    ui->setupUi(this);
}

dialogMediaFiles::~dialogMediaFiles()
{
    delete ui;
}

void dialogMediaFiles::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);

    if (!list_presets.empty())
    {
        for (int i=0; i < list_presets.length(); i++)
        {
            auto** data = list_presets[i]->getData();
            const auto sz = list_presets[i]->sizeofData();
            scmd->addMDDs(data, sz);
            list_presets.removeFirst();
            SystemCleanerMDPreset::cleanupData(data, sz);
        }
    }
}

void dialogMediaFiles::changeEvent(QEvent *event)
{
    QDialog::changeEvent(event);
    if (!event->type() == QEvent::ModifiedChange)
        return;
}

void dialogMediaFiles::on_addMD(QString *dir, QStringList *filters, const QString& lastfilter)
{
    // only the dir was added, ignore & wait until we have filters AND directories
    if (filters == nullptr)
        return;
    
    //
    // BOTH OUR WIDGETS
    //
    auto *wid_dir = ui->listViewDirectories;
    auto *wid_fil = ui->listViewFiletypes;
    // row count
    int rc_dir = wid_dir->currentIndex().row();
    int rc_fil = wid_dir->currentIndex().row();
    
    //
    // DIRS
    //
    QString dir2ins(*dir);

    if (rc_dir >= 1)
    {
        auto list = wid_dir->findItems(dir2ins, Qt::MatchContains | Qt::MatchWrap | Qt::MatchRecursive);
        if (!list.isEmpty())
            return;
    }
    QListWidgetItem item = QListWidgetItem(dir2ins, wid_dir, QMetaType::QString);
    item.setData(Qt::DisplayRole, QVariant(dir2ins));
    wid_dir->insertItem(rc_dir, new QListWidgetItem(item));
    
    //
    // FILTERS
    //
    QStringList fil2ins(*filters);
    
    wid_fil->insertItems(rc_fil, fil2ins);      // add to bottom listbox
    
    //
    // event to signify we changed it
    QCoreApplication::postEvent(this, new QEvent(QEvent::ModifiedChange));
}

void dialogMediaFiles::on_readMD(const MediaDirData *data)
{
    // only use this for filters
    // ;_; awful spaghetti
    
    auto *wid_fil = ui->listViewFiletypes;
    int rc_fil = wid_fil->currentIndex().row();
    
    wid_fil->insertItems(rc_fil, data->filters());
    // event to signify we changed it
    QCoreApplication::postEvent(this, new QEvent(QEvent::ModifiedChange));
}
    
void dialogMediaFiles::init()
{
    connect(scmd, SIGNAL(addedMediaDirectory(QString*, QStringList*,const QString&)), this, SLOT(on_addMD(QString*, QStringList*, const QString&)));
    connect(scmd, SIGNAL(mediaDataRead(const MediaDirData*)), this, SLOT(on_readMD(const MediaDirData*)));
}

dialogMediaFilesFactory* dialogMediaFilesFactory::mDMFF = nullptr;

dialogMediaFilesFactory::~dialogMediaFilesFactory()
{
    if (dialogMediaFilesFactory::mDMFF != nullptr)
    {
        delete dialogMediaFilesFactory::mDMFF;
        dialogMediaFilesFactory::mDMFF = nullptr;
    }
}

dialogMediaFiles* dialogMediaFilesFactory::createDialog(QWidget *parent, SystemCleanerMediaDir *dirs, init_list_presets presets)
{
    dialogMediaFilesFactory::create();

    dialogMediaFiles *dlg = new dialogMediaFiles(parent);

    dlg->scmd = dirs;

    for (auto a = presets.begin(); a != presets.end(); ++a)
    {
        auto *b = *a;
        if (b->mediaDirs() == nullptr)
            b->setMediaDirs(&dlg->scmd);
        dlg->list_presets.append(b);
    }

    AppManager::ins()->updateStylesheet();
    
    dlg->init();

    return dlg;
}
