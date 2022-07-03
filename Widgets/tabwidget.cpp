#include "tabwidget.h"
#include <QInputDialog>
#include <QMdiArea>
#include <QMessageBox>

TabWidget::TabWidget(QWidget *parent)
    : QTabWidget(parent)
{
    setMovable(true);
    setTabsClosable(true);
    setContextMenuPolicy(Qt::CustomContextMenu);

    // Add new tab button
    m_pAddTabButton = new QPushButton(QString("+"), this);
    m_pAddTabButton->setToolTip(QString("New tab"));
    m_pAddTabButton->resize(20, 20);
    m_pAddTabButton->show();

    QObject::connect(m_pAddTabButton,
                     SIGNAL(clicked()),
                     this,
                     SLOT(newTab()));

    QObject::connect(this,
                     SIGNAL(tabCloseRequested(int)),
                     this,
                     SLOT(_tabCloseRequested(int)));

    // repaint to move btnAddTab into position
    repaint();
}

void TabWidget::paintEvent(QPaintEvent *pEvent)
{
    QTabWidget::paintEvent(pEvent);
    m_pAddTabButton->move(this->width() - 21, 1);
}

void TabWidget::_tabCloseRequested(int index)
{
    QMessageBox::StandardButton reply;

    reply = QMessageBox::question(this, "Delete Tab", "Are you sure you want to delete this tab?",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        removeTab(index);

        /*if (_tabWidget->count() < 1)
        {
            // Disable all actions using a tab
            ui->actionAdd_Number_Dislpay->setDisabled(true);
            ui->actionCascade->setDisabled(true);
            ui->actionTile->setDisabled(true);
            ui->actionRename_Tab->setDisabled(true);
            ui->actionDelete_Tab->setDisabled(true);
            ui->actionClose_All_Windows->setDisabled(true);
            ui->actionAdd_Data_Log_Display->setDisabled(true);
            ui->actionAdd_Dial_Display->setDisabled(true);
        }*/
    }
}

void TabWidget::newTab()
{
    // enter new tab name
    bool    ok;
    QString defaultText = "New Tab";
    QString text        = QInputDialog::getText(this, tr("New Tab Name"),
                                                tr("Tab name:"), QLineEdit::Normal, defaultText, &ok, Qt::WindowTitleHint);

    if (!ok)
    {
        return;
    }

    if (text.isEmpty())
    {
        text = defaultText;
    }

    QMdiArea *area = new QMdiArea(this);
    area->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    area->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // Add new tab
    insertTab(count(), area, text);

    // select new tab
    setCurrentIndex(indexOf(area));

    emit atLeastOneTabOpen();
}
