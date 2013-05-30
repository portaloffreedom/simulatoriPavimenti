/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2013  Matteo De Carlo <email>

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


#include "mathservice.h"


namespace smMath {
//#define SQR(x) ((x)*(x))

smReal Quad(smReal x) {
    return x*x;
}

smReal Distance(QPointF p1, QPointF p2)
{
 return sqrt(
        Quad( p1.x() - p2.x() ) +
        Quad( p1.y() - p2.y() )
    );
}


smReal Distance(QLineF line, QPointF point)
{
    QPointF p1 = line.p1();
    QPointF p2 = line.p2();
    
    smReal upperDistanceQuad = (
        Quad( point.x() - p1.x() ) +
        Quad( point.y() - p1.y() )
    );
    smReal lowerDistanceQuad = (
        Quad( point.x() - p2.x() ) +
        Quad( point.y() - p2.y() )
    );
    smReal lineLenghtQuad = (
        Quad( p1.x() - p2.x() ) +
        Quad( p1.y() - p2.y() )
    );
    
    if (upperDistanceQuad > lowerDistanceQuad + lineLenghtQuad)
        return sqrt(lowerDistanceQuad);
    if (lowerDistanceQuad > upperDistanceQuad + lineLenghtQuad)
        return sqrt(upperDistanceQuad);
    
    //else
    QLineF p1p2(p1,p2);
    smReal m = getRectM(p1p2);// (p2.y()-p1.y()) / (p2.x() -p1.x());
    smReal q = getRectQ(p1p2,m);// p1.y() -(p1.x()*m);
    smReal heightToPoint = (
        //distanza punto retta ->  |y0 -m*x0 -q |/ sqrt(1+m*m)
        
        abs(point.y() - m*point.x() - q)
        /
        sqrt(1+Quad(m))
        
    );
    
    return heightToPoint;
}

smReal getRectM(QLineF line)
{
    return (
        (line.p2().y() - line.p1().y()) 
        / 
        (line.p2().x() - line.p1().x())
    );
}

smReal getRectQ(QLineF line, smReal m)
{
    return line.p1().y() - (line.p1().x()*m);
}

bool Intersect(QLineF l1, QLineF l2)
{
    //controllo che i punti siano sui semipiani opposti per entrambi i segmenti
    smReal ml1 = getRectM(l1);
    smReal ql1 = getRectQ(l1,ml1);
    if ( ( l2.p1().y() - ml1*l2.p1().x() - ql1 ) * (l2.p2().y() - ml1*l2.p2().x() - ql1) > 0)
        return false;
    
    smReal ml2 = getRectM(l2);
    smReal ql2 = getRectQ(l2,ml2);
    if ( ( l1.p1().y() - ml2*l1.p1().x() - ql2 ) * (l1.p2().y() - ml2*l1.p2().x() - ql2) > 0)
        return false;
    
    return true;
}

smReal Distance(QLineF l1, QLineF l2)
{
    if (Intersect(l1,l2))
        return 0;
    
    smReal distance121 = Distance(l1,l2.p1());
    smReal distance122 = Distance(l1,l2.p2());
    smReal distance211 = Distance(l2,l1.p1());
    smReal distance212 = Distance(l2,l1.p2());
    
    smReal distance = distance121;
    if (distance > distance122)
        distance = distance122;
    if (distance > distance211)
        distance = distance211;
    if (distance > distance212)
        distance = distance212;
   
    return distance;
}


bool Intersect(QLineF l1, QLineF l2, smReal radius)
{
    if (Intersect(l1,l2))
        return true;
    
    smReal distance121 = Distance(l1,l2.p1());
    if (distance121 < radius)
        return true;
    
    smReal distance122 = Distance(l1,l2.p2());
    if (distance122 < radius)
        return true;
    
    smReal distance211 = Distance(l2,l1.p1());
    if (distance211 < radius)
        return true;
    
    smReal distance212 = Distance(l2,l1.p2());
    if (distance212 < radius)
        return true;
   
    return false;
}

}
