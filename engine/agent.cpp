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
    #ifdef Agent_DEBUG
    hadCrash(false),
    #endif
    position(initialPos),
    trafficengine(trafficEngine),
    objective(initialPos),
    orientationV(1,0)
{
//     this->orientation = 0;
    this->speed = 0;
    this->dimensions = 0.5;
    this->maxSpeed = 30;
    this->motionNoise = 0.5;
    this->maxAccelleration = 1;
    this->maxDecelleration = -10;
    
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
#ifdef Agent_DEBUG
    painter.save();
    if (hadCrash) {
	painter.setBrush(Qt::red);
	hadCrash = false;
    }
#endif
    painter.drawEllipse(position,dimensions,dimensions);
#ifdef Agent_DEBUG
    painter.save();
    painter.setBrush(Qt::transparent);
    painter.drawEllipse(objective,dimensions,dimensions);
    painter.drawLine(position,position+(orientationV*dimensions*2));
    painter.drawLine(position,objective);
    painter.restore();
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
    this->objective = objetive;

    QPointF position = this->getPosition();

    // accellerate to new speed
    smReal newSpeed = (maxAccelleration*time)+speed;

    //control if maxSpeed is reached
    if (newSpeed >= maxSpeed)
	newSpeed = maxSpeed;

    // update new orientation
    QPointF newOrientationV = objetive-position;
    newOrientationV /= sqrt(SQR(newOrientationV.x())+SQR(newOrientationV.y()));

    //control orientation accelleration
    QPointF orientationChangeV = newOrientationV-orientationV;
    smReal orientationChange = (SQR(orientationChangeV.x())+SQR(orientationChangeV.y()))/time;

    //control orientation change over speed
    if (orientationChange * newSpeed > maxSpeed)
	newSpeed = maxSpeed / orientationChange;


    //calculate new accelleration in relation of new speed
    smReal acceleration = (newSpeed-speed)/time;
    
    //control orientation accelleration
    if (acceleration < maxDecelleration) {
	acceleration = maxDecelleration;
	newSpeed = (acceleration*time)+speed;
	
	orientationChangeV /= orientationChange; //normalize vector
	orientationChange = maxSpeed / newSpeed;
	orientationChangeV *= orientationChange;

	newOrientationV = orientationChangeV+orientationV;
    }


    //calculate new position (using the "Equations of motion")
    smReal linearMove = speed*time + .5*(acceleration*SQR(time));
    QPointF move = linearMove*newOrientationV;

    //add noise to the movement
    move.setY(randomService.randomNormal(move.y(),this->motionNoise*linearMove));
    move.setX(randomService.randomNormal(move.x(),this->motionNoise*linearMove));

    // Save old agent state for revert movement
    this->oldOrientationV = this->orientationV;
    this->oldSpeed = this->speed;
    this->oldPosition = this->position;
    
    // UPDATE new agent properties
    this->orientationV = newOrientationV;
    this->speed = newSpeed;
    this->position = position+move;
#undef SQR

#ifdef Agent_DEBUG
//     getOrientation();
#endif
    
}

bool Agent::collide(Agent* agent)
{
    #define SQR(x) ((x)*(x))
    //QLineF distance(this->position,agent->position);
    float distance = sqrt(
	SQR( this->position.x() - agent->position.x() ) +
	SQR( this->position.y() - agent->position.y() )
    );
    
    if (distance <= (this->dimensions+agent->dimensions))
	return true;
    else
	return false;
    #undef SQR
}

void Agent::revertMovement()
{
    this->orientationV = this->oldOrientationV;
    this->position = this->oldPosition;
    this->speed = this->oldSpeed;
}

#ifdef Agent_DEBUG
void Agent::setCrash()
{
    hadCrash = true;
}
#endif