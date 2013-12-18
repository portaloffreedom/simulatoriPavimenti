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


#ifndef PATHBEHAVIOR_H
#define PATHBEHAVIOR_H

#include <QPoint>
#include <QVector>
#include <QMap>
#include "pathbehaviorwidget.h"
#include "../agent.h"
#include "../agentbehavior.h"


class PathBehavior : public AgentBehavior
{
Q_OBJECT

private:
    QVector<QPointF> path;
    QVector<QPointF> noise;
    
    const smReal tollerance;
    PathBehaviorWidget* widget;

    struct AgentObjective {
        int position;
        QPointF objective;
    };

    QMap<Agent*,AgentObjective> agents;

    //methods
    AgentObjective getObjective (int pos);
    bool agentIsArrived(const QLineF &distance, Agent *const agent, const smReal time);
    
public slots:
    virtual void agentMove(Agent *const agent, const smReal time);
    virtual void addAgent(Agent* agent);
    void remAgent(QObject *agent);
    virtual QWidget* getBehaviourWidget();

public:
    PathBehavior(QObject* parent = nullptr);
    virtual ~PathBehavior();
};

#endif // PATHBEHAVIOR_H
