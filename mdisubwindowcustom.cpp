#include <QMdiArea>
#include <QMessageBox>
#include "mdisubwindowcustom.h"
#include "helpers.h"
#include <QtDebug>

static int gridSize = 10;


MdiSubWindowCustom::MdiSubWindowCustom(QWidget *pParent, Qt::WindowFlags flags) : QMdiSubWindow(pParent, flags)
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setMouseTracking(true);
}

void MdiSubWindowCustom::closeEvent(QCloseEvent *pCloseEvent)
{
    QMessageBox::StandardButton reply;

    reply = QMessageBox::question(this, "Close Window", "Are you sure you want to close this window?",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No)
    {
        pCloseEvent->ignore();
    }
}

void MdiSubWindowCustom::mousePressEvent(QMouseEvent *pEvent)
{
    QMdiSubWindow::mousePressEvent(pEvent);
}

void MdiSubWindowCustom::mouseMoveEvent(QMouseEvent *pEvent)
{
    QMdiSubWindow::mouseMoveEvent(pEvent);
}

void MdiSubWindowCustom::mouseReleaseEvent(QMouseEvent *pEvent)
{
    QMdiSubWindow::mouseReleaseEvent(pEvent);

    /*
     * this->resize(Helpers::round_to_nearest(this->size().width(), gridSize),
     *           Helpers::round_to_nearest(this->size().height(), gridSize));
     *
     *
     * this->move(Helpers::round_to_nearest(this->pos().x(), gridSize),
     *         Helpers::round_to_nearest(this->pos().y(), gridSize));
     */

    int newX    = 0;
    int newY    = 0;
    int halfway = gridSize / 2;

    if (this->pos().x() < 0)
    {
        /// THIS NEEDS TO BE FIXED
        if ((this->pos().x() % gridSize) > halfway)
        {
            newX = Helpers::round_up_to(gridSize, this->pos().x());
        }
        else
        {
            newX = Helpers::round_down_to(gridSize, this->pos().x());
        }
    }
    else
    {
        if ((this->pos().x() % gridSize) > halfway)
        {
            newX = Helpers::round_up_to(gridSize, this->pos().x());
        }
        else
        {
            newX = Helpers::round_down_to(gridSize, this->pos().x());
        }
    }

    if (this->pos().y() < 0)
    {
        /// THIS NEEDS TO BE FIXED
        if ((this->pos().y() % gridSize) < halfway)
        {
            newY = Helpers::round_down_to(gridSize, this->pos().y());
        }
        else
        {
            newY = Helpers::round_up_to(gridSize, this->pos().y());
        }
    }
    else
    {
        if ((this->pos().y() % gridSize) > halfway)
        {
            newY = Helpers::round_up_to(gridSize, this->pos().y());
        }
        else
        {
            newY = Helpers::round_down_to(gridSize, this->pos().y());
        }
    }

    this->move(newX, newY);


    ///////////////////////////
    //  RESIZE

    int newWidth  = this->size().width();
    int newHeight = this->size().height();

    if ((this->size().width() % gridSize) < halfway)
    {
        newWidth = Helpers::round_down_to(gridSize, this->size().width());
    }
    else
    {
        newWidth = Helpers::round_up_to(gridSize, this->size().width());
    }

    if ((this->size().height() % gridSize) < halfway)
    {
        newHeight = Helpers::round_down_to(gridSize, this->size().height());
    }
    else
    {
        newHeight = Helpers::round_up_to(gridSize, this->size().height());
    }

    this->resize(newWidth, newHeight);
}
