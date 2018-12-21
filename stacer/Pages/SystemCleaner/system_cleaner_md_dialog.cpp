#include "system_cleaner_md_dialog.h"
#include "ui_system_cleaner_md_dialog.h"

dialogMediaFiles::dialogMediaFiles(QWidget *parent)
    : QDialog(parent, Qt::Dialog | Qt::Tool),
    ui(new Ui::dialogMediaFiles)
{
    ui->setupUi(this);
}

dialogMediaFiles::~dialogMediaFiles()
{
    delete ui;
}

dialogMediaFilesFactory* dialogMediaFilesFactory::mDMFF = nullptr;

dialogMediaFilesFactory::~dialogMediaFilesFactory()
{
    if (dialogMediaFilesFactory::mDMFF != nullptr)
    {
        delete dialogMediaFilesFactory::mDMFF;
        dialogMediaFilesFactory::mDMFF = nullptr;
    }
}

dialogMediaFiles* dialogMediaFilesFactory::createDialog(QWidget *parent)
{
    dialogMediaFilesFactory::create();

    dialogMediaFiles *dlg = new dialogMediaFiles(parent);

    return dlg;
}
