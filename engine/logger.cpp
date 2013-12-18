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

#include "logger.h"
#include <QMessageBox>
#include <QDomDocument>

const QString Logger::SaveNameLogConfig = QString("config.xml");
const QString Logger::SaveNameLog = QString("capture.txt");
const int Logger::IndentSize = 4;

Logger::Logger(QString savePath) :
 logStarted(false)
,savePath(savePath)
,sensorBlockCounter(0)
,config(nullptr)
{
    this->resetConfiguration();
}

Logger::~Logger()
{
    this->close();
    delete config;
}


bool Logger::addConfiguration(const GroundSensor* groundSensorBlock)
{
    if (logStarted)
        return false;
    static const QString idSA = "Id";
    static const QString xposSA = "Position-X";
    static const QString yposSA = "Position-Y"; 
    static const QString rotationSA= "Position-Y"; 
    
    sensorMap.insert(groundSensorBlock,sensorBlockCounter);
    //sensorList.append(&groundSensorBlock); //see header file
    
    QDomElement sensorBlock = config->createElement("SensorBlock");
    sensorBlock.setAttribute(idSA, sensorBlockCounter++);
    sensorBlock.setAttribute(xposSA, groundSensorBlock->getCX());
    sensorBlock.setAttribute(yposSA, groundSensorBlock->getCY());
    sensorBlock.setAttribute(rotationSA, groundSensorBlock->getAngle());
    
    QDomElement root = config->firstChildElement("root");
    root.appendChild(sensorBlock);
    
    for (int sensorCounter = 0; sensorCounter < groundSensorBlock->getDim(); sensorCounter++) {
        QDomElement sensor = config->createElement("Sensor");
        sensor.setAttribute(idSA, sensorCounter);
        
        QPointF pos = groundSensorBlock->realWorldSensorPos(sensorCounter);
        sensor.setAttribute(xposSA, pos.x());
        sensor.setAttribute(yposSA, pos.y());
        
        sensorBlock.appendChild(sensor);
    }
    
    return true;
}

void Logger::resetConfiguration()
{
    delete config;
    logStarted = false;
    config = new QDomDocument("Sersor Configuration");
    //TODO test if this "root" variable works
    config->appendChild(config->createElement("root"));
}

bool Logger::writeSensorData()
{
    if (!logStarted) {
        if (!startLogging())
            return false;
    }
    
    QMapIterator<const GroundSensor*, int> i(sensorMap);
    while (i.hasNext()) {
        i.next();
        
        //stuff with one sensorBlock
        const int sensorBlockNumber = i.value();
        const GroundSensor *sensorBlock = i.key();
        
        for (int i = 0; i < sensorBlock->getDim(); i++) {
            const smReal sensorValue = sensorBlock->sensorStatus(i);
            (*rawSensorDataFileStream)<<sensorValue<<'\t';
        }
        (*rawSensorDataFileStream)<<'\n';
        
    }
    
}

bool Logger::startLogging()
{
    if (!saveConfigFile())
        return false;
    
    rawSensorDataFile = new QFile(savePath+SaveNameLog);
    if (!rawSensorDataFile->open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(nullptr, "Sersor Data File",
                              QString("Cannot write file %1:\n%2.")
                              .arg(rawSensorDataFile->fileName())
                              .arg(rawSensorDataFile->errorString()));
        delete rawSensorDataFile;
        return false;
    }
    rawSensorDataFileStream = new QTextStream(rawSensorDataFile);
    
    logStarted = true;
}

void Logger::close()
{
    rawSensorDataFileStream->flush();
    rawSensorDataFile->close();
    delete rawSensorDataFile;
    delete rawSensorDataFileStream;
}


bool Logger::saveConfigFile()
{   
    if (logStarted)
        return false;
    QFile configFile(savePath+SaveNameLogConfig);
    if (!configFile.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(nullptr, "Sersor Configuration XML File",
                              QString("Cannot write file %1:\n%2.")
                              .arg(configFile.fileName())
                              .arg(configFile.errorString()));
        return false;
    }
    
    QTextStream out(&configFile);
    config->save(out, IndentSize);
    configFile.close();
    return true;
}

