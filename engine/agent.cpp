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
    this->maxSpeed = 30;
    this->motionNoise = 0.5;
    this->maxAccelleration = 1;
    this->maxDecelleration = -1;
    
    this->behavior = behavior;
    behavior->addAgent(this);
    connect(this,SIGNAL(callMotionSIG(Agent*,smReal)),behavior,SLOT(agentMove(Agent*,smReal)));
}

Agent::~Agent()
{

}
void Agent::callMotion(smReal time)
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

smReal Agent::getMotionStep()
{
    smReal frame = trafficengine->getFrameDuration();
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

smReal Agent::getOrientation()
{
    smReal orientation = acos(orientationV.x());
    PARAMETERSPrintPoint("PARAMETERS: orientation",QPointF(orientation,asin(orientationV.y())));
    
    return orientation;
}

void Agent::move(QPointF objetive, smReal time)
{
#define SQR(x) ((x)*(x))
#ifdef Agent_DEBUG
    this->objective = objetive;
#endif

    QPointF position = this->getPosition();

    smReal newSpeed = (maxAccelleration*time)+speed;
    if (newSpeed >= maxSpeed)
	newSpeed = maxSpeed;
    smReal acceleration = (newSpeed-speed)/time;

    this->orientationV = objetive-position;
    orientationV /= sqrt(SQR(orientationV.x())+SQR(orientationV.y()));

    smReal linearMove = speed*time + .5*(acceleration*SQR(time));
    QPointF move = linearMove*orientationV;

    move.setY(randomService.randomNormal(move.y(),this->motionNoise*linearMove));
    move.setX(randomService.randomNormal(move.x(),this->motionNoise*linearMove));

    this->speed = newSpeed;
    this->position = position+move;
#undef SQR

#ifdef Agent_DEBUG
//     getOrientation();
#endif
    
}

void Agent::move2(QPointF objetive, smReal time)
{
#define SQR(x) ((x)*(x))
#ifdef Agent_DEBUG
    this->objective = objetive;
#endif

    QPointF position = this->getPosition();
//     smReal   currentSpeed = this->maxSpeed; //TODO temporary constant speed
    if (speed > maxSpeed)
	speed = maxSpeed;
    QPointF distanceVect = objetive-position;
    QLineF  distanceLine(position,objetive);
    smReal   moveCount = distanceLine.length()/(maxSpeed*time);

    QPointF move = distanceVect/moveCount;
    QPointF newPosition = position+move;


    //update orientation, speed and new position
    smReal newSpeed = QLineF(position,newPosition).length()/(time); /* u/s */
    this->orientationV = move;
    orientationV /= sqrt(SQR(orientationV.x())+SQR(orientationV.y()));

    if ( (newSpeed-speed)/time > maxAccelleration ) {
	newSpeed = (maxAccelleration*time)+speed;
	if (newSpeed > maxSpeed)
	    newSpeed = maxSpeed;
	smReal newMove = speed*time + .5*(maxAccelleration*SQR(speed));
	move = orientationV*newMove;
    }
//     else if ( (newSpeed-speed)/time < maxDecelleration ) {
// 	newSpeed =  (maxDecelleration*time)+speed;
// 	if (newSpeed > maxSpeed)
// 	    newSpeed = maxSpeed;
// 	smReal newMove = speed*time + .5*(maxDecelleration*SQR(speed));
// 	move = orientationV*newMove;
// 	newPosition = position+move;
//     }

//     move.setY(randomService.randomNormal(move.y(),this->motionNoise*time));
//     move.setX(randomService.randomNormal(move.x(),this->motionNoise*time));
    newPosition = position+move;

    this->speed = newSpeed;
    this->position = newPosition;
#undef SQR

#ifdef Agent_DEBUG
//     getOrientation();
#endif

}

bool Agent::collide(Agent* agent)
{
    QLineF distance(this->position,agent->position);
    if (distance.length() <= (this->dimensions+agent->dimensions))
	return true;
    else
	return false;
}

