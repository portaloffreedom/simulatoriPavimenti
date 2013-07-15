/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2012  Matteo De Carlo <<matteo.dek@gmail.com>>

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

#include "simulatorePavimentiTypes.h"

#ifdef WIN32
#define M_PI 3.14159265358979323846
#endif

class RandomService
{

public:
    RandomService(uint seed);
    ~RandomService();

    smReal randomReal();
    uint  randomInt();
    smReal randomNormal(smReal mu, smReal sigma);

//     std::mt19937 gen;
    smReal lastMU;
    smReal lastSigma;


    static const uint RandMax = RAND_MAX;
    static const uint RandMin = 0;

private:
    smReal randn_notrig(smReal mu=0.0, smReal sigma=1.0);
    smReal   randn_trig(smReal mu=0.0, smReal sigma=1.0);
    
    bool deviateAvailable;	//flag
    smReal storedDeviate;	//deviate from previous calculation

};

extern RandomService randomService;

#endif // RANDOMSERVICE_H
