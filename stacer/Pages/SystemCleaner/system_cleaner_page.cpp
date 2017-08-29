#include "system_cleaner_page.h"
#include "ui_system_cleaner_page.h"

SystemCleanerPage::~SystemCleanerPage()
{
    delete ui;
    delete loadingMovie;
    delete loadingMovie_2;
}

SystemCleanerPage::SystemCleanerPage(QWidget *parent) :
    QWidget(parent),
    im(InfoManager::ins()),
    tmr(ToolManager::ins()),
    defaultIcon(QIcon::fromTheme("application-x-executable")),    
    ui(new Ui::SystemCleanerPage)
{
    ui->setupUi(this);

    init();

    ui->stackedWidget->setCurrentIndex(0);
}

void SystemCleanerPage::init()
{
    // treview settings
    ui->scanResultTreeW->setColumnCount(2);
    ui->scanResultTreeW->setColumnWidth(0, 600);

    ui->scanResultTreeW->header()->setFixedHeight(30);
    ui->scanResultTreeW->setHeaderLabels({ tr("File Name"), tr("Size") });

    // loaders
    connect(AppManager::ins(), &AppManager::changedTheme, this, [this]() {
        loadingMovie = new QMovie(QString(":/static/themes/%1/img/scanLoading.gif").arg(AppManager::ins()->getThemeName()));
        ui->loading->setMovie(loadingMovie);
        loadingMovie->start();
        ui->loading->hide();

        loadingMovie_2 = new QMovie(QString(":/static/themes/%1/img/loading.gif").arg(AppManager::ins()->getThemeName()));
        ui->loading_2->setMovie(loadingMovie_2);
        loadingMovie_2->start();
        ui->loading_2->hide();
    });
}

void SystemCleanerPage::addTreeRoot(CleanCategories cat, QString title, QFileInfoList infos, bool noChild)
{
    QTreeWidgetItem *root = new QTreeWidgetItem(ui->scanResultTreeW);
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

void SystemCleanerPage::addTreeChild(QString data, QString text, quint64 size, QTreeWidgetItem *parent)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setIcon(0, QIcon::fromTheme(text, defaultIcon));
    item->setText(0, text);
    item->setText(1, FormatUtil::formatBytes(size));
    item->setData(2, 0, data);
    item->setCheckState(0, Qt::Unchecked);
}

void SystemCleanerPage::addTreeChild(CleanCategories cat, QString text, quint64 size)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(ui->scanResultTreeW);
    item->setText(0, text);
    item->setText(1, FormatUtil::formatBytes(size));
    item->setData(2, 0, cat);
    item->setCheckState(0, Qt::Unchecked);
}

void SystemCleanerPage::on_scanResultTreeW_itemClicked(QTreeWidgetItem *item, int column)
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
    if (ui->packageCacheCheck->isChecked() ||
        ui->crashReportsCheck->isChecked() ||
        ui->logCheck->isChecked()          ||
        ui->appCacheCheck->isChecked()     ||
        ui->trashCheck->isChecked()
    ){
        ui->scanBtn->hide();
        ui->loading->show();
        ui->packageCacheCheck->setEnabled(false);
        ui->crashReportsCheck->setEnabled(false);
        ui->logCheck->setEnabled(false);
        ui->appCacheCheck->setEnabled(false);
        ui->trashCheck->setEnabled(false);

        QThread::sleep(1);

        ui->scanResultTreeW->clear();

        // Package Caches
        if (ui->packageCacheCheck->isChecked()) {
            addTreeRoot(PACKAGE_CACHE,
                        ui->packageCacheLabel->text(),
                        tmr->getPackageCaches());
        }

        // Crash Reports
        if (ui->crashReportsCheck->isChecked()) {
            addTreeRoot(CRASH_REPORTS,
                        ui->crashReportsLabel->text(),
                        im->getCrashReports());
        }

        // Application Logs
        if (ui->logCheck->isChecked()) {
            addTreeRoot(APPLICATION_LOGS,
                        ui->logLabel->text(),
                        im->getAppLogs());
        }

        // Application Cache
        if (ui->appCacheCheck->isChecked()) {
            addTreeRoot(APPLICATION_CACHES,
                        ui->appCacheLabel->text(),
                        im->getAppCaches());
        }

        // Trash
        if(ui->trashCheck->isChecked()) {
            addTreeRoot(TRASH,
                        ui->trashLabel->text(),
                        { QFileInfo("/home/oguzhan/.local/share/Trash/") },
                        true);
        }

        // scan results page
        ui->stackedWidget->setCurrentIndex(1);

        ui->packageCacheCheck->setChecked(false);
        ui->crashReportsCheck->setChecked(false);
        ui->logCheck->setChecked(false);
        ui->appCacheCheck->setChecked(false);
        ui->trashCheck->setChecked(false);
    }
}

bool SystemCleanerPage::cleanValid()
{
    for (int i = 0; i < ui->scanResultTreeW->topLevelItemCount(); ++i) {

        QTreeWidgetItem *it = ui->scanResultTreeW->topLevelItem(i);

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
    if (cleanValid())
    {
        ui->cleanBtn->hide();
        ui->loading_2->show();
        ui->scanResultTreeW->setEnabled(false);

        quint64 totalCleanedSize = 0;

        QTreeWidget *tree = ui->scanResultTreeW;

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

                    QString homePath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);

                    QDir(homePath + "/.local/share/Trash/files").removeRecursively();
                    QDir(homePath + "/.local/share/Trash/info").removeRecursively();
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

        QThread::sleep(1);

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

        ui->removedTotalSizeLbl->setText(tr("%1 size files cleaned.")
                                         .arg(FormatUtil::formatBytes(totalCleanedSize)));

        ui->cleanBtn->show();
        ui->loading_2->hide();
        ui->scanResultTreeW->setEnabled(true);
    }
}

void SystemCleanerPage::on_scanBtn_clicked()
{
    QtConcurrent::run(this, &SystemCleanerPage::systemScan);
}

void SystemCleanerPage::on_cleanBtn_clicked()
{
    QtConcurrent::run(this, &SystemCleanerPage::systemClean);
}

void SystemCleanerPage::on_backButtton_clicked()
{
    ui->scanBtn->show();
    ui->loading->hide();
    ui->packageCacheCheck->setEnabled(true);
    ui->crashReportsCheck->setEnabled(true);
    ui->logCheck->setEnabled(true);
    ui->appCacheCheck->setEnabled(true);
    ui->trashCheck->setEnabled(true);
    ui->scanResultTreeW->clear();
    ui->stackedWidget->setCurrentIndex(0);
}
