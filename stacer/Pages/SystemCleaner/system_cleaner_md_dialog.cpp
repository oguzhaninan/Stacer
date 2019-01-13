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

void dialogMediaFiles::on_addMD(QString *dir, QStringList *filters)
{
    // only the dir was added, ignore & wait until we have filters AND directories
    if (filters == nullptr)
        return;
}

void dialogMediaFiles::init()
{
    connect(scmd, SIGNAL(addedMediaDirectory(QString*, QStringList*)), this, SLOT(on_addMD(QString*, QStringList*)));
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
