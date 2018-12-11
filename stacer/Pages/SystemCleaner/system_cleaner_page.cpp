#include "system_cleaner_page.h"
#include "ui_system_cleaner_page.h"

#include "Types/command.hpp"

/* hack to add a custom flag for the treeview items
 * * * i am not refactoring omg
 */
constexpr decltype(Qt::ItemIsEnabled) flags_notapath(void)
{
    return static_cast<Qt::ItemFlag>(512);
}

// screw u sigsegv
int SystemCleanerPage::mDestructed = 0;

SystemCleanerPage::~SystemCleanerPage()
{
    this->invalidateTree(ui->treeWidgetScanResult);

    delete ui;
}

SystemCleanerPage::SystemCleanerPage(QWidget *parent) :
    QWidget(parent),    
    ui(new Ui::SystemCleanerPage),
    im(InfoManager::ins()),
    tmr(ToolManager::ins()),
    mDefaultIcon(QIcon::fromTheme("application-x-executable")),
    mLoadingMovie(nullptr),
    mLoadingMovie_2(nullptr)
{
    ui->setupUi(this);

    init();

    ui->stackedWidget->setCurrentIndex(0);
}

void SystemCleanerPage::init()
{
    // treview settings
    ui->treeWidgetScanResult->setColumnCount(2);
    ui->treeWidgetScanResult->setColumnWidth(0, 600);

    ui->treeWidgetScanResult->header()->setFixedHeight(30);
    ui->treeWidgetScanResult->setHeaderLabels({ tr("File Name"), tr("Size") });

    // loaders
    connect(SignalMapper::ins(), &SignalMapper::sigChangedAppTheme, [=] {
        QString themeName = SettingManager::ins()->getThemeName();

        mLoadingMovie = new QMovie(QString(":/static/themes/%1/img/scanLoading.gif").arg(themeName));
        ui->lblLoadingScanner->setMovie(mLoadingMovie);
        mLoadingMovie->start();
        ui->lblLoadingScanner->hide();

        mLoadingMovie_2 = new QMovie(QString(":/static/themes/%1/img/loading.gif").arg(themeName));
        ui->lblLoadingCleaner->setMovie(mLoadingMovie_2);
        mLoadingMovie_2->start();
        ui->lblLoadingCleaner->hide();
    });

    // memory management :o
    connect(this, SIGNAL(treeInvalidated(QTreeWidget*)), this, SLOT(invalidateTree(QTreeWidget*)));
    // more mm >:o
    connect(this, &SystemCleanerPage::destroyed, this, &SystemCleanerPage::when_destroyed);
}

void SystemCleanerPage::addTreeRoot(const CleanCategories &cat, const QString &title, const QFileInfoList &infos, bool noChild)
{
    QTreeWidgetItem *root = new QTreeWidgetItem(ui->treeWidgetScanResult);
    root->setData(2, 0, cat);
    root->setData(2, 1, title);
    if (! infos.isEmpty())
        root->setData(3, 0, infos.at(0).absoluteDir().path());
    root->setCheckState(0, Qt::Unchecked);

    // add children
    quint64 totalSize = 0;

    if(! noChild) {
        for (const QFileInfo &i : infos) {
            QString path = i.absoluteFilePath();
            quint64 size = FileUtil::getFileSize(path);

            addTreeChild(path, i.fileName(), size, root);

            totalSize += size;
        }

        root->setText(0, QString("%1 (%2)")
                      .arg(title)
                      .arg(infos.count()));

    } else {
        if (! infos.isEmpty())
            totalSize += FileUtil::getFileSize(infos.first().absoluteFilePath());

        root->setText(0, QString("%1")
                      .arg(title));
    }

    root->setText(1, QString("%1").arg(FormatUtil::formatBytes(totalSize)));
}

void SystemCleanerPage::addCallbackRoot(const QString &title, typeCallback callback, QTreeWidgetItem **out)
{
    QVariant variant;
    const CleanCategories cc = SystemCleanerPage::BROKEN_APPLICATIONS;
    QTreeWidgetItem *root    = new QTreeWidgetItem(ui->treeWidgetScanResult);
    scpCallback     cb;

    cb.pointer = &callback;
    cb.callback = callback;
    variant.setValue(cb);
    root->setData(2, 0, cc);
    root->setData(2, 1, title);
    root->setData(3, 0, variant);
    root->setCheckState(0, Qt::Unchecked);
    root->setFlags(root->flags() | flags_notapath());

    root->setText(0,QString("%1").arg(title));
    
    *out = root;
}

