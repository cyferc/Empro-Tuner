
#pragma once

#include <QGraphicsPathItem>

class QneBlock;
class QneConnection;

class QnePort : public QGraphicsPathItem
{
public:
	enum { Type = QGraphicsItem::UserType + 1 };
	enum { NamePort = 1, TypePort = 2 };

    QnePort(QGraphicsItem *pParent);
    ~QnePort();

    void setNEBlock(QneBlock*);
	void setName(const QString &n);
	void setIsOutput(bool o);
	int radius();
	bool isOutput();
    QVector<QneConnection*>& connections();
	void setPortFlags(int);

    const QString& portName() const { return m_Name; }
    int portFlags() const { return m_PortFlags; }

	int type() const { return Type; }

    QneBlock* block() const;

	quint64 ptr();
	void setPtr(quint64);

    bool isConnected(QnePort*);

protected:
	QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
    QneBlock *m_pBlock;
    QString m_Name;
    bool m_IsOutput;
    QGraphicsTextItem *m_pLabel;
    int m_Radius;
    int m_Margin;
    QVector<QneConnection*> m_ConnectionsVec;
    int m_PortFlags;
    quint64 m_Ptr;

    const int cInputBorderColorR = 50;
    const int cInputBorderColorG = 50;
    const int cInputBorderColorB = 50;

    const int cInputBackgroundColorR = 66;
    const int cInputBackgroundColorG = 134;
    const int cInputBackgroundColorB = 244;

    const int cOutputBorderColorR = 50;
    const int cOutputBorderColorG = 50;
    const int cOutputBorderColorB = 50;

    const int cOutputBackgroundColorR = 133;
    const int cOutputBackgroundColorG = 244;
    const int cOutputBackgroundColorB = 66;
};
