
#pragma once

#include <QDialog>
#include "vocalObject.h"
#include "ecudata.h"

namespace Ui {
class DialogSensorSetup;
}

class DialogSensorSetup : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSensorSetup(QWidget *pParent, EcuData *pEcuData);
    ~DialogSensorSetup();

private:
    Ui::DialogSensorSetup *ui;
    void addNumberDisplay(QWidget *pParentWidget, VocalObject *pChannel);
};
