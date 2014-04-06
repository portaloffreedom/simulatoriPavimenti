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


#include "trafficengine.h"
#include "behaviors/defaultbehavior.h"
#include "behaviors/pathbehavior.h"
#include "groundengine.h"
#include "logger.h"
#include "../service/randomservice.h"
#include <cmath>

#define E_DBG(A) {if (ENGINE_DEBUG) {A;}}

TrafficEngine::TrafficEngine(SettingsWidget* settingsWidget, Map* map, GroundEngine* groundEngine, smReal fps) :
    map(map), groundEngine(groundEngine),
    fps(fps),
    speed(1.0),
    ENGINE_DEBUG(false),
    settingsWidget(settingsWidget),
    population(4)
{
    this->qtimer = new QTimer(this);
    this->timer = new Timer(this);
    this->lastFramesDuration.append(1);

    connect(qtimer,SIGNAL(timeout()),this,SLOT(step()));
    connect(map,SIGNAL(isDrawing(QPainter&)),this,SLOT(drawAgents(QPainter&)));
    //E_DBG(connect(map,SIGNAL(isDrawing(QPainter&)),this,SLOT(drawDebugInfo(QPainter&))));
    
    this->settingsWidget->setTrafficEngine(this);
}

void TrafficEngine::setEngineDebug(bool value)
{
    //std::cout<<"setting engine debug to "<<value<<std::endl;
    
    if (ENGINE_DEBUG != value) {
        if (value)
            connect(map,SIGNAL(isDrawing(QPainter&)),this,SLOT(drawDebugInfo(QPainter&)));
        else
            disconnect(map,SIGNAL(isDrawing(QPainter&)),this,SLOT(drawDebugInfo(QPainter&)));
        ENGINE_DEBUG = value;
    }
}

bool TrafficEngine::isEngineDebugActive() const
{
    return this->ENGINE_DEBUG;
}


