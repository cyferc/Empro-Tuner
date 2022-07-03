#pragma once

#include <QDialog>
#include <QTableWidget>
#include "pointlist.h"

namespace Ui {
class DialogPlotChannelChoose;
}

class DialogPlotChannelChoose : public QDialog
{
    Q_OBJECT

public:
    explicit DialogPlotChannelChoose(QList<PointList*> listOfPointLists, QWidget *pParent);
    ~DialogPlotChannelChoose();
    QList<int> getSelectedIndices();

private slots:
    void on_btnSelectAll_clicked();

    void on_btnDeselectAll_clicked();

private:
    Ui::DialogPlotChannelChoose *m_pUi;
    int m_SelectedIndex;
    QTableWidget *m_pTable;
};
