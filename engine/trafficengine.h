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
    qreal fps;
    qreal speed;

    QVector<AgentBehavior*> behaviorList;
    QList<Agent*> agentList;

    void moveAgents(qreal time);

    QList<qreal> lastFramesDuration;

public:
    TrafficEngine(Map *map, qreal fps=std::numeric_limits<double>::infinity());
    virtual ~TrafficEngine();

    qreal getFps();
    qreal getFrameDuration();

    
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
};

#endif // TRAFFICENGINE_H
