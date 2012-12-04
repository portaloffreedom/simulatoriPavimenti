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


#ifndef SPATIALOBJECT_H
#define SPATIALOBJECT_H
#include <QString>
#include <QPainter>
#include "figure.h"

class SpatialObject
{

public:
    SpatialObject(QString name, QString description, QString id);
    virtual ~SpatialObject();
    
    const QString getDescription();
    const QString getID();
    const QString getName();
    const QString toString();
    
    void draw(QPainter&);
    
    //memory management of the figures (deletes) is handled internally SpatialObject
    void addFigure(Figure* figure);
    Figure *lastFigure();
    
protected:
    QString name;
    QString description;
    QString idPreposition;
    int idNumber;
    
private:
    QVector<Figure*> figures;
};

#endif // SPATIALOBJECT_H
