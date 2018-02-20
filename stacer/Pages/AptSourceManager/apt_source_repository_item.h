#ifndef APTSourceRepositoryItem_H
#define APTSourceRepositoryItem_H

#include <QWidget>

namespace Ui {
class APTSourceRepositoryItem;
}

class APTSourceRepositoryItem : public QWidget
{
    Q_OBJECT

public:
    explicit APTSourceRepositoryItem(QString repositoryName,
                               QString repositoryFilePath,
                               QWidget *parent = 0);
    ~APTSourceRepositoryItem();

private slots:
    void on_deleteAptSourceBtn_clicked();

signals:
    void aptSourceRepositoryDeleted();

private:
    void init();

private:
    Ui::APTSourceRepositoryItem *ui;

    QString mName;
    QString mFilePath;
};

#endif
