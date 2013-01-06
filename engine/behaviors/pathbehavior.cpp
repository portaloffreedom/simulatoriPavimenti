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

void PathBehavior::agentMove(Agent* agent, smReal time)
{    
    QPointF currentPos = agent->getPosition();
    QPointF objective = path[agents[agent]];
    QLineF distance = QLineF(currentPos,objective);
    if (distance.length() < agent->getMotionStep()*tollerance*time) {
	int pos = (agents[agent]+1)%path.count();
	agents[agent] = pos;
	objective = path[pos];
	std::cout<<"distance("<<distance.length()<<") motionStep("<<agent->getMotionStep()<<")"<<std::endl;
    }
    agent->move(objective,time);

}

void PathBehavior::addAgent(Agent* agent)
{
    agents[agent] = 0;
    agent->connect(agent,SIGNAL(destroyed(QObject*)),this,SLOT(remAgent(QObject*)));
}

void PathBehavior::remAgent(QObject* agent)
{
    agents.remove(reinterpret_cast<Agent*>(this->sender()));
    std::cout<<"sender: "<<this->sender()<<
	     "\nagent:  "<<agent<<std::endl;
}


QWidget* PathBehavior::getBehaviourWidget()
{
    return widget;
}


PathBehavior::PathBehavior(QObject* parent): AgentBehavior(parent),
    tollerance(100)
{
    widget = new PathBehaviorWidget();
    
    //questo è un path unico provissorio
    //TODO chiedere all'utente un path
    path.append(QPointF( +2.0, +2.0 ));
    path.append(QPointF( +2.0, +5.0 ));
    path.append(QPointF( +1.0, +6.0 ));
    path.append(QPointF( +0.0, +7.0 ));
    path.append(QPointF( -3.0, +8.0 ));
    path.append(QPointF( -5.0, +6.0 ));
    path.append(QPointF( -5.0, -6.0 ));
    path.append(QPointF( -4.0, -7.0 ));
    path.append(QPointF( -3.0, -7.5 ));
    path.append(QPointF( -2.0, -6.5 ));
    path.append(QPointF( -1.0, -4.0 ));
    path.append(QPointF( -0.5, -2.0 ));
    path.append(QPointF( +0.0, +0.0 ));

}

PathBehavior::~PathBehavior()
{

}

