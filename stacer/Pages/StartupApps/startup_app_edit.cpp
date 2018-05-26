#include "startup_app_edit.h"
#include "ui_startup_app_edit.h"
#include "utilities.h"
#include <QDebug>
#include <QStyle>

StartupAppEdit::~StartupAppEdit()
{
    delete ui;
}

QString StartupAppEdit::selectedFilePath = "";

StartupAppEdit::StartupAppEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartupAppEdit),
    mNewAppTemplate("[Desktop Entry]\n"
                   "Name=%1\n"
                   "Comment=%2\n"
                   "Exec=%3\n"
                   "Type=Application\n"
                   "Terminal=false\n"
                   "Hidden=false\n")
{
    ui->setupUi(this);

    init();
}

void StartupAppEdit::init()
{
    setGeometry(
        QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter,
            size(), qApp->desktop()->availableGeometry())
    );

    mAutostartPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/autostart";

    ui->lblErrorMsg->hide();

    setStyleSheet(AppManager::ins()->getStylesheetFileContent());
}

void StartupAppEdit::show()
{
    // clear fields
    ui->txtStartupAppName->clear();
    ui->txtStartupAppComment->clear();
    ui->txtStartupAppCommand->clear();
    ui->lblErrorMsg->hide();

    if(! selectedFilePath.isEmpty())
    {
        QStringList lines = FileUtil::readListFromFile(selectedFilePath);

        if(! lines.isEmpty())
        {
            ui->txtStartupAppName->setText(Utilities::getDesktopValue(NAME_REG, lines));
            ui->txtStartupAppComment->setText(Utilities::getDesktopValue(COMMENT_REG, lines));
            ui->txtStartupAppCommand->setText(Utilities::getDesktopValue(EXEC_REG, lines));
        }
    }

    QDialog::show();
}

void StartupAppEdit::changeDesktopValue(QStringList &lines, const QRegExp &reg, const QString &text)
{
    int pos = lines.indexOf(reg);

    if (pos != -1) {
        lines.replace(pos, text);
    } else {
        lines.append(text);
    }
}

void StartupAppEdit::on_btnSave_clicked()
{
    if(isValid()) {
        if(! selectedFilePath.isEmpty()) {
            QStringList lines = FileUtil::readListFromFile(selectedFilePath);

            changeDesktopValue(lines, NAME_REG, QString("Name=%1").arg(ui->txtStartupAppName->text()));
            changeDesktopValue(lines, COMMENT_REG, QString("Comment=%1").arg(ui->txtStartupAppComment->text()));
            changeDesktopValue(lines, EXEC_REG, QString("Exec=%1").arg(ui->txtStartupAppCommand->text()));

            FileUtil::writeFile(selectedFilePath, lines.join("\n"), QIODevice::ReadWrite | QIODevice::Truncate);
        }
        else {
            // new file content
            QString appContent = mNewAppTemplate
                    .arg(ui->txtStartupAppName->text())
                    .arg(ui->txtStartupAppComment->text())
                    .arg(ui->txtStartupAppCommand->text());

            // file name
            QString appFileName = ui->txtStartupAppName->text()
                    .simplified()
                    .replace(' ', '-')
                    .toLower();

            qDebug() << appFileName;

            QString path = QString("%1/%2.desktop").arg(mAutostartPath).arg(appFileName);

            FileUtil::writeFile(path, appContent);
        }

        emit startupAppAdded(); // signal
        close();
    }
    else {
        ui->lblErrorMsg->show();
    }

    selectedFilePath = "";
}

bool StartupAppEdit::isValid()
{
    return ! ui->txtStartupAppName->text().isEmpty() &&
           ! ui->txtStartupAppComment->text().isEmpty() &&
           ! ui->txtStartupAppCommand->text().isEmpty();
}
