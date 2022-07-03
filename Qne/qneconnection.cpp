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

#include "qneconnection.h"

#include "qneport.h"

#include <QBrush>
#include <QPen>
#include <QGraphicsScene>

QneConnection::QneConnection(QGraphicsItem *pParent) : QGraphicsPathItem(pParent)
{
    setPen(QPen(QColor(255, 255, 255), 2));
	setBrush(Qt::NoBrush);
    setZValue(-1);
    m_pPort1 = 0;
    m_pPort2 = 0;
}

QneConnection::~QneConnection()
{
    if (m_pPort1)
        m_pPort1->connections().remove(m_pPort1->connections().indexOf(this));
    if (m_pPort2)
        m_pPort2->connections().remove(m_pPort2->connections().indexOf(this));
}

void QneConnection::setPos1(const QPointF &p)
{
    m_Pos1 = p;
}

void QneConnection::setPos2(const QPointF &p)
{
    m_Pos2 = p;
}

void QneConnection::setPort1(QnePort *p)
{
    m_pPort1 = p;

    m_pPort1->connections().append(this);
}

void QneConnection::setPort2(QnePort *p)
{
    m_pPort2 = p;

    m_pPort2->connections().append(this);
}

void QneConnection::updatePosFromPorts()
{
    m_Pos1 = m_pPort1->scenePos();
    m_Pos2 = m_pPort2->scenePos();
}

void QneConnection::updatePath()
{
    QPainterPath p;

    p.moveTo(m_Pos1);

    qreal dx = m_Pos2.x() - m_Pos1.x();
    qreal dy = m_Pos2.y() - m_Pos1.y();

    QPointF ctr1(m_Pos1.x() + dx * 0.45, m_Pos1.y() + dy * 0.1);
    QPointF ctr2(m_Pos1.x() + dx * 0.55, m_Pos1.y() + dy * 0.9);

    p.cubicTo(ctr1, ctr2, m_Pos2);

	setPath(p);
}

QnePort* QneConnection::port1() const
{
    return m_pPort1;
}

QnePort* QneConnection::port2() const
{
    return m_pPort2;
}

void QneConnection::save(QDataStream &ds)
{
    ds << (quint64) m_pPort1;
    ds << (quint64) m_pPort2;
}

void QneConnection::load(QDataStream &ds, const QMap<quint64, QnePort*> &portMap)
{
	quint64 ptr1;
	quint64 ptr2;
	ds >> ptr1;
	ds >> ptr2;

	setPort1(portMap[ptr1]);
	setPort2(portMap[ptr2]);
	updatePosFromPorts();
	updatePath();
}
