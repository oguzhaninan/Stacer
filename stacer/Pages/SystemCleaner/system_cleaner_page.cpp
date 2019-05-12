#include "system_cleaner_page.h"
#include "ui_system_cleaner_page.h"

SystemCleanerPage::~SystemCleanerPage()
{
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
      Qt::CheckState cs = (item->checkState(column) == Qt::Checked ? Qt::Checked : Qt::Unchecked);

      // update check state
      //item->setCheckState(column, cs);

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
                        { QFileInfo(QDir::homePath() + "/.local/share/Trash/") },
                        true);
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
}

void SystemCleanerPage::on_checkSelectAllSystemScan_clicked(bool checked)
{
    ui->checkAppCache->setChecked(checked);
    ui->checkAppLog->setChecked(checked);
    ui->checkCrashReports->setChecked(checked);
    ui->checkPackageCache->setChecked(checked);
    ui->checkTrash->setChecked(checked);
}

void SystemCleanerPage::on_checkSelectAll_clicked(bool checked)
{
    for (int i = 0; i < ui->treeWidgetScanResult->topLevelItemCount(); ++i)
    {
        QTreeWidgetItem *it = ui->treeWidgetScanResult->topLevelItem(i);
        it->setCheckState(0, (checked ? Qt::Checked : Qt::Unchecked));

        for (int j = 0; j < it->childCount(); ++j)
            it->child(j)->setCheckState(0, (checked ? Qt::Checked : Qt::Unchecked));
    }
}
