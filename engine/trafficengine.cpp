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
#include "../service/randomservice.h"
#include <iostream>
#include <cmath>

TrafficEngine::TrafficEngine(Map* map, smReal fps) :
    map(map),
    fps(fps),
    speed(1.0),
    population(30)
{
    this->qtimer = new QTimer(this);
    this->timer = new Timer(this);
//     this->addBehavior(new PathBehavior(this));
    this->lastFramesDuration.append(1);

    connect(qtimer,SIGNAL(timeout()),this,SLOT(step()));
    connect(map,SIGNAL(isDrawing(QPainter&)),this,SLOT(drawAgents(QPainter&)));
#ifdef ENGINE_DEBUG
    connect(map,SIGNAL(isDrawing(QPainter&)),this,SLOT(drawDebugInfo(QPainter&)));
#endif

//     this->createAgent();
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




//TODO decisamente da migliorare con tanti parametri
void TrafficEngine::controlPopulation(smReal time)
{
    uint totalPopulation = agentList.size();
//     smReal populationToAdd = population - totalPopulation;
//     populationToAdd = randomService.randomNormal(population,std::abs(populationToAdd/2));
    smReal population = randomService.randomNormal(this->population,1);
    smReal populationToAdd = (population - totalPopulation);

    //If the population is enought, don't add anyone
    for (int i=1; i<populationToAdd; i++) {
	this->createAgent();
    }
}



void TrafficEngine::updatePositions(smReal time)
{
    this->moveAgents(time*speed);
}




void TrafficEngine::repaintGraphics(smReal time)
{
    this->map->repaint();
}




void TrafficEngine::step()
{
    smReal passedTime = timer->getElapsedSecondsAndReset();

    this->controlPopulation(passedTime);
	smReal controlPopulationTimer = timer->getElapsedSeconds();
    this->updatePositions(passedTime);
	smReal updatePositionsTimer = timer->getElapsedSeconds() - controlPopulationTimer;
    this->repaintGraphics(passedTime);
	this->controlPopulationTimer = controlPopulationTimer;
	this->updatePositionsTimer = updatePositionsTimer;
	totalFrameRenderTime = timer->getElapsedSeconds();
	repaintGraphicsTimer = totalFrameRenderTime - this->updatePositionsTimer;
    
    if (lastFramesDuration.size() > 10)
	this->lastFramesDuration.pop_front();
    this->lastFramesDuration.append(passedTime);
}

void TrafficEngine::singleStep()
{
    qWarning("attenzione questa azione ancora non funziona bene");
    smReal passedTime = speed;
    
    this->controlPopulation(passedTime);
    this->updatePositions(passedTime);
    this->repaintGraphics(passedTime);
}


void TrafficEngine::createAgent(int behaviorI)
{
    this->createAgent(this->behaviorList[behaviorI]);
}

void TrafficEngine::createAgent(AgentBehavior* behavior)
{
    QPointF position = map->getEntrancePoint();
    Agent* agent = new Agent("ciccio","","a1",position,this,behavior,this);

    if (agentCollideAgent(agent)) {
	delete agent;
	return;
    }
    this->agentList.append(agent);
}

int TrafficEngine::addBehavior(AgentBehavior* behavior)
{
    int pos = behaviorList.size();
    this->behaviorList.append(behavior);
    emit newBehaviour(behavior->getBehaviourWidget());
    return pos;
}

void TrafficEngine::moveAgents(smReal time)
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

void TrafficEngine::drawDebugInfo(QPainter& painter)
{
    painter.save();
    painter.resetTransform();

    int fontSize = painter.font().pointSize();
    painter.translate(2,fontSize*2);
    painter.drawText(0,0,QString("fps: ").append(QString::number(this->getFps())));

    painter.setPen(Qt::darkGreen);
    painter.translate(5,fontSize);
    painter.scale(.7,.7);
    
//     painter.translate(0,fontSize*2);
    painter.drawText(0,fontSize*2,QString("Total: ").append(QString::number(this->totalFrameRenderTime)));
//     painter.translate(0,fontSize*2);
    painter.drawText(0,fontSize*4,QString("controlPopulation: ").append(QString::number(this->controlPopulationTimer)));
//     painter.translate(0,fontSize*2);
    painter.drawText(0,fontSize*6,QString("updatePositions: ").append(QString::number(this->updatePositionsTimer)));
//     painter.translate(0,fontSize*2);
    painter.drawText(0,fontSize*8,QString("repaint: ").append(QString::number(this->repaintGraphicsTimer)));

    painter.restore();
}


smReal TrafficEngine::getFps()
{
    smReal value, sum=0;
    foreach(value, lastFramesDuration) {
	sum += 1/value;
    }
    return sum/lastFramesDuration.size();
}

smReal TrafficEngine::getFrameDuration()
{
    smReal value, sum=0;
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

bool TrafficEngine::agentCollideAgent(Agent* agent)
{
    Agent *otherAgent;
    foreach(otherAgent,agentList) {
	if (agent->collide(otherAgent))
	    return true;
    }
    return false;
}
