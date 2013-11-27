/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2012  Matteo De Carlo <<matteo.dek@gmail.com>>

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
    
    QPointF oldPosition;
    QPointF oldOrientationV;
    smReal oldSpeed;
    
    QPointF position;
    QPointF objective;
    smReal dimensions;

//     smReal orientation; //the angle is in radians
    QPointF orientationV; //unit vector rappresenting the orientation
    smReal speed;

    smReal maxAccelleration;
    smReal maxDecelleration;
    smReal maxSpeed;
    smReal motionNoise;

    #ifdef Agent_DEBUG
    bool hadCrash;
    #endif
    
    smReal getMaxOrientationChange(); /* relative to speed */

public:
    Agent(QString name, QString description, QString id, QPointF initialPos,
          TrafficEngine *trafficengine, AgentBehavior* behavior, QObject* parent = 0);
    virtual ~Agent();

    virtual void draw(QPainter &painter);
    virtual void move(QPointF objetive, smReal time);
    /** Revert last movement made from "move" function */
    void revertMovement();
    /** Returns true if the agents collides */
    static bool collide(Agent* a, Agent *b);
    
    /** How far the next near objetive has to be set */
    virtual smReal getMotionStep() const;
    virtual bool collide(Agent* agent) const;
    virtual smReal pathDistance(Agent* agent) const;
    
    QPointF getPosition() const;
    smReal getOrientation();
    QPointF getOrientationV();
    #ifdef Agent_DEBUG
    void setCrash();
    #endif
};

#endif // AGENT_H
