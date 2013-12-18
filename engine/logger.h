/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2013  Matteo De Carlo <matteo.dek@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#ifndef LOGGER_H
#define LOGGER_H
#include <QString>
#include <QTextStream>
#include <QMap>
#include <QDomDocument>
#include <QWidget>
#include "groundsensor.h"


/**
 * This implementation writes the file while receaving the data.
 */
class Logger
{
public:
    Logger(QString savePath);
    ~Logger();

    bool addConfiguration(GroundSensor const* groundSensorBlock);

    /** forgets all the previous set sensors */
    void resetConfiguration();
    
    /** write data */
    bool writeSensorData();
    
    /** closes files */
    void close();

private:
    QMap<const GroundSensor*,int> sensorMap;
    //QVector<GroundSensor*> sensorList; //useless

    QString savePath;
    static const QString SaveNameLogConfig;
    static const QString SaveNameLog;
    static const int IndentSize;
    QFile *rawSensorDataFile;
    QTextStream *rawSensorDataFileStream;

    bool logStarted;
    unsigned int sensorBlockCounter;

    QDomDocument *config;

    /** start logging (closes config file)
     * @return false if couldn't close config file or open allframes file
     */
    bool startLogging();
    
    /** save the configuration file 
     * @return false if couldn't close config XML file
     */
    bool saveConfigFile();

};

#endif // LOGGER_H
