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


#ifndef CIRCLE_H
#define CIRCLE_H

#include <../map/figure.h>


class Circle : public Figure
{

public:
    Circle();
    Circle(QPointF center, qreal radius);
    virtual ~Circle();
    
    virtual bool isInside(QPoint& );
    virtual void draw(QPainter& painter);
    
public slots:
    void setCenterAndRadius(QPointF center,qreal radius);
    
private:
    QPointF center;
    qreal radius;
};

#endif // CIRCLE_H
