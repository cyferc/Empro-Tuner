
#pragma once

#include <QColor>
#include <QGraphicsPathItem>

class QnePort;

class QneBlock : public QGraphicsPathItem
{
public:
	enum { Type = QGraphicsItem::UserType + 3 };

    QneBlock(QGraphicsItem *pParent);

    QnePort* addPort(const QString &name, bool isOutput, int flags = 0, int ptr = 0);
	void addInputPort(const QString &name);
	void addOutputPort(const QString &name);
	void addInputPorts(const QStringList &names);
	void addOutputPorts(const QStringList &names);
	void save(QDataStream&);
    void load(QDataStream&, QMap<quint64, QnePort*> &portMap);
    void paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption, QWidget *pWidget);
    QneBlock* clone();
    QVector<QnePort*> ports();

	int type() const { return Type; }

protected:
	QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
    const int cBorderColorSelectedR = 255;
    const int cBorderColorSelectedG = 130;
    const int cBorderColorSelectedB = 0;

    const int cBackgroundColorSelectedR = 50;
    const int cBackgroundColorSelectedG = 50;
    const int cBackgroundColorSelectedB = 50;

    const int cBorderColorDeselectedR = 150;
    const int cBorderColorDeselectedG = 150;
    const int cBorderColorDeselectedB = 150;

    const int cBackgroundColorDeselectedR = 50;
    const int cBackgroundColorDeselectedG = 50;
    const int cBackgroundColorDeselectedB = 50;

    const int cBorderWidth = 3;

    int m_HorzMargin;
    int m_VertMargin;
    int m_Width;
    int m_Height;
};
