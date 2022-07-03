
#pragma once

#include <QDialog>
#include <QTableWidget>
#include "ecudata.h"

namespace Ui {
class DialogDatalogSettings;
}

class DialogDatalogSettings : public QDialog
{
    Q_OBJECT

public:
    explicit DialogDatalogSettings(QWidget *pParent, EcuData *pEcuData);
    ~DialogDatalogSettings();

private slots:
    void on_pushButtonSelectAll_clicked();
    void on_pushButtonDeselectAll_clicked();

private:
    Ui::DialogDatalogSettings *m_pUi;
    QTableWidget              *m_pTableChannels;
};
