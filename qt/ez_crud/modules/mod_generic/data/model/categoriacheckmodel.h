#ifndef CRUD_CATEGORIACHECKMODEL_H
#define CRUD_CATEGORIACHECKMODEL_H

#include <QAbstractTableModel>

#include <global/ezcrud_global.h>

namespace crud {

class Categoria;
class CategoriaItem;
class Persona;
class PersonaCategoria;

const int ColumnCount = 2;

class EZCRUD_EXPORT CategoriaCheckModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(QVariant readOnly
               READ readOnly
               WRITE setReadOnly)
public:
    // Enum Declaration
    enum KCategoria{
        KId = 0,
        KNombre = 1
    };
    explicit CategoriaCheckModel(QObject *parent = 0);
    ~CategoriaCheckModel();
    // Model Methods
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QModelIndex index(int row, int column, const QModelIndex &parent=QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    CategoriaItem * itemForIndex(const QModelIndex &index) const;

    QVariant readOnly();
    void setReadOnly(QVariant readOnly);

    QList<PersonaCategoria *> getPersonaCategoriaList(QObject *parent=0);
    void setPersona(Persona *persona);
private:
    bool m_readOnly;
    CategoriaItem *m_rootItem;
    Persona *m_persona;

public slots:
    void selectAll();
    void selectNone();
};

} // namespace crud

#endif // CRUD_CATEGORIACHECKMODEL_H
