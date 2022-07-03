
#pragma once

#include <QObject>

class QGraphicsScene;
class QneConnection;
class QGraphicsItem;
class QPointF;
class QneBlock;

class QneController : public QObject
{
	Q_OBJECT
public:
    explicit QneController(QObject *pParent);

    void install(QGraphicsScene *pScene);

	bool eventFilter(QObject *, QEvent *);

	void save(QDataStream &ds);
	void load(QDataStream &ds);

private:
	QGraphicsItem *itemAt(const QPointF&);

private:
    QGraphicsScene *m_pScene;
    QneConnection *m_pConn;
	// QNEBlock *selBlock;
};
