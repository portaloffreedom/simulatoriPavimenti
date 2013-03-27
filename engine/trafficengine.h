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


#ifndef TRAFFICENGINE_H
#define TRAFFICENGINE_H

#include <QtCore/QObject>
#include <QTimer>
#include <limits> //to use std::numeric_limits<float>::infinity()

#include "../map/map.h"
#include "agentbehavior.h"
#include "agent.h"
#include "../service/timer.h"
#include "../service/simulatorePavimentiTypes.h"

#define ENGINE_DEBUG

class AgentBehavior;
class Agent;


class TrafficEngine : public QObject
{
Q_OBJECT

private:
    Map *map;
    QTimer *qtimer;
    Timer *timer;
    smReal fps;
    smReal speed;
    uint population;

    QList<AgentBehavior*> behaviorList;
    QList<Agent*> agentList;

    void moveAgents(smReal time);

    QList<smReal> lastFramesDuration;
    smReal controlPopulationTimer;
    smReal totalFrameRenderTime;
    smReal updatePositionsTimer;
    smReal repaintGraphicsTimer;

    void controlPopulation(smReal time);
    void controlCollisions();
    void solveCollision(Agent *a, Agent *b);
    void updatePositions(smReal time);
    void repaintGraphics(smReal time);

public:
    TrafficEngine(Map *map, smReal fps=std::numeric_limits<double>::infinity());
    virtual ~TrafficEngine();

    smReal getFps();
    smReal getFrameDuration();

signals:
    void newBehaviour(QWidget* widgetBehaviour);
    
public slots:
    void start();
    void step();
    void singleStep();
    void setSpeed(int speed);
    void createAgent(int behaviorI = 0);
    void createAgent(AgentBehavior* behavior);
    int addBehavior(AgentBehavior* behavior);

private slots:
    void drawAgents(QPainter &painter);
    void drawDebugInfo(QPainter &painter);
    bool agentCollideAgent(Agent* agent);
};

#endif // TRAFFICENGINE_H
