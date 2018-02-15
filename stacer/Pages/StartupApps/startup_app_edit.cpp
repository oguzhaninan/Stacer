#include "startup_app_edit.h"
#include "ui_startup_app_edit.h"
#include "utilities.h"

StartupAppEdit::~StartupAppEdit()
{
    delete ui;
}

QString StartupAppEdit::selectedFilePath = "";

StartupAppEdit::StartupAppEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartupAppEdit),
    newAppTemplate("[Desktop Entry]\n"
                   "Name=%1\n"
                   "Comment=%2\n"
                   "Exec=%3\n"
                   "Type=Application\n"
                   "Terminal=false\n"
                   "Hidden=true")
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

    this->autostartPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/autostart";

    ui->errorMsg->hide();

    setStyleSheet(AppManager::ins()->getStylesheetFileContent());
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
            ui->appNameTxt->setText(Utilities::getDesktopValue(NAME_REG, lines));
            ui->appCommentTxt->setText(Utilities::getDesktopValue(COMMENT_REG, lines));
            ui->appCommandTxt->setText(Utilities::getDesktopValue(EXEC_REG, lines));
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

void StartupAppEdit::on_saveBtn_clicked()
{
    if(isValid())
    {
        if(! selectedFilePath.isEmpty())
        {
            QStringList lines = FileUtil::readListFromFile(selectedFilePath);

            changeDesktopValue(lines, NAME_REG, QString("Name=%1").arg(ui->appNameTxt->text()));
            changeDesktopValue(lines, COMMENT_REG, QString("Comment=%1").arg(ui->appCommentTxt->text()));
            changeDesktopValue(lines, EXEC_REG, QString("Exec=%1").arg(ui->appCommandTxt->text()));

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
