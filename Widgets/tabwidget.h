
#pragma once

#include <QTabWidget>
#include <QPushButton>

class TabWidget : public QTabWidget
{
    Q_OBJECT
public:
    TabWidget(QWidget *parent);

signals:
    void noTabsOpen();
    void atLeastOneTabOpen();

public slots:
    void newTab();

private slots:
    void _tabCloseRequested(int index);

private:
    void paintEvent(QPaintEvent *pEvent);
    QPushButton *m_pAddTabButton;
};
