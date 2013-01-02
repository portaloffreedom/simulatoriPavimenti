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
#include <iostream>

TrafficEngine::TrafficEngine(Map* map, qreal fps) :
    map(map),
    fps(fps),
    speed(1.0)
{
    this->qtimer = new QTimer(this);
    this->timer = new Timer(this);
    this->addBehavior(new PathBehavior(this));
    this->lastFramesDuration.append(1);

    connect(qtimer,SIGNAL(timeout()),this,SLOT(step()));
    connect(map,SIGNAL(isDrawing(QPainter&)),this,SLOT(drawAgents(QPainter&)));

    this->createAgent();
}

TrafficEngine::~TrafficEngine()
{
    delete this->map;
//     this->~QObject();
}

void TrafficEngine::start()
{
    if (qtimer->isActive())
	qtimer->stop();
    else {
	timer->reset();
	qtimer->start((1/fps)*1000);
    }
}

void TrafficEngine::step()
{
    qreal passedTime = timer->getElapsedSecondsAndReset();
    
    this->moveAgents(passedTime*speed);

    //paint the result
    this->map->repaint();

    if (lastFramesDuration.size() > 10)
	this->lastFramesDuration.pop_front();
    this->lastFramesDuration.append(passedTime);
}

void TrafficEngine::singleStep()
{
    qWarning("attenzione questa azione ancora non funziona bene");
    qreal passedTime = speed;
    this->moveAgents(passedTime);
    this->map->repaint();
}


void TrafficEngine::createAgent(int behaviorI)
{
    this->createAgent(this->behaviorList[behaviorI]);
}

void TrafficEngine::createAgent(AgentBehavior* behavior)
{
    QPointF position = map->getEntrancePoint();
    Agent* agent = new Agent("ciccio","","a1",position,this,behavior,this);
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
    #ifdef ENGINE_DEBUG
    painter.save();
    painter.resetTransform();
    int fontSize = painter.font().pointSize();
    painter.drawText(2,fontSize*2,QString("fps: ").append(QString::number(this->getFps())));
    painter.restore();
    #endif

    painter.setPen(QPen(Qt::magenta));
    painter.setBrush(QBrush(Qt::magenta));
    Agent *agent;
    foreach(agent,agentList) {
	agent->draw(painter);
    }

    painter.restore();
}

qreal TrafficEngine::getFps()
{
    qreal value, sum;
    foreach(value, lastFramesDuration) {
	sum += 1/value;
    }
    return sum/lastFramesDuration.size();
}

qreal TrafficEngine::getFrameDuration()
{
    qreal value, sum;
    foreach(value, lastFramesDuration) {
	sum += value;
    }
    return sum/lastFramesDuration.size();
}

void TrafficEngine::setSpeed(int speed)
{
    this->speed = speed/(100.);
    //TODO this number shoult not be Hardcoded
//     std::cout<<this->speed<<" = "<<speed<<"/(1000.*100.)"<<std::endl;
}

