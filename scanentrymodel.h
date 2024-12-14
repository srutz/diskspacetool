#ifndef SCANENTRYMODEL_H
#define SCANENTRYMODEL_H

#include <QObject>
#include <QAbstractItemModel>
#include "applicationstate.h"
#include "util.h"

class ScanEntryModel : public QAbstractItemModel {
    Q_OBJECT

public:
    explicit ScanEntryModel(ScanEntry* rootEntry, QObject* parent = nullptr)
        : QAbstractItemModel(parent), root(rootEntry) {}

    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& child) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

private:
    ScanEntry* root;
};

#endif // SCANENTRYMODEL_H
