#include "verticallabel.h"
#include <QPainter>

VerticalLabel::VerticalLabel(QWidget *pParent)
    : QLabel(pParent)
{

}

VerticalLabel::VerticalLabel(const QString &text, QWidget *pParent)
: QLabel(text, pParent)
{
}

void VerticalLabel::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    QPen pen  = palette().text().color();
    painter.setPen(pen);
    painter.setBrush(Qt::Dense1Pattern);

    // no translate needed for 90 rotate
    //painter.rotate(90);

    painter.translate( sizeHint().width(), sizeHint().height() );
    painter.rotate(270);

    // No alignment
    //painter.drawText(0, 0, text());

    // Align centre
    QFontMetrics fm = painter.fontMetrics();
    painter.drawText(-(height() / 2) + (fm.width(text()) / 2), 0, text());
}

QSize VerticalLabel::minimumSizeHint() const
{
    QSize s = QLabel::minimumSizeHint();
    return QSize(s.height(), s.width());
}

QSize VerticalLabel::sizeHint() const
{
    QSize s = QLabel::sizeHint();
    return QSize(s.height(), s.width());
}
