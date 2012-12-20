/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2012  Matteo De Carlo <email>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/


#ifndef AGENT_H
#define AGENT_H

#include <QtCore/QObject>
#include "agentbehavior.h"
#include "../map/spatialobject.h"

#define Agent_DEBUG

class AgentBehavior;

class Agent : public QObject, public SpatialObject
{
Q_OBJECT
signals:
    void callMotionSIG(Agent *agent, qreal time);

public slots:
    void callMotion(qreal time);

private:
    AgentBehavior* behavior;
    QPointF position;
#ifdef Agent_DEBUG
    QPointF objective;
#endif
    qreal dimensions;

    qreal orientation; //the angle is in radians
    qreal speed;

    qreal maxAccelleration;
    qreal maxDecelleration;
    qreal maxSpeed;
    
    qreal getMaxOrientationChange(); /* relative to speed */

public:
    Agent(QString name, QString description, QString id, QPointF initialPos, AgentBehavior* behavior, QObject* parent = 0);
    virtual ~Agent();

    /** How far the next near objetive has to be set */
    void draw(QPainter &painter);
    virtual qreal getMotionStep();
    void move(QPointF objetive, qreal time);
    QPointF getPosition();
    qreal getOrientation();

};

#endif // AGENT_H
