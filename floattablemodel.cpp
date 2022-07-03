#include "floattablemodel.h"
#include <QSize>
#include <QDebug>
#include "helpers.h"

FloatTableModel::FloatTableModel(QObject *pParent)
    : QAbstractTableModel(pParent),
      m_HeadersSelectable(SelectableBody),
      m_Max(0),
      m_Min(0),
      m_BackColorIntensity(150),
      m_BackColorDarkness(255)
{
    int initialTableSize = 16;

    // Values
    for (int i = 0; i < (initialTableSize + 1); i++)
    {
        // Add row
        QList<float> list;
        m_DataF.append(list);

        // Add column
        for (int j = 0; j < (initialTableSize + 1); j++)
        {
            m_DataF[i].append(0.0f);
        }
    }

    // Colors
    for (int i = 0; i < (initialTableSize + 1); i++)
    {
        // Add row
        QList<QColor> list;
        m_DataBackColor.append(list);

        // Add column
        for (int j = 0; j < (initialTableSize + 1); j++)
        {
            m_DataBackColor[i].append(QColor(m_BackColorIntensity, m_BackColorIntensity, m_BackColorDarkness)); // light blue
        }
    }

    // Display stings
    for (int i = 0; i < (initialTableSize + 1); i++)
    {
        // Add row
        QList<QString> list;
        m_DataString.append(list);

        // Add column
        for (int j = 0; j < (initialTableSize + 1); j++)
        {
            m_DataString[i].append(QString("0"));
        }
    }

    calculateMaxMinValues();
    setAxesColor();

    // Set bottom left string blank
    m_DataString[m_DataF.size() - 1][0] = "";
}

int FloatTableModel::getSelectableArea()
{
    return this->m_HeadersSelectable;
}

void FloatTableModel::setSelectableArea(Selectable selectable)
{
    this->m_HeadersSelectable = selectable;

    // refresh all cells
    for (int row = 0; row < rowCount(); row++)
    {
        for (int col = 0; col < columnCount(); col++)
        {
            emit dataChanged(index(row, col), index(row, col), QVector<int>() << Qt::EditRole);
        }
    }
}

int FloatTableModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
    {
        return 0;
    }

    return m_DataF.size();
}

int FloatTableModel::columnCount(const QModelIndex& parent) const
{
    if (parent.isValid())
    {
        return 0;
    }

    return m_DataF[0].size();
}

QVariant FloatTableModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    if (role == Qt::DisplayRole)
    {
        return m_DataString[index.row()][index.column()];
    }
    else if (role == Qt::EditRole)
    {
        return m_DataF[index.row()][index.column()];
    }
    else if (role == Qt::BackgroundRole)
    {
        return m_DataBackColor[index.row()][index.column()];
    }

    return QVariant::Invalid;
}

void FloatTableModel::setAxesColor()
{
    // Set axes color
    for (int row = 0; row < rowCount(); row++)
    {
        m_DataBackColor[row][0] = QColor(220,220,220);
    }

    for (int col = 0; col < columnCount(); col++)
    {
        m_DataBackColor[m_DataF.size() - 1][col] = QColor(220,220,220);
    }

    m_DataBackColor[m_DataF.size() - 1][0] = QColor(Qt::black);
}

void FloatTableModel::setDefaultColors()
{
    for (int row = 0; row < (rowCount() - 1); row++)
    {
        for (int col = 1; col < columnCount(); col++)
        {
            m_DataBackColor[row][col] = QColor(m_BackColorIntensity, m_BackColorIntensity, m_BackColorDarkness);
        }
    }
}

void FloatTableModel::calculateColors()
{
    calculateMaxMinValues();

    if (m_Min == m_Max)
    {
        setDefaultColors();
        return;
    }

    for (int row = 0; row < (m_DataF.size() - 1); row++)
    {
        for (int col = 1; col < columnCount(); col++)
        {
            int green;
            double currentVal = data(index(row, col), Qt::EditRole).toFloat();

            double difference = m_Max - m_Min;
            double half = m_Min + (difference / 2);

            int red = Helpers::Map(currentVal, half, m_Max, m_BackColorIntensity, m_BackColorDarkness);

            if (currentVal < half)
            {
                green = Helpers::Map(currentVal, m_Min, half, m_BackColorIntensity, m_BackColorDarkness);
            }
            else
            {
                green = Helpers::Map(currentVal, half, m_Max, m_BackColorDarkness, m_BackColorIntensity);
            }

            int blue = Helpers::Map(currentVal, m_Min, half, m_BackColorDarkness, m_BackColorIntensity);

            if (red < m_BackColorIntensity)
                red = m_BackColorIntensity;
            else if (red > m_BackColorDarkness)
                red = m_BackColorDarkness;

            if (green < m_BackColorIntensity)
                green = m_BackColorIntensity;
            else if (green > m_BackColorDarkness)
                green = m_BackColorDarkness;

            if (blue < m_BackColorIntensity)
                blue = m_BackColorIntensity;
            else if (blue > m_BackColorDarkness)
                blue = m_BackColorDarkness;

            //setData(index(row, col), QColor(red, green, blue), Qt::BackgroundRole);
            m_DataBackColor[row][col] = QColor(red, green, blue);
        }
    }
}

