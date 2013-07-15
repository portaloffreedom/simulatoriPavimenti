/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2012  Matteo De Carlo <matteo.dek@gmail.com>

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


#include "defaultbehavior.h"
#include "../agent.h"

DefaultBehavior::DefaultBehavior(QObject* parent): AgentBehavior(parent)
{

}

void DefaultBehavior::agentMove(Agent* agent, uint ms)
{
    QPointF agentPosition = agent->getPosition();
    smReal motionstep = agent->getMotionStep();
    agentPosition += QPointF(motionstep,motionstep);
    agent->move(agentPosition, ms);
}

void DefaultBehavior::addAgent(Agent* agent)
{
    this->agentList.append(agent);
}

