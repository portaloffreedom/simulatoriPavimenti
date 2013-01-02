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


#include "pathbehavior.h"
#include <QLineF>
#include <iostream>

void PathBehavior::agentMove(Agent* agent, qreal time)
{    
    QPointF currentPos = agent->getPosition();
    QPointF objective = path.first();
    QLineF distance = QLineF(currentPos,objective);
    if (distance.length() < agent->getMotionStep()*tollerance) {
	path.append(objective);
	path.pop_front();
	objective = path.first();
	std::cout<<"distance("<<distance.length()<<") motionStep("<<agent->getMotionStep()<<")"<<std::endl;
    }
    agent->move(objective,time);

}

void PathBehavior::addAgent(Agent* agent)
{

}

PathBehavior::PathBehavior(QObject* parent): AgentBehavior(parent),
    tollerance(1)
{   
    //questo è un path unico provissorio
    //TODO chiedere all'utente un path
    path.append(QPointF(2,2));
    path.append(QPointF(2,5));
    path.append(QPointF(1,6));
    path.append(QPointF(0,7));
    path.append(QPointF(-3,8));
    path.append(QPointF(-5,6));
    path.append(QPointF(-5,-6));
    path.append(QPointF(-4,-7));
    path.append(QPointF(-3,-7.5));
    path.append(QPointF(-2,-6.5));
    path.append(QPointF(-1,-4));
    path.append(QPointF(-0.5,-2));
    path.append(QPointF(0,0));

}

PathBehavior::~PathBehavior()
{

}

