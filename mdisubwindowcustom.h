
#pragma once

#include <QMdiSubWindow>
#include <QMouseEvent>

class MdiSubWindowCustom : public QMdiSubWindow
{
public:
    MdiSubWindowCustom(QWidget *pParent, Qt::WindowFlags flags = 0);

protected:
    void mousePressEvent(QMouseEvent *pEvent);
    void mouseMoveEvent(QMouseEvent *pEvent);
    void mouseReleaseEvent(QMouseEvent *pEvent);
    void closeEvent(QCloseEvent *pCloseEvent);
};
