/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2012  Matteo De Carlo <email>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <iostream>
#include <QRegExp>

#include "spatialobject.h"
#include "../service/randomservice.h"

SpatialObject::SpatialObject(QString name, QString description, QString id)
{
    this->name = name;
    this->description = description;
    int idNumberPos = id.indexOf(QRegExp(QString("[0-9]")));
    this->idPreposition = id.left(idNumberPos);
    this->idNumber = id.right(id.size()-idNumberPos).toUInt();
    
//     std::cout<<"caricato oggetto:\n"<<this->toString().toStdString();
}

SpatialObject::~SpatialObject()
{
    int num_figures = figures.size();
    for (int i = 0; i < num_figures; i ++) {
	delete figures[i];
    }
}

const QString SpatialObject::getDescription()
{
    return description;
}

const QString SpatialObject::getID()
{
    QString ID;
    ID.append(idPreposition);
    ID.append(idNumber);
    return ID;
}

const QString SpatialObject::getName()
{
    return name;
}

const QString SpatialObject::toString()
{
    QString toString;
    toString.append(name).append(" : ").append(getID());
    toString.append("  # ").append(description);
    return toString;
}

void SpatialObject::draw(QPainter& painter)
{
    int num_figures = figures.size();
    for (int i = 0; i < num_figures; i ++) {
	figures[i]->draw(painter);
    }
}

void SpatialObject::addFigure(Figure* figure)
{
    figures.append(figure);
}

Figure *SpatialObject::lastFigure()
{
    return figures.back();
}

smReal SpatialObject::area()
{
    smReal sum = 0;
    Figure *figure;
    foreach(figure, this->figures) {
	sum += figure->area();
    }

    return sum;
}

QPointF SpatialObject::getRandomPointInside()
{
    int polNumber = randomService.randomInt() % figures.size();

    return figures[polNumber]->getRandomPointInside();
}



