
#pragma once

#include <QAbstractTableModel>
#include <QList>
#include <QColor>

class FloatTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum Selectable {SelectableRowHeaders, SelectableColumnHeaders, SelectableBody};
    explicit FloatTableModel(QObject *pParent);

    void setSelectableArea(Selectable selectable);
    int getSelectableArea();

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    // Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

    void calculateMaxMinValues();
    void calculateColors();
    void setAxesColor();
    void setDefaultColors();

    int actualRowCount();
    int actualColumnCount();

    float actualBodyValue(int row, int column);
    float actualRowValue(int row);
    float actualColumnValue(int column);

    void actualSetBodyValue(int row, int column, float value);
    void actualSetRowValue(int row, float value);
    void actualSetColumnValue(int column, float value);

private:
    QList< QList<float> > m_DataF;
    QList< QList<QColor> > m_DataBackColor;
    QList< QList<QString> > m_DataString;
    Selectable m_HeadersSelectable;
    double m_Max;
    double m_Min;
    int m_BackColorIntensity; // 0 = full intensity
    int m_BackColorDarkness;  // 255 = not dark
};
