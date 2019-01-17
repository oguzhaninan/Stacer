#include "system_cleaner_md_dialog.h"
#include "ui_system_cleaner_md_dialog.h"
#include "system_cleaner_mediadir.h"
#include "app.h"
#include <QMetaType>

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
    else {
        //
        // BOTH OUR WIDGETS
        //
        auto *wid_dir = ui->listViewDirectories;
        auto *wid_fil = ui->listViewFiletypes;
        // row count
        int rc_dir = wid_dir->currentIndex().row();
        int rc_fil = wid_dir->currentIndex().row();

        for (int i = 0; i < scmd->mediaDirectories()->count(); i++)
        {
            wid_dir->insertItem(rc_dir, scmd->mediaDirectories()->keys()[i]);
            wid_fil->insertItems(rc_fil, scmd->mediaDirectories()->values()[i]);
        }
        QCoreApplication::postEvent(this, new QEvent(QEvent::ModifiedChange));
    }
}

void dialogMediaFiles::changeEvent(QEvent *event)
{
    QDialog::changeEvent(event);
    if (!(event->type() == QEvent::ModifiedChange))
        return;
    
    ui->listViewDirectories->update();
    ui->listViewFiletypes->update();
    this->update();

    QCoreApplication::postEvent(ui->listViewDirectories, new QEvent(QEvent::Show));
    QCoreApplication::postEvent(ui->listViewFiletypes, new QEvent(QEvent::Show));
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
    
    if (rc_fil >= 1)
    {
        for (const auto& s : data->filters())
        {
            auto list = wid_fil->findItems(s, Qt::MatchContains | Qt::MatchWrap | Qt::MatchRecursive);
            if (list.isEmpty())
            {
                    QListWidgetItem item = QListWidgetItem(s, wid_fil, QMetaType::QString);
                    item.setData(Qt::DisplayRole, QVariant(s));
                    wid_fil->insertItem(rc_fil, new QListWidgetItem(item));
            }
        }
    }
    else
    {
        wid_fil->insertItems(rc_fil, data->filters());
    }
    
    // event to signify we changed it
    QCoreApplication::postEvent(this, new QEvent(QEvent::ModifiedChange));
}
    
void dialogMediaFiles::init()
{
    connect(scmd, SIGNAL(addedMediaDirectory(QString*, QStringList*,const QString&)), this, SLOT(on_addMD(QString*, QStringList*, const QString&)));
    connect(scmd, SIGNAL(mediaDataRead(const MediaDirData*)), this, SLOT(on_readMD(const MediaDirData*)));
    connect(this, SIGNAL(addingNewDirectory(MediaDirData&)), scmd, SLOT(addDirByData(MediaDirData&)));
}

void dialogMediaFiles::on_btnAddDirectory_clicked()
{
    QString to_add_me = QFileDialog::getExistingDirectory(this, "Pick A DIrectory To Scan...", FileUtil::expandHomePath("~/Desktop"),
                                      QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly | QFileDialog::DontUseNativeDialog);

    if (to_add_me.length() < 1)
        return;

    QStringList     filters;
    
    auto list = ui->listViewFiletypes->findItems("*", Qt::MatchWrap | Qt::MatchRecursive | Qt::MatchWildcard);
    for (const auto& qli : list)
    {
        filters << qvariant_cast<QString>(qli->data(Qt::DisplayRole));
    }

    // lambda's are awesome
    MediaDirData dirdat = [&to_add_me,&filters](){MediaDirData ref = MediaDirData(); ref.set_data(to_add_me, filters); return ref;}();
    emit addingNewDirectory(dirdat);

    // now add to gui so we see what happens
    auto *wid_dir = ui->listViewDirectories;
    int rc_dir = wid_dir->currentIndex().row();
    QListWidgetItem item = QListWidgetItem(to_add_me, wid_dir, QMetaType::QString);
    item.setData(Qt::DisplayRole, QVariant(to_add_me));
    wid_dir->insertItem(rc_dir, new QListWidgetItem(item));
    
    QCoreApplication::postEvent(this, new QEvent(QEvent::ModifiedChange));
}

void dialogMediaFiles::on_btnAddFiletypes_clicked()
{
    bool clicked_okay = false;
    auto res = QInputDialog::getText(this, "Input new filter by file extension:", "Enter extension, e.g.: \'*.ext\'", QLineEdit::Normal, "*.ext", &clicked_okay);

    if (!clicked_okay)
        return;

    QStringList qsl = scmd->mediaDirectories()->keys();
    for (auto a = qsl.begin(); a != qsl.end(); ++a)
    {
        scmd->addFilterToDirectory(*a, res, false);
    }

    auto *wid_fil = ui->listViewFiletypes;
    int rc_fil = wid_fil->currentIndex().row();
    QListWidgetItem item = QListWidgetItem(res, wid_fil, QMetaType::QString);
    item.setData(Qt::DisplayRole, QVariant(res));
    wid_fil->insertItem(rc_fil, new QListWidgetItem(item));
}

void dialogMediaFiles::on_btnRemoveDirectory_clicked()
{
    auto *wid_dir = ui->listViewDirectories;
    bool changed = false;

    if (wid_dir->selectedItems().empty())
    {
        return;
    }
    else
    {
        for (auto *a : wid_dir->selectedItems())
        {
            for (auto& b : scmd->mediaDirectories()->keys())
            {
                if (b == a->text() || b == qvariant_cast<QString>(a->data(Qt::DisplayRole)))
                {
                    scmd->removeDirectory(b);
                }
            }
            wid_dir->removeItemWidget(a);
            changed = true;
        }
    }

    if (changed)
    {
        wid_dir->repaint(wid_dir->geometry());
        QCoreApplication::postEvent(this, new QEvent(QEvent::ModifiedChange));
        QCoreApplication::postEvent(wid_dir, new QEvent(QEvent::ModifiedChange));
    }
}

void dialogMediaFiles::on_btnRemoveFiletypes_clicked()
{
    auto *wid_fil = ui->listViewFiletypes;
    bool changed = false;

    if (wid_fil->selectedItems().empty())
    {
        return;
    }
    else
    {
        for (auto *a : wid_fil->selectedItems())
        {
            for (auto& b : scmd->mediaDirectories()->values())
            {
                if (b.contains(a->text(), Qt::CaseInsensitive) ||
                     b.contains(qvariant_cast<QString>(a->data(Qt::DisplayRole)), Qt::CaseInsensitive))
                {
                    b.removeAll(a->text());
                    b.removeAll(qvariant_cast<QString>(a->data(Qt::DisplayRole)));
                }
            }
            wid_fil->removeItemWidget(a);
            changed = true;
        }
    }

    if (changed)
    {
        wid_fil->repaint(wid_fil->geometry());
        QCoreApplication::postEvent(this, new QEvent(QEvent::ModifiedChange));
        QCoreApplication::postEvent(wid_fil, new QEvent(QEvent::ModifiedChange));
    }
}

/************************************
 * FACTORY below
 * **********************************/

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

    if (presets.size() != 0)
    {
        for (auto a = presets.begin(); a != presets.end(); ++a)
        {
            auto *b = *a;
            if (b->mediaDirs() == nullptr)
                b->setMediaDirs(&dlg->scmd);
            dlg->list_presets.append(b);
        }
    }

    AppManager::ins()->updateStylesheet();
    
    dlg->init();

    return dlg;
}
