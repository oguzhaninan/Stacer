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
    
    if (rc_dir > 1)
    {
        if (wid_dir->item(rc_dir)->data(Qt::DisplayRole).toString() == dir2ins ||
               wid_dir->item(--rc_dir)->data(Qt::DisplayRole).toString() == dir2ins)
        {
            return;
        }
    }
    
    wid_dir->insertItem(rc_dir, dir2ins);       // add to top listbox
    //
    // FILTER
    //
    QStringList fil2ins(*filters);
    
    if (rc_fil > 1)
    {
        for (int i=0; i < rc_fil; i++)
        {
            if (fil2ins.contains(wid_fil->item(i)->data(Qt::DisplayRole).toString()) ||
                    fil2ins.contains(lastfilter) || 
                    lastfilter == wid_fil->item(i)->data(Qt::DisplayRole).toString())
            {
                auto *a = wid_fil->takeItem(i);
                delete a;
            }
        }
    }
    
    wid_fil->insertItems(rc_fil, fil2ins);      // add to bottom listbox
}

void dialogMediaFiles::init()
{
    connect(scmd, SIGNAL(addedMediaDirectory(QString*, QStringList*,const QString&)), this, SLOT(on_addMD(QString*, QStringList*, const QString&)));
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
