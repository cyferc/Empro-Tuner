#include "dialogecusettings.h"
#include "ui_dialogecusettings.h"

DialogEcuSettings::DialogEcuSettings(QWidget *pParent) :
    QDialog(pParent),
    m_pUi(new Ui::DialogEcuSettings)
{
    m_pUi->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

DialogEcuSettings::~DialogEcuSettings()
{
    delete m_pUi;
}
