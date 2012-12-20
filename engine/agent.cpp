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

Agent::Agent(QString name, QString description, QString id, QPointF initialPos, AgentBehavior* behavior, QObject* parent)
    : QObject(parent), SpatialObject(name,description,id), position(initialPos)
#ifdef Agent_DEBUG
    , objective(initialPos)
#endif
{
    this->orientation = 0;
    this->speed = 0;
    this->dimensions = 0.5;
    this->maxSpeed = 3;
    
    this->behavior = behavior;
    behavior->addAgent(this);
    connect(this,SIGNAL(callMotionSIG(Agent*,qreal)),behavior,SLOT(agentMove(Agent*,qreal)));

    std::cout<<"###############\n"
	       "position: (" <<this->position.x()<< ":" <<this->position.y()<< ")\tspeed: "<<this->speed<<std::endl;
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
    painter.restore();
#endif
}

qreal Agent::getMotionStep()
{
    //TODO it should return the motion step relative to the speed
    return dimensions;
    
}

QPointF Agent::getPosition()
{
    return position;
}

qreal Agent::getOrientation()
{
    return orientation;
}


template <typename V>
void PARAMETERSPrintVal(char* description, V val) {
    std::cout<<description<<"( "<<val<<" )"<<std::endl;
}

void PARAMETERSPrintPoint(char* description, QPointF point) {
    std::cout<<description<<"( "<<point.x()<<" : "<<point.y()<<" )"<<std::endl;
}

void Agent::move(QPointF objetive, qreal time)
{
    #define SQR(x) ((x)*(x))
    
    //TODO try to move near
    //TODO update orientation, speed and new position
    
#ifdef Agent_DEBUG
    this->objective = objetive;
#endif

    QPointF position = this->getPosition();
    qreal   currentSpeed = this->maxSpeed; //TODO temporary constant speed
    QPointF distanceVect = objetive-position;
    QLineF  distanceLine(position,objetive);
    qreal   moveCount = distanceLine.length()/(currentSpeed*time);

    QPointF move = distanceVect/moveCount;
    QPointF newPosition = position+move;

    
    this->speed = QLineF(position,newPosition).length()/(time); /* u/s */
    
    this->position = newPosition;
    #undef SQR

    #define PPOINT( a ) ( "(" <<( (a) .x() )<< ":" <<( (a) .y() )<< ")" )
    
    std::cout<<"###############\n";
    PARAMETERSPrintPoint("position", this->getPosition());
    PARAMETERSPrintVal<qreal>("speed",this->speed);
	       
    PARAMETERSPrintVal<uint>("PARAMETERS: time",time);
    PARAMETERSPrintVal<qreal>("PARAMETERS: distanceLine.length",distanceLine.length());
    PARAMETERSPrintVal<qreal>("PARAMETERS: moveCount",moveCount);
    PARAMETERSPrintPoint("PARAMETERS: distanceVect",distanceVect);
    PARAMETERSPrintPoint("PARAMETERS: move",move);


    #undef PPOINT
}


