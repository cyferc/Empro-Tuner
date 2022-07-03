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

#include "qneport.h"

#include <QGraphicsScene>
#include <QFontMetrics>

#include <QPen>

#include "qneconnection.h"

QnePort::QnePort(QGraphicsItem *pParent):
    QGraphicsPathItem(pParent)
{
    m_pLabel = new QGraphicsTextItem(this);
    m_pLabel->setDefaultTextColor(QColor(255, 255, 255));

    m_Radius = 5;
    m_Margin = 2;

	QPainterPath p;
    p.addEllipse(-m_Radius, -m_Radius, 2*m_Radius, 2*m_Radius);
	setPath(p);

	setFlag(QGraphicsItem::ItemSendsScenePositionChanges);

    m_PortFlags = 0;
}

QnePort::~QnePort()
{
    foreach(QneConnection *conn, m_ConnectionsVec)
		delete conn;
}

void QnePort::setNEBlock(QneBlock *b)
{
    m_pBlock = b;
}

void QnePort::setName(const QString &n)
{
    m_Name = n;
    m_pLabel->setPlainText(n);
}

void QnePort::setIsOutput(bool o)
{
    m_IsOutput = o;

	QFontMetrics fm(scene()->font());
    //QRect r = fm.boundingRect(name);

    if (m_IsOutput)
    {
        setPen(QPen(QColor(cOutputBorderColorR, cOutputBorderColorG, cOutputBorderColorB)));
        setBrush(QBrush(QColor(cOutputBackgroundColorR, cOutputBackgroundColorG, cOutputBackgroundColorB)));
        m_pLabel->setPos(-m_Radius - m_Margin - m_pLabel->boundingRect().width(), -m_pLabel->boundingRect().height()/2);
    }
	else
    {
        setPen(QPen(QColor(cInputBorderColorR, cInputBorderColorG, cInputBorderColorB)));
        setBrush(QBrush(QColor(cInputBackgroundColorR, cInputBackgroundColorG, cInputBackgroundColorB)));
        m_pLabel->setPos(m_Radius + m_Margin, -m_pLabel->boundingRect().height()/2);
    }
}

int QnePort::radius()
{
    return m_Radius;
}

bool QnePort::isOutput()
{
    return m_IsOutput;
}

QVector<QneConnection*>& QnePort::connections()
{
    return m_ConnectionsVec;
}

void QnePort::setPortFlags(int f)
{
    m_PortFlags = f;

    if (m_PortFlags & TypePort)
	{
		QFont font(scene()->font());
		font.setItalic(true);
        m_pLabel->setFont(font);
		setPath(QPainterPath());
    } else if (m_PortFlags & NamePort)
	{
		QFont font(scene()->font());
		font.setBold(true);
        m_pLabel->setFont(font);
		setPath(QPainterPath());
	}
}

QneBlock* QnePort::block() const
{
    return m_pBlock;
}

quint64 QnePort::ptr()
{
    return m_Ptr;
}

void QnePort::setPtr(quint64 p)
{
    m_Ptr = p;
}

bool QnePort::isConnected(QnePort *other)
{
    foreach(QneConnection *conn, m_ConnectionsVec)
		if (conn->port1() == other || conn->port2() == other)
			return true;

	return false;
}

QVariant QnePort::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if (change == ItemScenePositionHasChanged)
	{
        foreach(QneConnection *conn, m_ConnectionsVec)
		{
			conn->updatePosFromPorts();
			conn->updatePath();
		}
	}
	return value;
}
