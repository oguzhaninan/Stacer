#include <QWidgetAction>
#include "processes_page.h"
#include "ui_processes_page.h"
#include "utilities.h"
#include "limit_process_widget.h"

ProcessesPage::~ProcessesPage()
{
    delete ui;
}

ProcessesPage::ProcessesPage(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::ProcessesPage),
  mItemModel(new QStandardItemModel(this)),
  mSortFilterModel(new QSortFilterProxyModel(this)),
  im(InfoManager::ins()),
  mTimer(new QTimer(this))
{
    ui->setupUi(this);

    init();
}

void ProcessesPage::init()
{    
    mHeaders = QStringList {
        "PID", tr("Resident Memory"), tr("%Memory"), tr("Virtual Memory"),
        tr("User"), "%CPU", tr("Start Time"), tr("State"), tr("Group"),
        tr("Nice"), tr("CPU Time"), tr("Session"), tr("Process")
    };

    // slider settings
    ui->sliderRefresh->setRange(1, 10);
    ui->sliderRefresh->setPageStep(1);
    ui->sliderRefresh->setSingleStep(1);

    // Table settings
    mSortFilterModel->setSourceModel(mItemModel);

    mItemModel->setHorizontalHeaderLabels(mHeaders);

    ui->tableProcess->setModel(mSortFilterModel);
    mSortFilterModel->setSortRole(1);
    mSortFilterModel->setDynamicSortFilter(true);
    mSortFilterModel->sort(5, Qt::SortOrder::DescendingOrder);

    ui->tableProcess->horizontalHeader()->setSectionsMovable(true);
    ui->tableProcess->horizontalHeader()->setFixedHeight(36);
    ui->tableProcess->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->tableProcess->horizontalHeader()->setCursor(Qt::PointingHandCursor);

    ui->tableProcess->horizontalHeader()->resizeSection(0, 70);

    loadProcesses();

    connect(mTimer, &QTimer::timeout, this, &ProcessesPage::loadProcesses);
    mTimer->setInterval(1000);
    mTimer->start();

    ui->tableProcess->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->tableProcess->horizontalHeader(), SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(on_tableProcess_customContextMenuRequested(const QPoint&)));

    loadHeaderMenu();

    Utilities::addDropShadow(ui->btnEndProcess, 60);
    Utilities::addDropShadow(ui->tableProcess, 55);
}

void ProcessesPage::loadHeaderMenu()
{
    int i = 0;
    for (const QString &header : mHeaders) {
        QAction *action = new QAction(header);
        action->setCheckable(true);
        action->setChecked(true);
        action->setData(i++);

        mHeaderMenu.addAction(action);
    }

// exclude headers
#define ex(n) mHeaders.indexOf(n)
    QList<int> hiddenHeaders = { ex("Start Time"), ex("State"), ex("Group"),
                                 ex("Nice"), ex("CPU Time"), ex("Session"), ex("Virtual Memory") };
#undef ex

    QList<QAction*> actions = mHeaderMenu.actions();
    for (const int i : hiddenHeaders) {
        ui->tableProcess->horizontalHeader()->setSectionHidden(i, true);
        actions.at(i)->setChecked(false);
    }
}

void ProcessesPage::loadProcesses()
{
    QModelIndexList selecteds = ui->tableProcess->selectionModel()->selectedRows();

    mItemModel->removeRows(0, mItemModel->rowCount());

    im->updateProcesses();

    QList<Process> processes = im->getProcesses();
    QString username = im->getUserName();


    if (ui->checkAllProcesses->isChecked()) {
        for (const Process &proc : processes) {
            mItemModel->appendRow(createRow(proc));
        }
    } else  {
        for (const Process &proc : processes) {
            if (username == proc.getUname()) {
                mItemModel->appendRow(createRow(proc));
            }
        }
    }

    ui->lblProcessTitle->setText(tr("Processes (%1)").arg(mItemModel->rowCount()));

    // selected item
    if(! selecteds.isEmpty()) {
        mSeletedRowModel = selecteds.first();

        for (int i = 0; i < mSortFilterModel->rowCount(); ++i) {
            if (mSortFilterModel->index(i, 0).data(1).toInt() == mSeletedRowModel.data(1).toInt()) {
                ui->tableProcess->selectRow(i);
            }
        }
    } else {
        mSeletedRowModel = QModelIndex();
    }
}

