
#pragma once

#include <QDialog>
#include <QCloseEvent>
#include <QShowEvent>
#include <QTableView>
#include "floattablemodel.h"
#include "floattableview.h"

namespace Ui {
class DialogFuelTable;
}

class DialogFuelTable : public QDialog
{
    Q_OBJECT

public:
    explicit DialogFuelTable(QWidget *pParent, FloatTableModel *pTableModel);
    ~DialogFuelTable();

private slots:
    void closeEvent(QCloseEvent *pEvent);
    void showEvent(QShowEvent *pEvent);
    void on_buttonAddRow_clicked();
    void on_buttonRemoveRow_clicked();
    void on_buttonAddColumn_clicked();
    void on_buttonRemoveColumn_clicked();
    void on_pushButtonEditHeaders_clicked();

    void on_pushButtonInterpolate_clicked();

    void on_pushButtonIncrement_clicked();

    void on_pushButtonDecrement_clicked();

private:
    Ui::DialogFuelTable *m_pUi;
    int                 m_PosX;
    int                 m_PosY;
    bool                m_FirstShow;
    FloatTableModel     *m_pTableModel;
    FloatTableView      *m_pTableView;
};
