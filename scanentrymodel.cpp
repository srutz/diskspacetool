#include "scanentrymodel.h"
#include <QLocale>

static QLocale systemLocale = QLocale::system();

QModelIndex ScanEntryModel::index(int row, int column, const QModelIndex& parent) const {
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    ScanEntry* parentEntry = parent.isValid() ? static_cast<ScanEntry*>(parent.internalPointer()) : root;
    shared_ptr<ScanEntry> childEntry = parentEntry->children.at(row);
    if (childEntry) {
        return createIndex(row, column, childEntry.get());
    }
    return QModelIndex();
}

QModelIndex ScanEntryModel::parent(const QModelIndex& child) const  {
    if (!child.isValid())
        return QModelIndex();

    ScanEntry* childEntry = static_cast<ScanEntry*>(child.internalPointer());
    ScanEntry* parentEntry = childEntry->parent;

    if (!parentEntry || parentEntry == root)
        return QModelIndex();

    ScanEntry* grandParentEntry = parentEntry->parent;
    int row = grandParentEntry ? Util::indexOf(grandParentEntry->children, parentEntry) : 0;
    return createIndex(row, 0, parentEntry);
}

int ScanEntryModel::rowCount(const QModelIndex& parent) const  {
    if (parent.column() > 0)
        return 0;

    ScanEntry* parentEntry = parent.isValid() ? static_cast<ScanEntry*>(parent.internalPointer()) : root;
    if (parentEntry == nullptr) {
        return 0;
    }
    return parentEntry->children.size();
}

int ScanEntryModel::columnCount(const QModelIndex& parent) const  {
    return 4; // Path, Size, File Count, Percent
}

QVariant ScanEntryModel::data(const QModelIndex& index, int role) const  {
    if (!index.isValid() || role != Qt::DisplayRole)
        return QVariant();

    ScanEntry* entry = static_cast<ScanEntry*>(index.internalPointer());
    switch (index.column()) {
    case 0: return Util::basename(entry->path);
    case 1: return systemLocale.toString(entry->size);
    case 2: return entry->fileCount;
    case 3: return QString("%1").arg(entry->percent * 100, 0, 'f', 3);;
    default: return QVariant();
    }
}

QVariant ScanEntryModel::headerData(int section, Qt::Orientation orientation, int role) const  {
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal) {
        return QVariant();
    }
    switch (section) {
    case 0: return "Path";
    case 1: return "Size";
    case 2: return "File Count";
    case 3: return "% of Scan";
    default: return QVariant();
    }
}
