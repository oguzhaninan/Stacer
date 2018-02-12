#include "processes_page.h"
#include "ui_processes_page.h"

ProcessesPage::~ProcessesPage()
{
    delete ui;
}

ProcessesPage::ProcessesPage(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::ProcessesPage),
  model(new QStandardItemModel(this)),
  sortModel(new QSortFilterProxyModel(this)),
  headers{"PID", tr("Resident Memory"), tr("%Memory"), tr("Virtual Memory"),
          tr("User"), "%CPU", tr("Start Time"), tr("State"), tr("Group"),
          tr("Nice"), tr("CPU Time"), tr("Session"), tr("Process")},
  im(InfoManager::ins()),
  timer(new QTimer(this))
{
    ui->setupUi(this);

    init();
}

void ProcessesPage::init()
{
    // slider settings
    ui->refreshSlider->setRange(1, 10);
    ui->refreshSlider->setPageStep(1);
    ui->refreshSlider->setSingleStep(1);

    // Table settings
    sortModel->setSourceModel(model);

    model->setHorizontalHeaderLabels(headers);

    ui->processTable->setModel(sortModel);    
    sortModel->setSortRole(1);
    sortModel->setDynamicSortFilter(true);
    sortModel->sort(5, Qt::SortOrder::DescendingOrder);

    ui->processTable->horizontalHeader()->setSectionsMovable(true);
    ui->processTable->horizontalHeader()->setFixedHeight(36);
    ui->processTable->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->processTable->horizontalHeader()->setCursor(Qt::PointingHandCursor);

    ui->processTable->horizontalHeader()->resizeSection(0, 70);

    loadProcesses();

    connect(timer, &QTimer::timeout, this, &ProcessesPage::loadProcesses);
    timer->setInterval(1000);
    timer->start();

    ui->processTable->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->processTable->horizontalHeader(), SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(on_processTable_customContextMenuRequested(const QPoint&)));

    loadHeaderMenu();
}

void ProcessesPage::loadHeaderMenu()
{
    int i = 0;
    for (const QString &kolon : headers) {
        QAction *action = new QAction(kolon);
        action->setCheckable(true);
        action->setChecked(true);
        action->setData(i++);

        headerMenu.addAction(action);
    }

// exclude headers
#define ex(n) headers.indexOf(n)
    QList<int> hiddenHeaders = { ex("Start Time"), ex("State"), ex("Group"),
                                 ex("Nice"), ex("CPU Time"), ex("Session") };
#undef ex

    QList<QAction*> actions = headerMenu.actions();
    for (const int i : hiddenHeaders) {
        ui->processTable->horizontalHeader()->setSectionHidden(i, true);
        actions.at(i)->setChecked(false);
    }
}

void ProcessesPage::loadProcesses()
{
    QModelIndexList selecteds =  ui->processTable->selectionModel()->selectedRows();

    model->removeRows(0, model->rowCount());

    im->updateProcesses();

    QList<Process> processes = im->getProcesses();
    QString username = im->getUserName();

    for (const Process &proc : processes) {
        if (ui->allProcessesCheck->isChecked()) {
            model->appendRow(createRow(proc));
        }
        else {
            if (username == proc.getUname())
                model->appendRow(createRow(proc));
        }
    }

    ui->processTitleLbl->setText(tr("Processes (%1)").arg(model->rowCount()));

    // selected item
    if(! selecteds.isEmpty()) {
        seletedRowModel = selecteds.first();

        for (int i = 0; i < sortModel->rowCount(); ++i) {
            if (sortModel->index(i, 0).data(1).toInt() == seletedRowModel.data(1).toInt())
                ui->processTable->selectRow(i);
        }
    } else {
        seletedRowModel = QModelIndex();
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

void ProcessesPage::on_processSearchBox_textChanged(const QString &val)
{
    QRegExp query(val, Qt::CaseInsensitive, QRegExp::Wildcard);

    sortModel->setFilterKeyColumn(headers.count() - 1); // process name
    sortModel->setFilterRegExp(query);
}

void ProcessesPage::on_refreshSlider_valueChanged(const int &i)
{
    ui->refreshLabel->setText(tr("Refresh (%1)").arg(i));
    timer->setInterval(i * 1000);
}

void ProcessesPage::on_endProcessBtn_clicked()
{
    pid_t pid = seletedRowModel.data(1).toInt();

    QString selectedUname = sortModel->index(seletedRowModel.row(), 4).data(1).toString();

    try {
        if (pid) {
            if (selectedUname == im->getUserName())
                CommandUtil::exec("kill", { QString::number(pid) });
            else
                CommandUtil::sudoExec("kill", { QString::number(pid) });
        }
    } catch (QString &ex) {
        qCritical() << ex;
    }
}

void ProcessesPage::on_processTable_customContextMenuRequested(const QPoint &pos)
{
    QPoint globalPos = ui->processTable->mapToGlobal(pos);

    QAction *action = headerMenu.exec(globalPos);

    if (action)
        ui->processTable->horizontalHeader()->setSectionHidden(action->data().toInt(),
                                                               ! action->isChecked());
}
