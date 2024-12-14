#ifndef ALIGNDELEGATE_H
#define ALIGNDELEGATE_H

#include <QStyledItemDelegate>

class AlignDelegate : public QStyledItemDelegate {
public:
    AlignDelegate(int alignColumn, Qt::Alignment alignment, QObject *parent = nullptr)
        : QStyledItemDelegate(parent), m_alignColumn(alignColumn), m_alignment(alignment) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    int m_alignColumn;
    Qt::Alignment m_alignment;
};

#endif
