#include "startup_app_edit.h"
#include "ui_startup_app_edit.h"

StartupAppEdit::~StartupAppEdit()
{
    delete ui;
}

QString StartupAppEdit::selectedFilePath = "";

StartupAppEdit::StartupAppEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartupAppEdit)
{
    ui->setupUi(this);

    init();
}

void StartupAppEdit::init()
{
    setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            size(),
            qApp->desktop()->availableGeometry())
    );

    ui->errorMsg->hide();

    setStyleSheet(AppManager::ins()->getStylesheetFileContent());

    newAppTemplate = "[Desktop Entry]\n"
                     "Name=%1\n"
                     "Comment=%2\n"
                     "Exec=%3\n"
                     "Type=Application\n"
                     "Terminal=false\n"
                     "X-GNOME-Autostart-enabled=true";
}

void StartupAppEdit::show()
{
    // clear fields
    ui->appNameTxt->clear();
    ui->appCommentTxt->clear();
    ui->appCommandTxt->clear();
    ui->errorMsg->hide();

    if(! selectedFilePath.isEmpty())
    {
        QStringList lines = FileUtil::readListFromFile(selectedFilePath);

        if(! lines.isEmpty())
        {
#define getValue(r) lines.filter(r).first().split("=").last().trimmed()
            ui->appNameTxt->setText(getValue(NAME_REG));
            ui->appCommentTxt->setText(getValue(COMMENT_REG));
            ui->appCommandTxt->setText(getValue(EXEC_REG));
#undef getValue
        }
    }

    QDialog::show();
}

void StartupAppEdit::on_saveBtn_clicked()
{
    static QString autostartPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/autostart";

    if(isValid())
    {
        if(! selectedFilePath.isEmpty())
        {
            QStringList lines = FileUtil::readListFromFile(selectedFilePath);

            lines.replace(lines.indexOf(NAME_REG), QString("Name=%1").arg(ui->appNameTxt->text()));
            lines.replace(lines.indexOf(COMMENT_REG), QString("Comment=%1").arg(ui->appCommentTxt->text()));
            lines.replace(lines.indexOf(EXEC_REG), QString("Exec=%1").arg(ui->appCommandTxt->text()));

            FileUtil::writeFile(selectedFilePath, QString(lines.join("\n")), QIODevice::ReadWrite | QIODevice::Truncate);
        }
        else
        {
            // new file content
            QString appContent = newAppTemplate
                    .arg(ui->appNameTxt->text())
                    .arg(ui->appCommentTxt->text())
                    .arg(ui->appCommandTxt->text());

            // file name
            QString appFileName = ui->appNameTxt->text()
                    .replace(" ", "_")
                    .replace(QRegExp("\\W+"), "");

            QString path = QString("%1/%2.desktop").arg(autostartPath).arg(appFileName);

            FileUtil::writeFile(path, appContent);
        }

        close();
        closeWindow(); // signal
    }
    else {
        ui->errorMsg->show();
    }

    selectedFilePath = "";
}

bool StartupAppEdit::isValid()
{
    return ! ui->appNameTxt->text().isEmpty() &&
           ! ui->appCommentTxt->text().isEmpty() &&
           ! ui->appCommandTxt->text().isEmpty();
}
