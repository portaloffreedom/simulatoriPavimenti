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


#ifndef MATHSERVICE_H
#define MATHSERVICE_H
#include <QPointF>
#include <QLineF>
#include "simulatorePavimentiTypes.h"
//#include "../service/simulatorePavimentiTypes.h"

namespace smMath
{
    smReal Quad(smReal x);
    smReal Distance (QPointF p1, QPointF p2);
    smReal Distance (QLineF line, QPointF point);
    smReal Distance (QLineF l1, QPointF l2);
    bool Intersect(QLineF l1, QLineF l2);
    bool Intersect(QLineF l1, QLineF l2, smReal radius);
    smReal getRectM(QLineF line);
    smReal getRectQ(QLineF line,smReal m);
};

#endif // MATHSERVICE_H
