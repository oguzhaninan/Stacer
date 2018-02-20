#ifndef PPA_REPOSITORY_ITEM_H
#define PPA_REPOSITORY_ITEM_H

#include <QWidget>

namespace Ui {
class PPARepositoryItem;
}

class PPARepositoryItem : public QWidget
{
    Q_OBJECT

public:
    explicit PPARepositoryItem(QString repositoryName,
                               QString repositoryComment,
                               QString repositoryFilePath,
                               QWidget *parent = 0);
    ~PPARepositoryItem();

private:
    void init();

private:
    Ui::PPARepositoryItem *ui;

    QString mName;
    QString mComment;
    QString mFilePath;
};

#endif // PPA_REPOSITORY_ITEM_H
