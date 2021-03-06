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


#ifndef AGENTBEHAVIOR_H
#define AGENTBEHAVIOR_H

#include <QObject>
#include <QWidget>
#include "agent.h"
#include "../service/simulatorePavimentiTypes.h"

class Agent;

class AgentBehavior : public QObject
{
Q_OBJECT

public:
    AgentBehavior(QObject *parent = 0);
    virtual ~AgentBehavior();

public slots:
    virtual void addAgent(Agent *agent) = 0;
    virtual void agentMove(Agent *agent, smReal time) = 0;
    virtual QWidget* getBehaviourWidget() = 0;
};

#endif // AGENTBEHAVIOR_H
