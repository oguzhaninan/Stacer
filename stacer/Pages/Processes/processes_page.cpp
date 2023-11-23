#include <QWidgetAction>
#include <iostream>
#include "processes_page.h"
#include "ui_processes_page.h"
#include "utilities.h"
#include "limit_process_widget.h"

// Static variables for process limiting
static pid_t staticPID = 0;
static QString staticUIDSelected = "";
static QString staticUID = "";
static std::unordered_map<pid_t, std::string> cpuLimits;

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
    // Finds the value in the process table selected by the user
    QModelIndexList selected = ui->tableProcess->selectionModel()->selectedRows();

    if (ui->checkAllProcesses->isChecked()) // If all processes is checked
    {
        // No limit can be set.
        QMessageBox messageBox;
        messageBox.setText(QString("Please select only one process to limit."));
        messageBox.setWindowTitle(QString("Invalid Selection"));
        messageBox.exec();
    }
    else if(!selected.isEmpty()) // If the selected process is not empty
    {
        /*
         * Had to use a pointer to the button in this case because
         * referencing it with the arrow operator makes most of the
         * following functions view it as two arguments.
         */
        QPushButton *btn = ui->btnLimitProcess;

        // Create the menu that is displayed when the button is clicked
        QMenu *limitProcessMenu = new QMenu(this);

        // Create two new limit process widgets
        LimitProcessWidget *ramLimit = new LimitProcessWidget("RAM Limit: Bytes");
        LimitProcessWidget *cpuLimit = new LimitProcessWidget("CPU Limit: %");

        // Create a menu action to associate with RAM Limit
        QAction *setRAMLimit = limitProcessMenu->addAction("RAM Limit");
        setRAMLimit->setProperty("widget", QVariant::fromValue<QWidget *>(ramLimit));
        // Create a menu action to associate with CPU Limit
        QAction *setCPULimit = limitProcessMenu->addAction("CPU Limit");
        setCPULimit->setProperty("widget", QVariant::fromValue<QWidget *>(cpuLimit));

        // Connect RAM Limit action to Limit Process button and show it when button is clicked
        QObject::connect(setRAMLimit, &QAction::triggered, [ramLimit, btn]() {
            ramLimit->show();
        });
        // Connect CPU Limit action to Limit Process button and show it when button is clicked
        QObject::connect(setCPULimit, &QAction::triggered, [cpuLimit, btn]() {
            cpuLimit->show();
        });

        // Place the menu at the button when it is clicked.
        limitProcessMenu->exec(btn->mapToGlobal(QPoint(0, btn->height())));

        /*
         * These static variables allow the onLimitProcessConfirm function to be called
         * from the LimitProcessWidget object.
         */
        staticPID = mSeletedRowModel.data(1).toInt(); // The selected PID
        staticUIDSelected = mSortFilterModel->index(mSeletedRowModel.row(), 4).data(1).toString(); // The selected User ID
        staticUID = im->getUserName(); // The logged-in User ID

    }
    else // If no process is selected, inform the user
    {
        QMessageBox messageBox;
        messageBox.setText(QString("Please select a process to limit"));
        messageBox.setWindowTitle(QString("Invalid Selection"));
        messageBox.exec();
    }

}

void ProcessesPage::onLimitProcessConfirm(int limitValue, QString currentOptionName) {

    // If pid contains a valid process ID from a selected process
    if (staticPID) {

        try {

            // Convert integers to strings
            std::string processIDStr = std::to_string(staticPID);
            std::string limitValueStr = std::to_string(limitValue);
            if (currentOptionName == "RAM Limit: Bytes") {
                // Construct the prlimit command
                std::string command = "pkexec prlimit --pid " + processIDStr + " --rss=" + limitValueStr;

                // Convert the command string to a const char* for system function argument
                const char *command_cstr = command.c_str();

                // Execute the prlimit command
                int result = std::system(command_cstr);

                // Check if the command executed successfully
                if (result == 0) {
                    std::cout << "Resource limit set successfully!" << std::endl;
                } else {
                    std::cerr << "Failed to set resource limit!" << std::endl;
                }
            }
            else if (currentOptionName == "CPU Limit: %") {
                // Install cpulimit if not already installed
                std::string command = "pkexec apt-get install cpulimit";

                // Convert the command string to a const char* for system function argument
                const char *command_cstr = command.c_str();

                // Execute the cpulimit install command
                int result0 = std::system(command_cstr);

                // Construct the cpulimit command
                std::string command2 = "cpulimit -p " + processIDStr + " -l " + limitValueStr + " -b";

                // Convert the command string to a const char* for system function argument
                const char *command_cstr2 = command2.c_str();

                // Execute the cpulimit command
                int result = std::system(command_cstr2);

                // Check if the command executed successfully
                if (result == 0) {
                    std::cout << "Resource limit set successfully!" << std::endl;
                    cpuLimits[staticPID] = command2;
                } else {
                    std::cerr << "Failed to set resource limit!" << std::endl;
                }
            }
        } catch (QString &ex) {
            qCritical() << ex;
        }
    }
}