void FloatTableModel::calculateMaxMinValues()
{
    // Get max and min values
    m_Max = data(this->index(0, 1), Qt::EditRole).toFloat();
    m_Min = data(this->index(0, 1), Qt::EditRole).toFloat();

    for (int row = 0; row < (m_DataF.size() - 1); row++)
    {
        for (int col = 1; col < columnCount(); col++)
        {
            m_Max = qMax(data(this->index(row, col), Qt::EditRole).toDouble(), m_Max);
            m_Min = qMin(data(this->index(row, col), Qt::EditRole).toDouble(), m_Min);
        }
    }
}

bool FloatTableModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (data(index, role) != value)
    {
        if (role == Qt::BackgroundRole)
        {
            m_DataBackColor[index.row()][index.column()] = value.value<QColor>();
        }
        else
        {
            float valFloat = value.toFloat();
            m_DataF[index.row()][index.column()] = valFloat;

            // Bottom row, just Int's
            if (index.row() == (m_DataF.size() - 1))
            {
                m_DataString[index.row()][index.column()] = QString::number(valFloat, 'f', 0);
            }
            else
            {
                QString str = QString::number(m_DataF[index.row()][index.column()], 'f', 3);
                str.remove(QRegExp("0+$")); // Remove any number of trailing 0's
                str.remove(QRegExp("\\.$")); // If the last character is just a '.' then remove it

                m_DataString[index.row()][index.column()] = str;
            }
        }

        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }

    return false;
}

Qt::ItemFlags FloatTableModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
    {
        return Qt::NoItemFlags;
    }

    if ((index.row() == (m_DataF.size() - 1)) && (index.column() == 0))
    {
        return Qt::NoItemFlags;
    }

    if (this->m_HeadersSelectable == SelectableBody)
    {
        if ((index.row() < (m_DataF.size() - 1)) && (index.column() > 0))
        {
            return Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled;
        }
        else
        {
            return Qt::NoItemFlags;
        }
    }
    else if (this->m_HeadersSelectable == SelectableRowHeaders)
    {
        if (index.column() == 0)
        {
            return Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled;
        }
        else
        {
            return Qt::NoItemFlags;
        }
    }
    else // SelectableColumnHeaders
    {
        if (index.row() == (m_DataF.size() - 1))
        {
            return Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled;
        }
        else
        {
            return Qt::NoItemFlags;
        }
    }

    return Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

bool FloatTableModel::insertRows(int row, int count, const QModelIndex& parent)
{
    beginInsertRows(parent, row, row + count - 1);

    // Add row values
    QList<float> list;
    QList<QColor> listC;
    QList<QString> listS;
    for (int j = 0; j < m_DataF[0].size(); j++)
    {
        list.append(0.0f);
        listC.append(QColor(255,255,0));
        listS.append(QString("0"));
    }
    m_DataF.insert(row, list);
    m_DataBackColor.insert(row, listC);
    m_DataString.insert(row, listS);

    calculateColors();
    setAxesColor();

    endInsertRows();
    return true;
}

bool FloatTableModel::insertColumns(int column, int count, const QModelIndex& parent)
{
    beginInsertColumns(parent, column, column + count - 1);

    for (int j = 0; j < m_DataF.size(); j++)
    {
        m_DataF[j].insert(column, 0.0f);
        m_DataBackColor[j].insert(column, QColor(255,255,0));
        m_DataString[j].insert(column, QString("0"));
    }
    calculateColors();
    setAxesColor();

    endInsertColumns();
    return true;
}

bool FloatTableModel::removeRows(int row, int count, const QModelIndex& parent)
{
    beginRemoveRows(parent, row, row + count - 1);

    m_DataF.removeAt(row);
    m_DataBackColor.removeAt(row);
    m_DataString.removeAt(row);
    calculateColors();

    endRemoveRows();
    return true;
}

bool FloatTableModel::removeColumns(int column, int count, const QModelIndex& parent)
{
    beginRemoveColumns(parent, column, column + count - 1);

    for (int j = 0; j < m_DataF.size(); j++)
    {
        m_DataF[j].removeAt(column);
        m_DataBackColor[j].removeAt(column);
        m_DataString[j].removeAt(column);
    }
    calculateColors();

    endRemoveColumns();

    return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int FloatTableModel::actualRowCount()
{
    return rowCount() - 1;
}

int FloatTableModel::actualColumnCount()
{
    return columnCount() - 1;
}

float FloatTableModel::actualBodyValue(int row, int column)
{
    return data(index(row, column + 1), Qt::EditRole).toFloat();
}

float FloatTableModel::actualRowValue(int row)
{
    return data(index(row, 0), Qt::EditRole).toFloat();
}

float FloatTableModel::actualColumnValue(int column)
{
    return data(index(rowCount() - 1, column + 1), Qt::EditRole).toFloat();
}

void FloatTableModel::actualSetBodyValue(int row, int column, float value)
{
    setData(index(row, column + 1), value, Qt::EditRole);
}

void FloatTableModel::actualSetRowValue(int row, float value)
{
    setData(index(row, 0), value, Qt::EditRole);
}

void FloatTableModel::actualSetColumnValue(int column, float value)
{
    setData(index(rowCount() - 1, column + 1), value, Qt::EditRole);
}
