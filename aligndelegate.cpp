#include "aligndelegate.h"


void AlignDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    if (index.column() == m_alignColumn) {
        QStyleOptionViewItem alignedOption = option;
        alignedOption.displayAlignment = m_alignment;
        QStyledItemDelegate::paint(painter, alignedOption, index);
    } else {
        QStyledItemDelegate::paint(painter, option, index);
    }
}
