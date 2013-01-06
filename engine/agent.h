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
#include "trafficengine.h"
#include "../map/spatialobject.h"
#include "../service/simulatorePavimentiTypes.h"

#define Agent_DEBUG

class AgentBehavior;
class TrafficEngine;

class Agent : public QObject, public SpatialObject
{
Q_OBJECT
signals:
    void callMotionSIG(Agent *agent, smReal time);

public slots:
    void callMotion(smReal time);

private:
    AgentBehavior* behavior;
    TrafficEngine* trafficengine;
    
    QPointF position;
#ifdef Agent_DEBUG
    QPointF objective;
#endif
    smReal dimensions;

//     smReal orientation; //the angle is in radians
    QPointF orientationV; //unit vector rappresenting the orientation
    smReal speed;

    smReal maxAccelleration;
    smReal maxDecelleration;
    smReal maxSpeed;
    smReal motionNoise;
    
    smReal getMaxOrientationChange(); /* relative to speed */

public:
    Agent(QString name, QString description, QString id, QPointF initialPos,
	  TrafficEngine *trafficengine, AgentBehavior* behavior, QObject* parent = 0);
    virtual ~Agent();

    /** How far the next near objetive has to be set */
    void draw(QPainter &painter);
    virtual smReal getMotionStep();
    void move(QPointF objetive, smReal time);
    void move2(QPointF objetive, smReal time);
    QPointF getPosition();
    smReal getOrientation();
    QPointF getOrientationV();
    virtual bool collide(Agent* agent);

};

#endif // AGENT_H
