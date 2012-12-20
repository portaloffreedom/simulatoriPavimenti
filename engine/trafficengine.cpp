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


#include "trafficengine.h"
#include "behaviors/defaultbehavior.h"
#include "behaviors/pathbehavior.h"

TrafficEngine::TrafficEngine(Map* map)
{
    this->map = map;
    this->qtimer = new QTimer(this);
    this->timer = new Timer(this);
    this->addBehavior(new PathBehavior(this));

    connect(qtimer,SIGNAL(timeout()),this,SLOT(step()));
    connect(map,SIGNAL(isDrawing(QPainter&)),this,SLOT(drawAgents(QPainter&)));

    this->createAgent();
}

TrafficEngine::~TrafficEngine()
{
    delete this->map;
//     this->~QObject();
}

void TrafficEngine::start(uint ms)
{
    if (qtimer->isActive())
	qtimer->stop();
    else {
	timer->reset();
	qtimer->start(30);
    }
}

//TODO void TrafficEngine::step()
void TrafficEngine::step()
{
    qreal passedTime = timer->getElapsedSecondsAndReset();
    
    this->moveAgents(passedTime);

    //paint the result
    this->map->repaint();
}

void TrafficEngine::createAgent(int behaviorI)
{
    this->createAgent(this->behaviorList[behaviorI]);
}

void TrafficEngine::createAgent(AgentBehavior* behavior)
{
    QPointF position = map->getEntrancePoint();
    Agent* agent = new Agent("ciccio","","a1",position,behavior);
    this->agentList.append(agent);
}

int TrafficEngine::addBehavior(AgentBehavior* behavior)
{
    int pos = behaviorList.size();
    this->behaviorList.append(behavior);
    return pos;
}

void TrafficEngine::moveAgents(qreal time)
{
    Agent *agent;
    foreach(agent,agentList) {
	agent->callMotion(time);
    }

}

void TrafficEngine::drawAgents(QPainter& painter)
{
    painter.save();
    
    painter.setPen(QPen(Qt::magenta));
    painter.setBrush(QBrush(Qt::magenta));
    Agent *agent;
    foreach(agent,agentList) {
	agent->draw(painter);
    }

    painter.restore();
}








