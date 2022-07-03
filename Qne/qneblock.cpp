
#include "qneblock.h"

#include <QPen>
#include <QGraphicsScene>
#include <QFontMetrics>
#include <QPainter>

#include "qneport.h"

QneBlock::QneBlock(QGraphicsItem *pParent) : QGraphicsPathItem(pParent)
{
	QPainterPath p;
    p.addRoundedRect(-50, -15, 100, 30, 5, 5);
    setPath(p);
	setFlag(QGraphicsItem::ItemIsMovable);
	setFlag(QGraphicsItem::ItemIsSelectable);
    m_HorzMargin = 20;
    m_VertMargin = 5;
    m_Width = m_HorzMargin;
    m_Height = m_VertMargin;
}

QnePort* QneBlock::addPort(const QString &name, bool isOutput, int flags, int ptr)
{
	QnePort *port = new QnePort(this);
	port->setName(name);
	port->setIsOutput(isOutput);
	port->setNEBlock(this);
	port->setPortFlags(flags);
	port->setPtr(ptr);

	QFontMetrics fm(scene()->font());
	int w = fm.width(name);
	int h = fm.height();
	// port->setPos(0, height + h/2);
    if (w > m_Width - m_HorzMargin)
        m_Width = w + m_HorzMargin;
    m_Height += h;

	QPainterPath p;
    p.addRoundedRect(-m_Width/2, -m_Height/2, m_Width, m_Height, 5, 5);
	setPath(p);

    int y = -m_Height / 2 + m_VertMargin + port->radius();
    foreach(QGraphicsItem *port_, childItems()) {
		if (port_->type() != QnePort::Type)
			continue;

		QnePort *port = (QnePort*) port_;
		if (port->isOutput())
            port->setPos(m_Width/2 + port->radius(), y);
		else
            port->setPos(-m_Width/2 - port->radius(), y);
		y += h;
	}

	return port;
}

void QneBlock::addInputPort(const QString &name)
{
	addPort(name, false);
}

void QneBlock::addOutputPort(const QString &name)
{
	addPort(name, true);
}

void QneBlock::addInputPorts(const QStringList &names)
{
	foreach(QString n, names)
		addInputPort(n);
}

void QneBlock::addOutputPorts(const QStringList &names)
{
	foreach(QString n, names)
		addOutputPort(n);
}

void QneBlock::save(QDataStream &ds)
{
	ds << pos();

	int count(0);

    foreach(QGraphicsItem *port_, childItems())
	{
		if (port_->type() != QnePort::Type)
			continue;

		count++;
	}

	ds << count;

    foreach(QGraphicsItem *port_, childItems())
	{
		if (port_->type() != QnePort::Type)
			continue;

		QnePort *port = (QnePort*) port_;
		ds << (quint64) port;
		ds << port->portName();
		ds << port->isOutput();
		ds << port->portFlags();
	}
}

void QneBlock::load(QDataStream &ds, QMap<quint64, QnePort*> &portMap)
{
	QPointF p;
	ds >> p;
	setPos(p);
	int count;
	ds >> count;
	for (int i = 0; i < count; i++)
	{
		QString name;
		bool output;
		int flags;
		quint64 ptr;

		ds >> ptr;
		ds >> name;
		ds >> output;
		ds >> flags;
		portMap[ptr] = addPort(name, output, flags, ptr);
	}
}

#include <QStyleOptionGraphicsItem>

void QneBlock::paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption, QWidget *pWidget)
{
    Q_UNUSED(pOption)
    Q_UNUSED(pWidget)

	if (isSelected()) {
        pPainter->setPen(QPen(QColor(cBorderColorSelectedR, cBorderColorSelectedG, cBorderColorSelectedB), cBorderWidth));
        pPainter->setBrush(QBrush(QColor(cBackgroundColorSelectedR, cBackgroundColorSelectedG, cBackgroundColorSelectedB)));
	} else {
        pPainter->setPen(QPen(QColor(cBorderColorDeselectedR, cBorderColorDeselectedG, cBorderColorDeselectedB), cBorderWidth));
        pPainter->setBrush(QBrush(QColor(cBackgroundColorDeselectedR, cBackgroundColorDeselectedG, cBackgroundColorDeselectedB)));
	}

    pPainter->drawPath(path());
}

QneBlock* QneBlock::clone()
{
    QneBlock *b = new QneBlock(0);
    this->scene()->addItem(b);

	foreach(QGraphicsItem *port_, childItems())
	{
		if (port_->type() == QnePort::Type)
		{
			QnePort *port = (QnePort*) port_;
			b->addPort(port->portName(), port->isOutput(), port->portFlags(), port->ptr());
		}
	}

	return b;
}

QVector<QnePort*> QneBlock::ports()
{
	QVector<QnePort*> res;
	foreach(QGraphicsItem *port_, childItems())
	{
		if (port_->type() == QnePort::Type)
			res.append((QnePort*) port_);
	}
	return res;
}

QVariant QneBlock::itemChange(GraphicsItemChange change, const QVariant &value)
{

    Q_UNUSED(change);

	return value;
}