/* Remove Limit Button */
void ProcessesPage::on_btnRemoveLimit_clicked() // ui file: line 221
{
    // Finds the value in the process table selected by the user
    QModelIndexList selected = ui->tableProcess->selectionModel()->selectedRows();

    if (ui->checkAllProcesses->isChecked()) // If all processes is checked
    {
        // No limit can be set.
        QMessageBox messageBox;
        messageBox.setText(QString("Please select only one process to limit."));
        messageBox.setWindowTitle(QString("Invalid Selection"));
        messageBox.exec();
    }
    else if(!selected.isEmpty()) // If the selected process is not empty
    {
        // These static variables are recycled from the limit process functions
        staticPID = mSeletedRowModel.data(1).toInt(); // The selected PID
        staticUIDSelected = mSortFilterModel->index(mSeletedRowModel.row(), 4).data(1).toString(); // The selected User ID
        staticUID = im->getUserName(); // The logged-in User ID
        /*
         * Had to use a pointer to the button in this case because
         * referencing it with the arrow operator makes most of the
         * following functions view it as two arguments.
         */
        QPushButton *btn = ui->btnRemoveLimit;

        // Create the menu that is displayed when the button is clicked
        QMenu *removeLimitMenu = new QMenu(this);

        // Create a menu action to associate with Remove RAM Limit
        QAction *removeRAMLimit = removeLimitMenu->addAction("Remove RAM Limit");
        connect(removeRAMLimit, &QAction::triggered, [=]() {

            if (staticPID) {

                try {
                        // Convert integers to strings
                        std::string processIDStr = std::to_string(staticPID);

                        // Construct the prlimit command to remove limit
                        std::string command = "pkexec prlimit --pid " + processIDStr + " --rss=unlimited";

                        // Convert the command string to a const char* for system function argument
                        const char *command_cstr = command.c_str();

                        // Execute the prlimit command
                        int result = std::system(command_cstr);

                        // Check if the command executed successfully
                        if (result == 0) {
                            std::cout << "Resource limit removed successfully!" << std::endl;
                        } else {
                            std::cerr << "Failed to remove resource limit!" << std::endl;
                        }

                } catch (QString &ex) {
                    qCritical() << ex;
                }
            }
        });
        // Create a menu action to associate with Remove CPU Limit
        QAction *removeCPULimit = removeLimitMenu->addAction("Remove CPU Limit");
        connect(removeCPULimit, &QAction::triggered, [=]() {
            if (staticPID) {

                try {
                        // Construct the kill command for cpulimit
                        std::string command = "pkexec pkill -f \"" + cpuLimits[staticPID] + "\"";

                        // Convert the command string to a const char* for system function argument
                        const char *command_cstr = command.c_str();

                        // Execute the kill command
                        int result = std::system(command_cstr);
                        // Check if the command executed successfully
                        if (result == 15) {
                            std::cout << "Resource limit removed successfully!" << std::endl;
                        } else {
                            std::cerr << "Failed to remove resource limit!" << std::endl;
                        }

                } catch (QString &ex) {
                    qCritical() << ex;
                }
            }
        });

        // Place the menu at the button when it is clicked.
        removeLimitMenu->exec(btn->mapToGlobal(QPoint(0, btn->height())));

    }
    else // If no process is selected, inform the user
    {
        QMessageBox messageBox;
        messageBox.setText(QString("Please select a process to limit"));
        messageBox.setWindowTitle(QString("Invalid Selection"));
        messageBox.exec();
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
