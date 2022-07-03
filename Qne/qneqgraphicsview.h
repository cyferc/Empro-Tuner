
#pragma once

#include <QGraphicsView>
#include <QWheelEvent>
#include <QKeyEvent>

class QneQGraphicsView : public QGraphicsView
{
public:
    QneQGraphicsView(QWidget *pParent);

    void pan(QPointF delta);
    void zoom(float scaleFactor);
    void zoomIn();
    void zoomOut();

    qreal getScale() const;

protected:

    void keyPressEvent(QKeyEvent*) override;
    void keyReleaseEvent(QKeyEvent*) override;

    void mouseMoveEvent(QMouseEvent*) override;
    void mousePressEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;
    void wheelEvent(QWheelEvent*) override;

private:

    // Flags to determine if zooming or panning should be done.
    bool m_DoMousePanning;
    qreal m_DoZoomInKey;
    qreal m_DoZoomOutKey;

    QPoint m_LastMousePos;
    qreal m_Scale;
    qreal m_PanSpeed;
    qreal m_ZoomDelta;
    Qt::MouseButton m_PanButton;

    void setMaxSize();
};
