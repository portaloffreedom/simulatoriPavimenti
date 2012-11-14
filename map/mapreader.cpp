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


#include "mapreader.h"
#include <QXmlStreamReader>
#include <iostream>

MapReader::MapReader(QFile &xmlFile)
{
    this->xmlFile = &xmlFile;
    map = new Map();
    
    connect(this,SIGNAL(beginBorder()),map,SLOT(beginBorder()));
    connect(this,SIGNAL(addBorderPoint(QPointF)),map,SLOT(addBorderPoint(QPointF)));
    connect(this,SIGNAL(closeBorder()),map,SLOT(closeBorder()));
    
    connect(this,SIGNAL(beginEntrance()),map,SLOT(beginEntrance()));
    connect(this,SIGNAL(addEntrancePoint(QPointF)),map,SLOT(addEntrancePoint(QPointF)));
    connect(this,SIGNAL(closeEntrace()),map,SLOT(closeEntrace()));
    
    connect(this,SIGNAL(beginExit()),map,SLOT(beginExit()));
    connect(this,SIGNAL(addExitPoint(QPointF)),map,SLOT(addExitPoint(QPointF)));
    connect(this,SIGNAL(closeExit()),map,SLOT(closeExit()));
    
    connect(this,SIGNAL(beginObstacle()),map,SLOT(beginObstacle()));
    connect(this,SIGNAL(addObstaclePoint(QPointF)),map,SLOT(addObstaclePoint(QPointF)));
    connect(this,SIGNAL(closeObstacle()),map,SLOT(closeObstacle()));
    
    
    parseStatus = this->parse();
}

MapReader::~MapReader()
{
    this->xmlFile->close();
}

bool MapReader::parse()
{
    QXmlStreamReader xml;
    xml.setDevice(xmlFile);
    xml.tokenType();
    
    //parse all the elements
    for (int i=0; i < 500; i++) {
	if (xml.atEnd()) {
	    std::cout<<"finito il file!"<<std::endl;
	    break;
	}
	if (xml.error()) {
	    emit error(xml.errorString());
	    return false;
	}
	QXmlStreamReader::TokenType token= xml.readNext();
	std::cout<<i<<":"<<xml.tokenString().toStdString()<<"   \t- ";
	
	
	switch (token) {
	    case QXmlStreamReader::Comment :
		std::cout<<xml.text().toString().toStdString()<<std::endl;
		break;
		
	    case QXmlStreamReader::StartElement :
		std::cout<<xml.name().toString().toStdString()<<std::endl;
		if (xml.name().compare("border") == 0) {
		    parsePolygon(xml,&MapReader::beginBorder,&MapReader::addBorderPoint,&MapReader::closeBorder);
		    break;
		}
		if (xml.name().compare("entrance") == 0) {
		    parsePolygon(xml,&MapReader::beginEntrance,&MapReader::addEntrancePoint,&MapReader::closeEntrace);
		    break;
		}
		if (xml.name().compare("exit") == 0) {
		    parsePolygon(xml,&MapReader::beginExit,&MapReader::addExitPoint,&MapReader::closeExit);
		    break;
		}
		if (xml.name().compare("obstacle") == 0) {
		    parsePolygon(xml,&MapReader::beginObstacle,&MapReader::addObstaclePoint,&MapReader::closeObstacle);
		    break;
		}
		break;
	    
	    case QXmlStreamReader::EndElement :
		std::cout<<xml.name().toString().toStdString()<<std::endl;
		break;
	    
	    case QXmlStreamReader::Characters :
		if (xml.isWhitespace())
		    std::cout<<"whitespaces"<<std::endl;
		else
		    std::cout<<xml.text().toString().toStdString()<<std::endl;
		break;
		
	    case QXmlStreamReader::StartDocument :
		std::cout<<"inizio del documento"<<std::endl;
		break;
		
	    case QXmlStreamReader::EndDocument : 
		std::cout<<"fine del documento"<<std::endl;
		return true;
		
	    case QXmlStreamReader::DTD : 
		break;
		
		
	    //casi anomali
	    case QXmlStreamReader::NoToken :
		emit error(QString("no token"));
		break;
	    case QXmlStreamReader::Invalid :
		emit error(QString("Token invalido"));
		break;
	    default:
		emit error(QString("token non riconosciuto"));
		break;
	}
	    
    }
    emit error("parse non andato a buon fine");
    
    return false;
}

// bool MapReader::parsePolygon(QXmlStreamReader* xml, function void* () begin, function void* () addPoint, function void* () close)
// bool parsePolygon(QXmlStreamReader &xml, void (MapReader::*)(), void (MapReader::*)(), void (MapReader::*close)())
// bool MapReader::parsePolygon(QXmlStreamReader &xml, void* begin() , void* addPoint(QPointF), void* close())
bool MapReader::parsePolygon(QXmlStreamReader &xml, void (MapReader::*begin)(), void (MapReader::*addPoint)(QPointF), void (MapReader::*close)())
{
    emit (this->*begin)();
    QStringRef polygonType = xml.name();
    QString errorString("formattazione del poligono ");
    errorString.append(polygonType);
    errorString.append(" non corretta: ");
   
    int counter = 0;
    while (true) {
	xml.readNext();
	
	if ((xml.isCharacters() && xml.isWhitespace()) ||
	     xml.isComment())
	    continue;
	
	if(xml.isEndElement() && (xml.name().compare(polygonType) == 0)) {
	    //fine del poligono
	    break;
	}
	
	if(xml.name().compare("point") != 0) {
	    emit error(errorString.append("non è un punto"));
	    return false;
	}
	
	QXmlStreamAttributes attributes = xml.attributes();
	float x = attributes.value("x").toString().toFloat();
	float y = attributes.value("y").toString().toFloat();
	QPointF point(x,y);
	emit (this->*addPoint)(point);
	std::cout<<"Aggiunto un punto a "<<polygonType.toString().toStdString()<<": "<<point.x()<<","<<point.y()<<std::endl;
	
	xml.readNext();
	if (!xml.isEndElement()) {
	    emit error(errorString.append("punto non formattato bene"));
	    return false;
	}
	
	counter++;
    }
    
    if (counter<3) {
	emit error(errorString.append("numero di punti non sufficenti per formare un poligono"));
	return false; 
    }
    emit  (this->*close)();
    return true;
}


bool MapReader::getMap()
{
    return parseStatus;
}

// #include "map/mapreader.moc"