void SystemCleanerPage::addTreeChild(const QString &data, const QString &text, const quint64 &size, QTreeWidgetItem *parent)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setIcon(0, QIcon::fromTheme(text, mDefaultIcon));
    item->setText(0, text);
    item->setText(1, FormatUtil::formatBytes(size));
    item->setData(2, 0, data);
    item->setCheckState(0, Qt::Unchecked);
}

void SystemCleanerPage::addTreeChild(const CleanCategories &cat, const QString &text, const quint64 &size)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidgetScanResult);
    item->setText(0, text);
    item->setText(1, FormatUtil::formatBytes(size));
    item->setData(2, 0, cat);
    item->setCheckState(0, Qt::Unchecked);
}

void SystemCleanerPage::on_treeWidgetScanResult_itemClicked(QTreeWidgetItem *item, const int &column)
{
    if(column == 0) {
      // new check state
      Qt::CheckState cs = (item->checkState(column) == Qt::Checked ? Qt::Unchecked : Qt::Checked);

      // update check state
      item->setCheckState(column, cs);

      // change check state if has children
      for (int i = 0; i < item->childCount(); ++i)
        item->child(i)->setCheckState(column, cs);
    }
}

void SystemCleanerPage::systemScan()
{

    if (ui->checkPackageCache->isChecked() ||
        ui->checkCrashReports->isChecked() ||
        ui->checkAppLog->isChecked()       ||
        ui->checkAppCache->isChecked()     ||
        ui->checkTrash->isChecked()
    ){
        ui->btnScan->hide();
        ui->lblLoadingScanner->show();
        ui->checkPackageCache->setEnabled(false);
        ui->checkCrashReports->setEnabled(false);
        ui->checkAppLog->setEnabled(false);
        ui->checkAppCache->setEnabled(false);
        ui->checkTrash->setEnabled(false);

        ui->treeWidgetScanResult->clear();

        // Package Caches
        if (ui->checkPackageCache->isChecked()) {
            addTreeRoot(PACKAGE_CACHE,
                        ui->lblPackageCache->text(),
                        tmr->getPackageCaches());
        }

        // Crash Reports
        if (ui->checkCrashReports->isChecked()) {
            addTreeRoot(CRASH_REPORTS,
                        ui->lblCrashReports->text(),
                        im->getCrashReports());
        }

        // Application Logs
        if (ui->checkAppLog->isChecked()) {
            addTreeRoot(APPLICATION_LOGS,
                        ui->lblAppLog->text(),
                        im->getAppLogs());
        }

        // Application Cache
        if (ui->checkAppCache->isChecked()) {
            addTreeRoot(APPLICATION_CACHES,
                        ui->lblAppCache->text(),
                        im->getAppCaches());
        }

        // Trash
        if(ui->checkTrash->isChecked()) {
            addTreeRoot(TRASH,
                        ui->lblTrash->text(),
                        { QFileInfo("/home/oguzhan/.local/share/Trash/") },
                        true);
        }

        // Broken Applications
        if(ui->checkBrokenApps->isChecked()) {
            //oops
            QTreeWidgetItem *badhack = nullptr;
            
            addCallbackRoot(ui->checkBrokenApps->text(),[](QTreeWidgetItem *item, bool is_scan){
                qint64 all{}, broken{};
                QStringList ls_all = FileUtil::getDesktopFiles(&all, true);
                QStringList *ls_brk = new QStringList();
                
                using namespace Types::Applications;
                broken = listBrokenApps(ls_all, ls_brk);
                all    = ls_all.length();
                
                if (is_scan)
                {
                    item->setText(0, QString("%1 (%2 / %3)").arg(item->text(0)).arg(broken)
                                     .arg(all));
                    return;
                }
                else // // // CALLED FROM SYSTEM CLEAN
                {
                    if (ls_brk->isEmpty())
                    {
                        delete ls_brk;
                        return;
                    }
                    
                    // we must backup 1st
                    QString path_bu = QStandardPaths::locate(QStandardPaths::HomeLocation,".local",
                                                             QStandardPaths::LocateDirectory);
                    // prepare mkdir
                    using namespace Types;
                    PosixCmd com_1(QString("mkdir -p %1/backup/").arg(path_bu), false);
                    path_bu += "/backup";

                    //mkdir
                    com_1.runCommand();

                    //list of broken desktop files, we will backup & then rm
                    QStringList brokestuff(*ls_brk);
                    
                    for (const auto& str : brokestuff)
                    {
                        using namespace Types;
                        PosixCmd com_2(QString("cp -R --parents %1 %2%3").arg(str).arg(path_bu).arg(str), false);
                        com_2.runCommand();
                    }
                    /*
                     * Now we can delete the bull!!!! files...
                     */
                    CommandUtil::sudoExec("rm", QStringList() << "-rf" << brokestuff);
                }
                
                delete ls_brk;
            }, &badhack);
            
            scpCallback cb = badhack->data(3,0).value<scpCallback>();
            if (cb.is_callback())
            {
                decltype(cb.callback) cbp = cb.callback;
                decltype(cbp) cbd = reinterpret_cast<decltype(cbp)>(0x0);
                    
                if (!(cbp == cbd)) {
                    cbp(badhack,true);
                }
            }
        }

        // scan results page
        ui->stackedWidget->setCurrentIndex(1);

        ui->checkPackageCache->setChecked(false);
        ui->checkCrashReports->setChecked(false);
        ui->checkAppLog->setChecked(false);
        ui->checkAppCache->setChecked(false);
        ui->checkTrash->setChecked(false);
    }
}

