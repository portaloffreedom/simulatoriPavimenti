/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2013  Matteo De Carlo <matteo.dek@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#ifndef GROUNDENGINE_H
#define GROUNDENGINE_H

#include <QtCore>
#include "groundsensor.h"
#include "agent.h"
#include "../map/map.h"

class Map;
class Agent;

class GroundEngine :  public QObject
{
Q_OBJECT
public:
    explicit GroundEngine( Map *parent = nullptr );
    virtual ~GroundEngine();

private:
    QVector<GroundSensor*> groundMap;

public slots:
    void reset();
    void updateGround(Agent* agent);

};

#endif // GROUNDENGINE_H
