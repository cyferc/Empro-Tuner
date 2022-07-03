#include "qneqgraphicsview.h"
#include <QScrollBar>
#include <QGraphicsItem>
#include "Qne/qneblock.h"
#include "Qne/qneconnection.h"

#define VIEW_CENTER viewport()->rect().center()
#define VIEW_WIDTH viewport()->rect().width()
#define VIEW_HEIGHT viewport()->rect().height()

QneQGraphicsView::QneQGraphicsView(QWidget *pParent)
    : QGraphicsView(pParent)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setMaxSize();
    centerOn(0, 0);

    m_ZoomDelta = 0.1;
    m_PanSpeed = 1;
    m_DoMousePanning = false;
    m_Scale = 1.0;

    m_PanButton = Qt::MiddleButton;
    m_DoZoomInKey = Qt::Key_Plus;
    m_DoZoomOutKey = Qt::Key_Minus;
}

qreal QneQGraphicsView::getScale() const
{
    return m_Scale;
}

void QneQGraphicsView::keyPressEvent(QKeyEvent * event)
{
    qint32 key = event->key();

    if (key == m_DoZoomInKey){
        zoomIn();
    }
    else if (key == m_DoZoomOutKey)
    {
        zoomOut();
    }
    else
    {
        QGraphicsView::keyPressEvent(event);
    }
}

void QneQGraphicsView::keyReleaseEvent(QKeyEvent * event)
{
    QGraphicsView::keyReleaseEvent(event);
}

void QneQGraphicsView::mouseMoveEvent(QMouseEvent * event)
{
    if (m_DoMousePanning){
        QPointF mouseDelta = mapToScene(event->pos()) - mapToScene(m_LastMousePos);
        pan(mouseDelta);

        m_LastMousePos = event->pos();
        //event->accept();
    }
    else
    {
        QGraphicsView::mouseMoveEvent(event);
    }
}

void QneQGraphicsView::mousePressEvent(QMouseEvent * event)
{
    /// Bring to front ///
    QGraphicsItem *clickedItem = itemAt(event->pos());

    // If we clicked on something
    if (clickedItem)
    {
        // Find largest Z of all items in scene
        qreal maxZ = 0;

        foreach (QGraphicsItem *item, this->scene()->items())
        {
            maxZ = qMax(maxZ, item->zValue());
        }

        QneConnection *connection = qgraphicsitem_cast<QneConnection *>(clickedItem);

        // If item is connection, do not bring to front
        if (!connection)
        {
            QneBlock *block = qgraphicsitem_cast<QneBlock *>(clickedItem);

            // If item is block
            if (block)
            {
                // Assign new Z, bring to front
                block->setZValue(maxZ + 0.001);
            }
            else
            {
                // Bring parent block to front
                clickedItem->topLevelItem()->setZValue(maxZ + 0.001);
            }
        }
    }

    /// Pan ///
    if (event->button() == m_PanButton){
        m_LastMousePos = event->pos();
        m_DoMousePanning = true;
        //event->accept();
    }
    else
    {
        QGraphicsView::mousePressEvent(event);
    }
}

void QneQGraphicsView::mouseReleaseEvent(QMouseEvent * event)
{
    if (event->button() == m_PanButton){
        m_DoMousePanning = false;
    }
    else
    {
        QGraphicsView::mouseReleaseEvent(event);
    }
}

void QneQGraphicsView::wheelEvent(QWheelEvent *event)
{
    QPoint scrollAmount = event->angleDelta();

    // Apply zoom.
    scrollAmount.y() > 0 ? zoomIn() : zoomOut();
}

void QneQGraphicsView::setMaxSize()
{
    setSceneRect(INT_MIN/2, INT_MIN/2, INT_MAX, INT_MAX);
}

void QneQGraphicsView::zoom(float scaleFactor)
{
    scale(scaleFactor, scaleFactor);
    m_Scale *= scaleFactor;
}

void QneQGraphicsView::zoomIn()
{
    zoom(1 + m_ZoomDelta);
}

void QneQGraphicsView::zoomOut()
{
    zoom (1 - m_ZoomDelta);
}

void QneQGraphicsView::pan(QPointF delta)
{
    // Scale the pan amount by the current zoom.
    delta *= m_Scale;
    delta *= m_PanSpeed;

    // Have panning be anchored from the mouse.
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    QPoint newCenter(VIEW_WIDTH / 2 - delta.x(),  VIEW_HEIGHT / 2 - delta.y());
    centerOn(mapToScene(newCenter));

    // For zooming to anchor from the view center.
    setTransformationAnchor(QGraphicsView::AnchorViewCenter);
}