bool SystemCleanerPage::cleanValid()
{
    for (int i = 0; i < ui->treeWidgetScanResult->topLevelItemCount(); ++i) {

        QTreeWidgetItem *it = ui->treeWidgetScanResult->topLevelItem(i);

        if (it->checkState(0) == Qt::Checked)
            return true;

        for (int j = 0; j < it->childCount(); ++j)
            if (it->child(j)->checkState(0) == Qt::Checked)
                return true;
    }

    return false;
}

void SystemCleanerPage::systemClean()
{
    if (cleanValid()) {
        ui->btnClean->hide();
        ui->lblLoadingCleaner->show();
        ui->treeWidgetScanResult->setEnabled(false);

        quint64 totalCleanedSize = 0;

        QTreeWidget *tree = ui->treeWidgetScanResult;

        QStringList filesToDelete;

        QList<QTreeWidgetItem *> children;

        for (int i = 0; i < tree->topLevelItemCount(); ++i) {

            QTreeWidgetItem *it = tree->topLevelItem(i);
            
            if (it->flags() & flags_notapath())
            {
                scpCallback cb = it->data(3,0).value<scpCallback>();

                if (cb.is_callback())
                {
                    decltype(cb.callback) cbp = cb.callback;
                    decltype(cbp) cbd = reinterpret_cast<decltype(cbp)>(0x0);
                    
                    if (!(cbp == cbd)) {
                        cbp(it,false);
                    }
                }
                continue;
            }

            CleanCategories cat = (CleanCategories) it->data(2, 0).toInt();

            // Package Caches | Crash Reports | Application Logs | Application Caches
            if (cat != CleanCategories::TRASH) {

                for (int j = 0; j < it->childCount(); ++j) { // files

                    if(it->child(j)->checkState(0) == Qt::Checked) { // if checked

                        QString filePath = it->child(j)->data(2, 0).toString();

                        filesToDelete << filePath;

                        children.append(it->child(j));
                    }
                }
            }

            // Trash
            else if (cat == CleanCategories::TRASH) {

                if (it->checkState(0) == Qt::Checked) {

                    QString trashPath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation).append("/.local/share/Trash");

                    QDir(trashPath + "/files").removeRecursively();
                    QDir(trashPath + "/info").removeRecursively();
                }
            }
        }

        // get removed files total size
        for (const QString &file : filesToDelete) {
            totalCleanedSize += FileUtil::getFileSize(file);
        }

        // remove selected files
        if(! filesToDelete.isEmpty()) {
            CommandUtil::sudoExec("rm", QStringList() << "-rf" << filesToDelete);
        }

        for (int i = 0; i < tree->topLevelItemCount(); ++i) {
            // clear removed childs
            for (QTreeWidgetItem *item : children) {
                tree->topLevelItem(i)->removeChild(item);
            }
        }

        // update titles
        for (int i = 0; i < tree->topLevelItemCount(); ++i) {

            QTreeWidgetItem *it = tree->topLevelItem(i);
            
            if (it->flags() & flags_notapath()) // ITEM NOT TO BE HANDLED BY LOOP
                continue;
            
            it->setText(0, QString("%1 (%2)")
                        .arg(it->data(2, 1).toString())
                        .arg(it->childCount()));

            it->setText(1, QString("%1")
                        .arg(FormatUtil::formatBytes(FileUtil::getFileSize(it->data(3, 0).toString()))));
        }

        ui->lblRemovedTotalSize->setText(tr("%1 size files cleaned.")
                                         .arg(FormatUtil::formatBytes(totalCleanedSize)));

        ui->btnClean->show();
        ui->lblLoadingCleaner->hide();
        ui->treeWidgetScanResult->setEnabled(true);
    }
}

