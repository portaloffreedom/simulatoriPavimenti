/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2012  Matteo De Carlo <<matteo.dek@gmail.com>>

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

#include <QtGlobal>
#include <ctime>
#include <cstdlib>

#include "polygon.h"

Polygon::Polygon()
{
    verteces.reserve(3);
//     qsrand(time(0));
}

Polygon::~Polygon()
{

}

void Polygon::draw(QPainter& painter)
{
    painter.drawPolygon(verteces.data(),verteces.size());
}

void Polygon::addVertex(QPointF point)
{
    verteces.append(point);

    if (verteces.size() == 1) {
	boundingBox.setLeft(point.x());
	boundingBox.setRight(point.x());
	boundingBox.setBottom(point.y());
	boundingBox.setTop(point.y());
    }
    else {
	if (point.x() < boundingBox.left())
	    boundingBox.setLeft(point.x());
	if (point.x() > boundingBox.right())
	    boundingBox.setRight(point.x());
	if (point.y() < boundingBox.bottom())
	    boundingBox.setBottom(point.y());
	if (point.y() > boundingBox.top())
	    boundingBox.setTop(point.y());
    }
}

//TODO bool Polygon::isInside(QPoint& )
/**
 * reference: http://en.wikipedia.org/wiki/Point_in_polygon
 */
bool Polygon::isInside(QPointF& point)
{
    //first tests the distance from the center of the polygon
    

    //then if is closer enough a more complex test is taken

	//take care if the intersection point is a vertex
    return false;
}

smReal Polygon::area()
{
    //TODO this only works for a non-self-intersecting (simple) polygon
    #define next(x) ((x+1)%verteces.size())
    
    smReal sum = 0;
    for (int i=0; i<verteces.size(); i++) {
	smReal mul1 = verteces[i].x() * verteces[next(i)].y();
	smReal mul2 = verteces[next(i)].x() * verteces[i].y();
	sum += mul1 - mul2;
    }
    
    sum /=2;

    return sum;

    #undef next
}


QPointF Polygon::getRandomPointInside()
{
    //TODO find a way to do this
    return verteces[0];
    
    /*
    //this only works for a non-self-intersecting (simple) polygon
    #define RAND (qrand()/(static_cast<smReal>(RAND_MAX)))
    #define next(x) ((x+1)%verteces.size())
    
    smReal randy = RAND;

    randy += boundingBox.bottom();
    randy *= boundingBox.height();

    smReal randx = RAND;

    for (int i=0; i<verteces.size(); i++) {
	if (verteces[i].y() < )
    }


    #undef RAND
    #undef next
    */

}




