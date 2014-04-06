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


#include "pathbehavior.h"
#include "../service/randomservice.h"
#include <QLineF>
#include <QFileDialog>
#include <iostream>

void PathBehavior::agentMove(Agent* const agent, const smReal time)
{    
    QPointF currentPos = agent->getPosition();
    QPointF objective = agents.value(agent).objective;
    QLineF distance = QLineF(currentPos,objective);
//     if (distance.length() < agent->getMotionStep()*tollerance*time) {
    if (agentIsArrived(distance, agent, time)) {
        int pos = (agents[agent].position+1)%path.count();
        AgentObjective objectiveInfo = getObjective(pos);
        agents[agent] = objectiveInfo;
        objective = objectiveInfo.objective;
        //std::cout<<"distance("<<distance.length()<<") motionStep("<<agent->getMotionStep()<<")"<<std::endl;
    }
    agent->move(objective,time);

}

bool PathBehavior::agentIsArrived(const QLineF& distance, Agent *const agent, const smReal time)
{
    return distance.length() < agent->getMotionStep()*time + tollerance;
}


void PathBehavior::addAgent(Agent* agent)
{
    agents[agent] = getObjective(0);
    agent->connect(agent,SIGNAL(destroyed(QObject*)),this,SLOT(remAgent(QObject*)));
}

PathBehavior::AgentObjective PathBehavior::getObjective(int pos)
{
    AgentObjective agentObjective;
    agentObjective.position = pos;

    agentObjective.objective = path[pos];
    QPointF objectiveNoise = noise[pos];
    agentObjective.objective.setX(randomService.randomNormal(agentObjective.objective.x(),objectiveNoise.x()));
    agentObjective.objective.setY(randomService.randomNormal(agentObjective.objective.y(),objectiveNoise.y()));
    
    return agentObjective;
}


void PathBehavior::remAgent(Agent*const agent)
{
    agents.remove(reinterpret_cast<Agent*>(agent));
}


QWidget* PathBehavior::getBehaviourWidget()
{
    return widget;
}

bool PathBehavior::loadPathFromFile(const QString& file)
{
    QFile _file(file);
    if (!_file.open(QIODevice::ReadOnly))
        return false;
    QTextStream fileStream(&_file);
    float x,y;
    
    while (!fileStream.atEnd()) {
        //TODO check file correctness
        fileStream>>x>>y;
        path.append (QPointF(x,y));
        noise.append(QPointF(STDnoise,STDnoise));
    }
    
    return true;
}


PathBehavior::PathBehavior(QObject* parent): AgentBehavior(parent),
    tollerance(0.5),
    STDnoise(0.1)
{
    widget = new PathBehaviorWidget();
    
    QFileDialog dialog(widget);
    dialog.setFileMode(QFileDialog::ExistingFile);
    bool ok = false;
    do
        if (dialog.exec())
            ok = loadPathFromFile(dialog.selectedFiles().first());
    while (!ok);

}

PathBehavior::~PathBehavior()
{

}

