#include "qneqgraphicsscene.h"

#include <cmath>

QneQGraphicsScene::QneQGraphicsScene(QObject *pParent)
    : QGraphicsScene(pParent)
    , m_ColorBackground(QColor("#393939"))//QColor(255, 255, 255))
    , m_ColorLight(QColor("#2F2F2F"))//QColor(240, 240, 240))
    , m_ColorDark(QColor("#292929"))//QColor(200, 200, 200))
    , m_ColorNull(QColor("#212121"))//QColor(150, 150, 150))
    , m_PenLight(QPen(m_ColorLight))
    , m_PenDark(QPen(m_ColorDark))
    , m_PenNull(QPen(m_ColorNull))
    , m_BrushBackground(m_ColorBackground)
{
    // initialize default pen settings
    for (auto p : {&m_PenLight, &m_PenDark, &m_PenNull}) {
        p->setWidth(0);
    }

    // initialize the background
    setBackgroundBrush(m_BrushBackground);
}

void QneQGraphicsScene::drawBackground(QPainter *pPainter, const QRectF &rect)
{
    // call parent method
    QGraphicsScene::drawBackground(pPainter, rect);

    // augment the painted with grid
    const int gridsize = 20;
    auto left = static_cast<int>(std::floor(rect.left()));
    auto right = static_cast<int>(std::ceil(rect.right()));
    auto top = static_cast<int>(std::floor(rect.top()));
    auto bottom = static_cast<int>(std::ceil(rect.bottom()));

    // compute indices of lines to draw
    const auto first_left = left - (left % gridsize);
    const auto first_top = top - (top % gridsize);

    // compute lines to draw and
    std::vector<QLine> lines_light;
    std::vector<QLine> lines_dark;
    for (auto x = first_left; x <= right; x += gridsize) {
        if (x % 100 != 0)
            lines_light.push_back(QLine(x, top, x, bottom));
        else
            lines_dark.push_back(QLine(x, top, x, bottom));
    }
    for (auto y = first_top; y <= bottom; y += gridsize) {
        if (y % 100 != 0)
            lines_light.push_back(QLine(left, y, right, y));
        else
            lines_dark.push_back(QLine(left, y, right, y));
    }

    // nullspace lines
    std::vector<QLine> lines_null;
    lines_null.push_back(QLine(0, top, 0, bottom));
    lines_null.push_back(QLine(left, 0, right, 0));

    // draw calls
    pPainter->setPen(m_PenLight);
    pPainter->drawLines(lines_light.data(), lines_light.size());

    pPainter->setPen(m_PenDark);
    pPainter->drawLines(lines_dark.data(), lines_dark.size());

    pPainter->setPen(m_PenNull);
    pPainter->drawLines(lines_null.data(), lines_null.size());

    //painter->setBrush(QBrush(QColor(200, 200, 200)));
    //painter->setPen(QColor(200, 200, 200));
    //painter->drawRect(rect);
}
