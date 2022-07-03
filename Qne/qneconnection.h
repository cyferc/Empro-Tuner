
#pragma once

#include <QGraphicsPathItem>

class QnePort;

class QneConnection : public QGraphicsPathItem
{
public:
	enum { Type = QGraphicsItem::UserType + 2 };

    QneConnection(QGraphicsItem *pParent);
    ~QneConnection();

	void setPos1(const QPointF &p);
	void setPos2(const QPointF &p);
    void setPort1(QnePort *p);
    void setPort2(QnePort *p);
	void updatePosFromPorts();
	void updatePath();
    QnePort* port1() const;
    QnePort* port2() const;

	void save(QDataStream&);
    void load(QDataStream&, const QMap<quint64, QnePort*> &portMap);

	int type() const { return Type; }

private:
    QPointF m_Pos1;
    QPointF m_Pos2;
    QnePort *m_pPort1;
    QnePort *m_pPort2;
};
