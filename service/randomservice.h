/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2012  Matteo De Carlo <email>

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


#ifndef RANDOMSERVICE_H
#define RANDOMSERVICE_H

#include <QtGlobal>
#include <cmath>
#include <cstdlib>

#ifdef WIN32
#define M_PI 3.14159265358979323846
#endif

class RandomService
{

public:
    RandomService(uint seed);
    ~RandomService();

    qreal randomReal();
    uint  randomInt();
    qreal randomNormal(qreal mu, qreal sigma);

//     std::mt19937 gen;
    qreal lastMU;
    qreal lastSigma;


    static const uint RandMax = RAND_MAX;
    static const uint RandMin = 0;

private:
    qreal randn_notrig(qreal mu=0.0, qreal sigma=1.0);
    qreal   randn_trig(qreal mu=0.0, qreal sigma=1.0);
    
    bool deviateAvailable;	//flag
    qreal storedDeviate;	//deviate from previous calculation

};

extern RandomService randomService;

#endif // RANDOMSERVICE_H
