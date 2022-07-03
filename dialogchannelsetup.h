
#pragma once

#include <QDialog>
#include <QTableWidget>
#include <QLCDNumber>
#include "ecudata.h"

namespace Ui {
class DialogChannelSetup;
}

class DialogChannelSetup : public QDialog
{
    Q_OBJECT

public:
    explicit DialogChannelSetup(QWidget *pParent, EcuData *pEcuData, QLCDNumber *pNumberDisplay);
    ~DialogChannelSetup();

private slots:
    void slotTableSelectionChanged();

    void on_doubleSpinBoxMajorStepSize_valueChanged(double arg1);

    void on_doubleSpinBoxMinorSteps_valueChanged(double arg1);

    void on_doubleSpinBoxMax_valueChanged(double arg1);

    void on_doubleSpinBoxMin_valueChanged(double arg1);

    void on_spinBoxDecimalPlaces_valueChanged(int arg1);

    void on_doubleSpinBoxTestValue_valueChanged(double arg1);

private:
    Ui::DialogChannelSetup *m_pUi;
    QTableWidget           *m_pTableChannels;
    EcuData                *m_pEcuData;
    QLCDNumber             *m_pNumberDisplay;
};
