#ifndef SYSTEM_CLEANER_MD_DIALOG_H
#define SYSTEM_CLEANER_MD_DIALOG_H

#include <initializer_list>
#include <QList>
#include <QDialog>
#include "system_cleaner_mediadir.h"
#include "system_cleaner_md_preset.h"

namespace Ui
{
    class dialogMediaFiles;
}

typedef std::initializer_list<SystemCleanerMDPreset*> init_list_presets;

class dialogMediaFiles : public QDialog
{
    Q_OBJECT

public:
    explicit dialogMediaFiles(QWidget *parent = 0);
    virtual ~dialogMediaFiles();

protected:
    virtual void showEvent(QShowEvent *event);
    virtual void changeEvent(QEvent *event);

public slots:
    void on_addMD(QString *dir, QStringList *filters, const QString& lastfilter);
    void on_readMD(const MediaDirData *data);

private slots:
    void init();

private:
    Ui::dialogMediaFiles    *ui;

    SystemCleanerMediaDir   *scmd;
    
    QList<SystemCleanerMDPreset*>   list_presets;

    friend class dialogMediaFilesFactory;
};

class dialogMediaFilesFactory
{
public:
    virtual ~dialogMediaFilesFactory();

    static dialogMediaFiles* createDialog(QWidget *parent, SystemCleanerMediaDir *dirs, init_list_presets presets);
protected:
    dialogMediaFilesFactory() = default;
    dialogMediaFilesFactory(const dialogMediaFilesFactory& other) = delete;

    static void create() {if(mDMFF==nullptr){mDMFF=new dialogMediaFilesFactory();}}
private:
    static dialogMediaFilesFactory *mDMFF;
};

#endif
