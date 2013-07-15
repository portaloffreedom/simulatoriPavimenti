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


#include "timer.h"

Timer::Timer(QObject* parent) : QObject(parent)
{
    #ifdef WIN32
    QueryPerformanceFrequency(&m_CounterFrequency);
    QueryPerformanceCounter(&m_LastCount);
    #else
    gettimeofday(&m_LastCount, 0);
    #endif
}

Timer::~Timer()
{

}

void Timer::reset()
{
    #ifdef WIN32
    QueryPerformanceCounter(&m_LastCount);
    #else
    gettimeofday(&m_LastCount, 0);
    #endif
}

smRealD Timer::getElapsedSeconds()
{
    // Get the current count
    #ifdef WIN32
    LARGE_INTEGER lCurrent;
    QueryPerformanceCounter(&lCurrent);

    return float((lCurrent.QuadPart - m_LastCount.QuadPart) /
		double(m_CounterFrequency.QuadPart));

    #else
    timeval lcurrent;
    gettimeofday(&lcurrent, 0);
    smRealD fSeconds = (smRealD)(lcurrent.tv_sec - m_LastCount.tv_sec);
    smRealD fFraction = (smRealD)(lcurrent.tv_usec - m_LastCount.tv_usec) * 0.000001f;
    return fSeconds + fFraction;
    #endif
}

smRealD Timer::getElapsedSecondsAndReset()
{
    smRealD time = this->getElapsedSeconds();
    this->reset();
    return time;
}