TrafficEngine::~TrafficEngine()
{
    settingsWidget->setTrafficEngine(0);
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

void TrafficEngine::startStepByStep(Logger* logger,smReal frequency, smReal timeDuration, bool graphicCheck)
{
    
    //set the logger
    QVector< GroundSensor* > sensorVector = map->getSensorMap();
    for (int i = 0; i < sensorVector.size(); ++i) {
        logger->addConfiguration(sensorVector.at(i));
    }   
    
    // start the calculation
    const long int cicles = frequency*timeDuration;
    const smReal frame_duration = 1/frequency;
    for (int i=0; i<cicles; i++) {
        this->singleStep(frame_duration,graphicCheck);
        logger->writeSensorData();
    }
    
    delete logger;
}


//TODO decisamente da migliorare con tanti parametri
void TrafficEngine::controlPopulation(smReal time)
{
    uint totalPopulation = agentList.size();
    smReal population = 0;
    
    //if minor then 10 -> take the population number as precise
    if (this->population < 10)
        population = this->population;
    else //else randomize it a bit
        population = randomService.randomNormal(this->population,1);
    
    //If the population is enough, don't add anyone
    smReal populationToAdd = (population - totalPopulation);
    for (int i=0; i<populationToAdd; i++) {
        this->createAgent();
    }
    
    //TODO control population that has to die!
}



void TrafficEngine::updatePositions(smReal time)
{
    this->moveAgents(time);
}




void TrafficEngine::repaintGraphics(smReal time)
{
    this->map->repaint();
}




void TrafficEngine::step()
{
//     std::cout<<"#@1#_data_analysis; "<<std::scientific;
//     std::string separator = "; ";
    smRealD timebefore = 0;
    staticPassedTime = false;
    
    smRealD passedTime = timer->getElapsedSecondsAndReset();
    passedTime *= speed;
    
    this->controlPopulation(passedTime);
        smRealD controlPopulationTimer = timer->getElapsedSeconds();
//         std::cout<<controlPopulationTimer<<separator;///////////////////////////
        timebefore += controlPopulationTimer;
    this->updatePositions(passedTime);
        smRealD updatePositionsTimer = timer->getElapsedSeconds() - timebefore;
//         std::cout<<updatePositionsTimer<<separator;/////////////////////////////
        timebefore += updatePositionsTimer;
    this->updateSensors(passedTime);
        smRealD updateSensorsTimer = timer->getElapsedSeconds() - timebefore;
//         std::cout<<updateSensorsTimer<<separator;///////////////////////////////
        timebefore += updateSensorsTimer;
        
    this->repaintGraphics(passedTime);
        totalFrameRenderTime = timer->getElapsedSeconds();
        repaintGraphicsTimer = totalFrameRenderTime - timebefore;
//         std::cout<<repaintGraphicsTimer<<separator;/////////////////////////////
//         std::cout<<totalFrameRenderTime;////////////////////////////////////////
        
        this->controlPopulationTimer = controlPopulationTimer;
        this->updatePositionsTimer = updatePositionsTimer;
        this->updateSensorsTimer = updateSensorsTimer;
    
    if (lastFramesDuration.size() > 10)
        this->lastFramesDuration.pop_front();
    this->lastFramesDuration.append(totalFrameRenderTime);
    lastFrameDuration = totalFrameRenderTime;
    
//     std::cout<<std::endl;
   
}

void TrafficEngine::singleStep(smReal passedTime, bool graphicRepaint)
{   
    staticPassedTime = true;
    lastFrameDuration = passedTime;
//     std::cout<<"#@2#_data_analysis; ";
//     std::cout<<timer->getElapsedSecondsAndReset()<<std::endl;
    
    this->controlPopulation(passedTime);
    this->updatePositions(passedTime);
    this->updateSensors(passedTime);
    if (graphicRepaint)
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
    //prova a muovere tutti
    Agent *agent;
    foreach(agent,agentList) {
        agent->callMotion(time);
    }
   
    //risoluzione collisioni
    controlCollisions();
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
    
    painter.drawText(0,fontSize*2,QString("Total: ").append(QString::number(this->totalFrameRenderTime)));
    painter.drawText(0,fontSize*4,QString("controlPopulation: ").append(QString::number(this->controlPopulationTimer)));
    painter.drawText(0,fontSize*6,QString("updatePositions: ").append(QString::number(this->updatePositionsTimer)));
    painter.drawText(0,fontSize*8,QString("repaint: ").append(QString::number(this->repaintGraphicsTimer)));

    painter.restore();
}

void TrafficEngine::controlCollisions()
{
    int collisions = 1;
    
    while (collisions > 0){
        collisions = 0;
        
        //TODO controllo collisioni con gli oggetti statici
        
        // controllo collisioni tra gli agenti
        for(int i=0; i<agentList.size(); i++){
            Agent *agentA = agentList[i];
            
            for (int j=i+1; j<agentList.size(); j++){
                Agent *agentB = agentList[j];
                
                if (agentA->collide(agentB)) {
                    solveCollision(agentA,agentB);
                    collisions++;
                }
            }
        }
    }
}



smReal TrafficEngine::getFps() const
{
    smReal value, sum=0;
    foreach(value, lastFramesDuration)
        sum += 1/value;
    
    return sum/lastFramesDuration.size();
}

smReal TrafficEngine::getFrameDuration() const
{
    if (staticPassedTime) {
        return lastFrameDuration;
    }
    
    smReal value, sum=0;
    foreach(value, lastFramesDuration)
        sum += value;
        
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

void TrafficEngine::solveCollision(Agent* a, Agent* b)
{
    //TODO riscrivere in maniera decente
    E_DBG(
    a->setCrash();
    b->setCrash();
    )
    a->revertMovement();
    b->revertMovement();
}

void TrafficEngine::updateSensors(smReal time)
{
    groundEngine->reset();
    foreach(Agent *agent, agentList) {
        groundEngine->updateGround(agent);
    }
}
