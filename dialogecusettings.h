
#pragma once

#include <QDialog>

namespace Ui {
class DialogEcuSettings;
}

class DialogEcuSettings : public QDialog
{
    Q_OBJECT

public:
    explicit DialogEcuSettings(QWidget *pParent);
    ~DialogEcuSettings();

private:
    Ui::DialogEcuSettings *m_pUi;
};