QList<QStandardItem*> ProcessesPage::createRow(const Process &proc)
{
    QList<QStandardItem*> row;

    int data = 1;

    QStandardItem *pid_i = new QStandardItem(QString::number(proc.getPid()));
    pid_i->setData(proc.getPid(), data);
    pid_i->setData(proc.getPid(), Qt::ToolTipRole);

    QStandardItem *rss_i = new QStandardItem(FormatUtil::formatBytes(proc.getRss()));
    rss_i->setData(proc.getRss(), data);
    rss_i->setData(FormatUtil::formatBytes(proc.getRss()), Qt::ToolTipRole);

    QStandardItem *pmem_i = new QStandardItem(QString::number(proc.getPmem()));
    pmem_i->setData(proc.getPmem(), data);
    pmem_i->setData(proc.getPmem(), Qt::ToolTipRole);

    QStandardItem *vsize_i = new QStandardItem(FormatUtil::formatBytes(proc.getVsize()));
    vsize_i->setData(proc.getVsize(), data);
    vsize_i->setData(FormatUtil::formatBytes(proc.getVsize()), Qt::ToolTipRole);

    QStandardItem *uname_i = new QStandardItem(proc.getUname());
    uname_i->setData(proc.getUname(), data);
    uname_i->setData(proc.getUname(), Qt::ToolTipRole);

    QStandardItem *pcpu_i = new QStandardItem(QString::number(proc.getPcpu()));
    pcpu_i->setData(proc.getPcpu(), data);
    pcpu_i->setData(proc.getPcpu(), Qt::ToolTipRole);

    QStandardItem *starttime_i = new QStandardItem(proc.getStartTime());
    starttime_i->setData(proc.getStartTime(), data);
    starttime_i->setData(proc.getStartTime(), Qt::ToolTipRole);

    QStandardItem *state_i = new QStandardItem(proc.getState());
    state_i->setData(proc.getState(), data);
    state_i->setData(proc.getState(), Qt::ToolTipRole);

    QStandardItem *group_i = new QStandardItem(proc.getGroup());
    group_i->setData(proc.getGroup(), data);
    group_i->setData(proc.getGroup(), Qt::ToolTipRole);

    QStandardItem *nice_i = new QStandardItem(QString::number(proc.getNice()));
    nice_i->setData(proc.getNice(), data);
    nice_i->setData(proc.getNice(), Qt::ToolTipRole);

    QStandardItem *cpuTime_i = new QStandardItem(proc.getCpuTime());
    cpuTime_i->setData(proc.getCpuTime(), data);
    cpuTime_i->setData(proc.getCpuTime(), Qt::ToolTipRole);

    QStandardItem *session_i = new QStandardItem(proc.getSession());
    session_i->setData(proc.getSession(), data);
    session_i->setData(proc.getSession(), Qt::ToolTipRole);

    QStandardItem *cmd_i = new QStandardItem(proc.getCmd());
    cmd_i->setData(proc.getCmd(), data);
    cmd_i->setData(QString("<p>%1</p>").arg(proc.getCmd()), Qt::ToolTipRole);

    row << pid_i << rss_i << pmem_i << vsize_i << uname_i << pcpu_i
        << starttime_i << state_i << group_i << nice_i << cpuTime_i
        << session_i << cmd_i;

    return row;
}

void ProcessesPage::on_txtProcessSearch_textChanged(const QString &val)
{
    QRegExp query(val, Qt::CaseInsensitive, QRegExp::Wildcard);

    mSortFilterModel->setFilterKeyColumn(mHeaders.count() - 1); // process name
    mSortFilterModel->setFilterRegExp(query);
}

void ProcessesPage::on_sliderRefresh_valueChanged(const int &i)
{
    ui->lblRefresh->setText(tr("Refresh (%1)").arg(i));
    mTimer->setInterval(i * 1000);
}

