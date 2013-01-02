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


#include "agent.h"
#include <cmath>
#include <iostream>
#include <string>
#include "../service/randomservice.h"

Agent::Agent(QString name, QString description, QString id, QPointF initialPos,
	     TrafficEngine *trafficEngine, AgentBehavior* behavior, QObject* parent) :
    QObject(parent),
    SpatialObject(name,description,id),
    position(initialPos),
    trafficengine(trafficEngine),
#ifdef Agent_DEBUG
    objective(initialPos),
#endif
    orientationV(1,0)
{
//     this->orientation = 0;
    this->speed = 0;
    this->dimensions = 0.5;
    this->maxSpeed = 3;
    this->motionNoise = .05;

    this->behavior = behavior;
    behavior->addAgent(this);
    connect(this,SIGNAL(callMotionSIG(Agent*,qreal)),behavior,SLOT(agentMove(Agent*,qreal)));
}

Agent::~Agent()
{

}
void Agent::callMotion(qreal time)
{
    emit callMotionSIG(this, time);
}

void Agent::draw(QPainter &painter)
{
    painter.drawEllipse(position,dimensions,dimensions);
#ifdef Agent_DEBUG
    painter.save();
    painter.setBrush(Qt::transparent);
    painter.drawEllipse(objective,dimensions,dimensions);
    painter.drawLine(position,position+(orientationV*dimensions*2));
    painter.drawLine(position,objective);
    painter.restore();
#endif
}

qreal Agent::getMotionStep()
{
    qreal frame = trafficengine->getFrameDuration();
    return speed*frame;
    
}

QPointF Agent::getPosition()
{
    return position;
}

QPointF Agent::getOrientationV()
{
    return orientationV;
}

template <typename V>
inline void PARAMETERSPrintVal(const std::string &description, V val) {
    std::cout<<description<<"( "<<val<<" )"<<std::endl;
}

inline void PARAMETERSPrintPoint(const std::string &description, QPointF point) {
    std::cout<<description<<"( "<<point.x()<<" : "<<point.y()<<" )"<<std::endl;
}

qreal Agent::getOrientation()
{
    qreal orientation = acos(orientationV.x());
    PARAMETERSPrintPoint("PARAMETERS: orientation",QPointF(orientation,asin(orientationV.y())));
    
    return orientation;
}

void Agent::move(QPointF objetive, qreal time)
{
#define SQR(x) ((x)*(x))
    
    //TODO try to move near
    
#ifdef Agent_DEBUG
    this->objective = objetive;
#endif

    QPointF position = this->getPosition();
    qreal   currentSpeed = this->maxSpeed; //TODO temporary constant speed
    QPointF distanceVect = objetive-position;
    QLineF  distanceLine(position,objetive);
    qreal   moveCount = distanceLine.length()/(currentSpeed*time);

    QPointF move = distanceVect/moveCount;
    move.setY(randomService.randomNormal(move.y(),this->motionNoise));
    move.setX(randomService.randomNormal(move.x(),this->motionNoise));
    QPointF newPosition = position+move;


    //update orientation, speed and new position
    this->speed = QLineF(position,newPosition).length()/(time); /* u/s */
    this->orientationV = move;
    orientationV /= sqrt(SQR(orientationV.x())+SQR(orientationV.y()));
    
    this->position = newPosition;
#undef SQR

#ifdef Agent_DEBUG
    getOrientation();
#endif
    
//     std::cout<<"###############\n";
//     PARAMETERSPrintPoint("position", this->getPosition());
//     PARAMETERSPrintVal<qreal>("speed",this->speed);
//     PARAMETERSPrintVal<uint>("PARAMETERS: time",time);
//     PARAMETERSPrintVal<qreal>("PARAMETERS: distanceLine.length",distanceLine.length());
//     PARAMETERSPrintVal<qreal>("PARAMETERS: moveCount",moveCount);
//     PARAMETERSPrintPoint("PARAMETERS: distanceVect",distanceVect);
//     PARAMETERSPrintPoint("PARAMETERS: move",move);
}


