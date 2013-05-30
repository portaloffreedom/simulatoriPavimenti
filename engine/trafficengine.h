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


// #ifdef ENGINE_DEBUG
// #define E_DBG(A) (A)
// #elseif
// #define E_DEBUG(A)
// #endif

class AgentBehavior;
class Agent;

class Map;

class TrafficEngine : public QObject
{
Q_OBJECT

private:
    SettingsWidget *settingsWidget;
    Map *map;
    QTimer *qtimer;
    Timer *timer;
    smReal fps;
    smReal speed;
    uint population;
    bool ENGINE_DEBUG;

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
    TrafficEngine(SettingsWidget* settingWidget, Map* map, smReal fps = std::numeric_limits<double>::infinity());
    virtual ~TrafficEngine();

    smReal getFps();
    smReal getFrameDuration();
    bool isEngineDebugActive();

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
    void setEngineDebug(bool value);

private slots:
    void drawAgents(QPainter &painter);
    void drawDebugInfo(QPainter &painter);
    bool agentCollideAgent(Agent* agent);
};

#endif // TRAFFICENGINE_H