/* Limit Process Button */
void ProcessesPage::on_btnLimitProcess_clicked() // ui file: line 205
{
    // Captures the PID for the highlighted process
    pid_t pid = mSeletedRowModel.data(1).toInt();

    QModelIndexList selected = ui->tableProcess->selectionModel()->selectedRows();

    if (ui->checkAllProcesses->isChecked())
    {
        QMessageBox messageBox;
        messageBox.setText(QString("Please select only one process to limit."));
        messageBox.setWindowTitle(QString("Invalid Selection"));
        messageBox.exec();
    }
    else if(!selected.isEmpty())
    {
        QPushButton *btn = ui->btnLimitProcess;

        QMenu *limitProcessMenu = new QMenu(this);

        LimitProcessWidget *ramLimit = new LimitProcessWidget("RAM Limit");
        LimitProcessWidget *cpuLimit = new LimitProcessWidget("CPU Limit");

        QAction *setRAMLimit = limitProcessMenu->addAction("RAM Limit");
        setRAMLimit->setProperty("widget", QVariant::fromValue<QWidget *>(ramLimit));

        QAction *setCPULimit = limitProcessMenu->addAction("CPU Limit");
        setCPULimit->setProperty("widget", QVariant::fromValue<QWidget *>(cpuLimit));

        QObject::connect(setRAMLimit, &QAction::triggered, [ramLimit, btn]() {
            ramLimit->show();
        });
        QObject::connect(setCPULimit, &QAction::triggered, [cpuLimit, btn]() {
            cpuLimit->show();
        });

        limitProcessMenu->exec(btn->mapToGlobal(QPoint(0, btn->height())));

        QMessageBox messageBox;
        messageBox.setText(QString("Continuing Execution"));
        messageBox.setWindowTitle(QString("Message"));
        messageBox.exec();

        /*
        if (limitProcessMenu->) {

            QMessageBox messageBox;
            messageBox.setText(QString("ram limit set"));
            messageBox.setWindowTitle(QString("Success"));
            messageBox.exec();

            ramLimit->close();
            cpuLimit->close();
            limitProcessMenu->close();
        }
        else if (cpuLimit->limitSet) {

            QMessageBox messageBox;
            messageBox.setText(QString("cpu limit set"));
            messageBox.setWindowTitle(QString("Success"));
            messageBox.exec();

            cpuLimit->close();
            ramLimit->close();
            limitProcessMenu->close();
        }
         */

    }
    else
    {
        QMessageBox messageBox;
        messageBox.setText(QString("Please select a process to limit"));
        messageBox.setWindowTitle(QString("Invalid Selection"));
        messageBox.exec();
    }

    // If pid contains a valid process ID from a selected process
    if (pid) {
        // Retrieve's the username from the table
        QString selectedUname = mSortFilterModel->index(mSeletedRowModel.row(), 4).data(1).toString();

        try {
            // Check if table username matches user logged into Linux
            if (selectedUname == im->getUserName()) {
                // Call exec as that user
                // CommandUtil::exec("kill", { QString::number(pid) });
            } else {
                // Otherwise execute the call as superuser
                // CommandUtil::sudoExec("kill", { QString::number(pid) });
            }
        } catch (QString &ex) {
            qCritical() << ex;
        }
    }
}

/* Remove Limit Button */
void ProcessesPage::on_btnRemoveLimit_clicked() // ui file: line 221
{
    // Captures the PID for the highlighted process
    pid_t pid = mSeletedRowModel.data(1).toInt();
    // If pid contains a valid process ID from a selected process
    if (pid) {
        // Retrieve's the username from the table
        QString selectedUname = mSortFilterModel->index(mSeletedRowModel.row(), 4).data(1).toString();

        try {
            // Check if table username matches user logged into Linux
            if (selectedUname == im->getUserName()) {
                // Call exec as that user
                CommandUtil::exec("kill", { QString::number(pid) });
            } else {
                // Otherwise execute the call as superuser
                CommandUtil::sudoExec("kill", { QString::number(pid) });
            }
        } catch (QString &ex) {
            qCritical() << ex;
        }
    }
}

/* End Process Button */
void ProcessesPage::on_btnEndProcess_clicked()
{
    // Captures the PID for the highlighted process
    pid_t pid = mSeletedRowModel.data(1).toInt();
    // If pid contains a valid process ID from a selected process
    if (pid) {
        // Retrieve's the username from the table
        QString selectedUname = mSortFilterModel->index(mSeletedRowModel.row(), 4).data(1).toString();

        try {
            // Check if table username matches user logged into Linux
            if (selectedUname == im->getUserName()) {
                // Call exec as that user
                CommandUtil::exec("kill", { QString::number(pid) });
            } else {
                // Otherwise execute the call as superuser
                CommandUtil::sudoExec("kill", { QString::number(pid) });
            }
        } catch (QString &ex) {
            qCritical() << ex;
        }
    }
}

void ProcessesPage::on_tableProcess_customContextMenuRequested(const QPoint &pos)
{
    QPoint globalPos = ui->tableProcess->mapToGlobal(pos);

    QAction *action = mHeaderMenu.exec(globalPos);

    if (action) {
        ui->tableProcess->horizontalHeader()->setSectionHidden(action->data().toInt(), ! action->isChecked());
    }
}
