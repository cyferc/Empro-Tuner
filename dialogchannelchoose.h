
#pragma once

#include <QDialog>
#include <QTableWidget>
#include "ecudata.h"

namespace Ui {
class DialogChannelChoose;
}

class DialogChannelChoose : public QDialog
{
    Q_OBJECT

public:
    explicit DialogChannelChoose(QWidget *pParent, EcuData *pEcuData);
    ~DialogChannelChoose();
    QString getSelectedChannel();

private:
    Ui::DialogChannelChoose *m_pUi;
    QTableWidget            *m_pTableChannels;
};
