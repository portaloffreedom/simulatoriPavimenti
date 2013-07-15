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


#ifndef TIMER_H
#define TIMER_H

#ifdef WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#include <QtCore/QObject>
#include "simulatorePavimentiTypes.h"


class Timer : public QObject
{
Q_OBJECT

public:
    explicit Timer(QObject* parent = 0);
    virtual ~Timer();


    smRealD getElapsedSeconds();
    smRealD getElapsedSecondsAndReset();
    
public slots:
    void reset();

protected:
    #ifdef WIN32
    LARGE_INTEGER m_CounterFrequency;
    LARGE_INTEGER m_LastCount;
    #else
    timeval m_LastCount;
    #endif
};
#endif // TIMER_H
