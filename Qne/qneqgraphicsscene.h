
#pragma once

#include <QGraphicsScene>
#include <QPainter>
#include <QRectF>
#include <QMouseEvent>

class QneQGraphicsScene : public QGraphicsScene
{
public:
    QneQGraphicsScene(QObject *pParent);

private:
    void drawBackground(QPainter *pPainter, const QRectF &rect) override;

    QColor m_ColorBackground;
    QColor m_ColorLight;
    QColor m_ColorDark;
    QColor m_ColorNull;

    QPen m_PenLight;
    QPen m_PenDark;
    QPen m_PenNull;

    QBrush m_BrushBackground;
};
