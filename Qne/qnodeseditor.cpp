/* Copyright (c) 2012, STANISLAW ADASZEWSKI
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of STANISLAW ADASZEWSKI nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL STANISLAW ADASZEWSKI BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */

#include "qnodeseditor.h"

#include <QGraphicsScene>
#include <QEvent>
#include <QGraphicsSceneMouseEvent>

#include "qneport.h"
#include "qneconnection.h"
#include "qneblock.h"

QneController::QneController(QObject *pParent) :
    QObject(pParent)
{
    m_pConn = 0;
}

void QneController::install(QGraphicsScene *pScene)
{
    pScene->installEventFilter(this);
    m_pScene = pScene;
}

QGraphicsItem* QneController::itemAt(const QPointF &pos)
{
    QList<QGraphicsItem*> items = m_pScene->items(QRectF(pos - QPointF(1,1), QSize(3,3)));

	foreach(QGraphicsItem *item, items)
		if (item->type() > QGraphicsItem::UserType)
			return item;

	return 0;
}

bool QneController::eventFilter(QObject *o, QEvent *e)
{
	QGraphicsSceneMouseEvent *me = (QGraphicsSceneMouseEvent*) e;

	switch ((int) e->type())
	{
	case QEvent::GraphicsSceneMousePress:
	{

		switch ((int) me->button())
		{
		case Qt::LeftButton:
		{
			QGraphicsItem *item = itemAt(me->scenePos());
            if (item && item->type() == QnePort::Type)
			{
                m_pConn = new QneConnection(0);
                m_pScene->addItem(m_pConn);
                m_pConn->setPort1((QnePort*) item);
                m_pConn->setPos1(item->scenePos());
                m_pConn->setPos2(me->scenePos());
                m_pConn->updatePath();

				return true;
            } else if (item && item->type() == QneBlock::Type)
			{
				/* if (selBlock)
					selBlock->setSelected(); */
				// selBlock = (QNEBlock*) item;
			}
			break;
		}
		case Qt::RightButton:
		{
			QGraphicsItem *item = itemAt(me->scenePos());
            if (item && (item->type() == QneConnection::Type || item->type() == QneBlock::Type))
				delete item;
			// if (selBlock == (QNEBlock*) item)
				// selBlock = 0;
			break;
		}
		}
	}
	case QEvent::GraphicsSceneMouseMove:
	{
        if (m_pConn)
		{
            m_pConn->setPos2(me->scenePos());
            m_pConn->updatePath();
			return true;
		}
		break;
	}
	case QEvent::GraphicsSceneMouseRelease:
	{
        if (m_pConn && me->button() == Qt::LeftButton)
		{
			QGraphicsItem *item = itemAt(me->scenePos());
            if (item && item->type() == QnePort::Type)
			{
                QnePort *port1 = m_pConn->port1();
                QnePort *port2 = (QnePort*) item;

				if (port1->block() != port2->block() && port1->isOutput() != port2->isOutput() && !port1->isConnected(port2))
				{
                    m_pConn->setPos2(port2->scenePos());
                    m_pConn->setPort2(port2);
                    m_pConn->updatePath();
                    m_pConn = 0;
					return true;
				}
			}

            delete m_pConn;
            m_pConn = 0;
			return true;
		}
		break;
	}
	}
	return QObject::eventFilter(o, e);
}

void QneController::save(QDataStream &ds)
{
    foreach(QGraphicsItem *item, m_pScene->items())
        if (item->type() == QneBlock::Type)
		{
			ds << item->type();
            ((QneBlock*) item)->save(ds);
		}

    foreach(QGraphicsItem *item, m_pScene->items())
        if (item->type() == QneConnection::Type)
		{
			ds << item->type();
            ((QneConnection*) item)->save(ds);
		}
}

void QneController::load(QDataStream &ds)
{
    m_pScene->clear();

    QMap<quint64, QnePort*> portMap;

	while (!ds.atEnd())
	{
		int type;
		ds >> type;
        if (type == QneBlock::Type)
		{
            QneBlock *block = new QneBlock(0);
            m_pScene->addItem(block);
			block->load(ds, portMap);
        } else if (type == QneConnection::Type)
		{
            QneConnection *conn = new QneConnection(0);
            m_pScene->addItem(conn);
			conn->load(ds, portMap);
		}
	}
}