void SystemCleanerPage::on_btnScan_clicked()
{
    QtConcurrent::run(this, &SystemCleanerPage::systemScan);
}

void SystemCleanerPage::on_btnClean_clicked()
{
    QtConcurrent::run(this, &SystemCleanerPage::systemClean);
}

void SystemCleanerPage::on_btnBackToCategories_clicked()
{
    ui->btnScan->show();
    ui->lblRemovedTotalSize->clear();
    ui->lblLoadingScanner->hide();
    ui->checkPackageCache->setEnabled(true);
    ui->checkCrashReports->setEnabled(true);
    ui->checkAppLog->setEnabled(true);
    ui->checkAppCache->setEnabled(true);
    ui->checkTrash->setEnabled(true);
    ui->treeWidgetScanResult->clear();
    ui->stackedWidget->setCurrentIndex(0);
    ui->checkSelectAllSystemScan->setChecked(false);

    emit treeInvalidated(ui->treeWidgetScanResult);
}

void SystemCleanerPage::on_checkSelectAllSystemScan_clicked(bool checked)
{
    ui->checkAppCache->setChecked(checked);
    ui->checkAppLog->setChecked(checked); 
    ui->checkCrashReports->setChecked(checked);
    ui->checkPackageCache->setChecked(checked);
    ui->checkTrash->setChecked(checked);
    
    if (ui->checkBrokenApps->isChecked() & ui->checkSelectAllSystemScan->isChecked() != checked) {
        ui->checkBrokenApps->setChecked(!checked);
    } else {
        ui->checkBrokenApps->setChecked(checked);
    }

}

void SystemCleanerPage::invalidateTree(QTreeWidget *tree)
{
    auto items = tree->findItems(QString("*"), Qt::MatchWrap | Qt::MatchWildcard | Qt::MatchRecursive);
    QTreeView *view = qobject_cast<QTreeView*>(tree);

    if (!ui->treeWidgetScanResult->children().empty() || !items.empty())
    {
        for (auto a = items.begin(); a != items.end(); ++a)
        {
            auto* c = *a;
            c = q_check_ptr<QTreeWidgetItem>(c);
            /** double-check pointer we're about to free **/
            quintptr addr = reinterpret_cast<quintptr>(c);
            addr = addr & 0xFFFFFFFFFFFFFF00; /*
                                               * the 0x55/0x54 is not from here
                                               */
            if (c != nullptr && addr != 0)
            {
                delete c;
            }
        }
	view->reset();
    }
}

void SystemCleanerPage::when_destroyed()
{
    int *deaths = &mDestructed;
    mDestructed = ++(*deaths);
}

void SystemCleanerPage::on_checkBrokenApps_stateChanged(int state)
{
    const Qt::CheckState cs = static_cast<Qt::CheckState>(state);

    if (cs == Qt::Checked)
        return;

    if (ui->checkBrokenApps->property("unclicked_yet").value<bool>() == true)
    {
        QVariant falsevar;
        falsevar.setValue(false);

        ui->checkBrokenApps->setProperty("unclicked_yet", falsevar);
        ui->checkBrokenApps->style()->unpolish(ui->checkBrokenApps);
        ui->checkBrokenApps->style()->polish(ui->checkBrokenApps);
    }
}

void SystemCleanerPage::on_checkBrokenApps_clicked()
{
    bool checky = !ui->checkBrokenApps->isChecked();

    if (ui->checkBrokenApps->property("unclicked_yet").value<bool>() == true)
    {
        QVariant falsevar;
        falsevar.setValue(false);

        ui->checkBrokenApps->setProperty("unclicked_yet", falsevar);
        ui->checkBrokenApps->style()->unpolish(ui->checkBrokenApps);
        ui->checkBrokenApps->style()->polish(ui->checkBrokenApps);
    }

    if (!checky)
    {
        ui->checkBrokenApps->setChecked(true);
    }
    else
    {
        ui->checkBrokenApps->setChecked(false);
    }
}

