#ifndef SYSTEM_CLEANER_MD_DIALOG_H
#define SYSTEM_CLEANER_MD_DIALOG_H

#include <QDialog>
#include "system_cleaner_mediadir.h"

namespace Ui
{
    class dialogMediaFiles;
}

class dialogMediaFiles : public QDialog
{
    Q_OBJECT

public:
    explicit dialogMediaFiles(QWidget *parent = 0);
    virtual ~dialogMediaFiles();

private:
    Ui::dialogMediaFiles    *ui;

    SystemCleanerMediaDir   *scmd;

    friend class dialogMediaFilesFactory;
};

class dialogMediaFilesFactory
{
public:
    virtual ~dialogMediaFilesFactory();

    static dialogMediaFiles* createDialog(QWidget *parent, SystemCleanerMediaDir *dirs);
protected:
    dialogMediaFilesFactory() = default;
    dialogMediaFilesFactory(const dialogMediaFilesFactory& other) = delete;

    static void create() {if(mDMFF==nullptr){mDMFF=new dialogMediaFilesFactory();}}
private:
    static dialogMediaFilesFactory *mDMFF;
};

#endif
