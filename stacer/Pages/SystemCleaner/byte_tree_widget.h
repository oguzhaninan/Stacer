#ifndef BYTE_TREE_WIDGET_H
#define BYTE_TREE_WIDGET_H

#include <QTreeWidgetItem>
#include <QWidget>

#include <Utils/format_util.h>

class ByteTreeWidget : public QTreeWidgetItem
{
public:
    ByteTreeWidget(QTreeWidget* parent) : QTreeWidgetItem(parent) {}
    ByteTreeWidget(QTreeWidgetItem* parent) : QTreeWidgetItem(parent) {}

    void setValues(const QString &text, const quint64 &size, const QVariant &data);

    virtual bool operator<(const QTreeWidgetItem &other) const;
};

#endif // BYTE_TREE_WIDGET_H
