#ifndef ROSQLTABLEMODEL_H
#define ROSQLTABLEMODEL_H

#include <QSqlTableModel>
#include <QColor>
#include <QBrush>

class ROTableModel: public QSqlTableModel
{
    Q_OBJECT
public:
    explicit ROTableModel(QObject *parent=nullptr, QSqlDatabase db = QSqlDatabase()): QSqlTableModel(parent, db)
    {

    }

    QVariant data(const QModelIndex &idx, int role = Qt::DisplayRole) const override
    {
        if(!idx.isValid())
            return QVariant();

        if(role == Qt::BackgroundRole)
        {
            if (data(this->index(idx.row(), 23)).toString().isEmpty())
                return QVariant(QBrush (QColor(Qt::white)));
            return QVariant(QBrush (QColor("#ebdcff")));
        }

        return QSqlTableModel::data(idx, role);
    }

};

#endif // ROSQLTABLEMODEL_H
