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
    
    connect(this,SIGNAL(beginBorder(QString,QString))  ,map,SLOT(beginBorder(QString,QString))  );
    connect(this,SIGNAL(addBorderPolygon())            ,map,SLOT(addBorderPolygon())            );
    connect(this,SIGNAL(addBorderPolygonPoint(QPointF)),map,SLOT(addBorderPolygonPoint(QPointF)));
    connect(this,SIGNAL(addBorderCircle(QPointF,qreal)),map,SLOT(addBorderCircle(QPointF,qreal)));
    connect(this,SIGNAL(closeBorder())                 ,map,SLOT(closeBorder())                 );
    
    connect(this,SIGNAL(beginEntrance(QString,QString))  ,map,SLOT(beginEntrance(QString,QString))  );
    connect(this,SIGNAL(addEntrancePolygon())            ,map,SLOT(addEntrancePolygon())            );
    connect(this,SIGNAL(addEntrancePolygonPoint(QPointF)),map,SLOT(addEntrancePolygonPoint(QPointF)));
    connect(this,SIGNAL(addEntranceCircle(QPointF,qreal)),map,SLOT(addEntranceCircle(QPointF,qreal)));
    connect(this,SIGNAL(closeEntrace())                  ,map,SLOT(closeEntrace())                  );
    
    connect(this,SIGNAL(beginExit(QString,QString))  ,map,SLOT(beginExit(QString,QString))  );
    connect(this,SIGNAL(addExitPolygon())            ,map,SLOT(addExitPolygon())            );
    connect(this,SIGNAL(addExitPolygonPoint(QPointF)),map,SLOT(addExitPolygonPoint(QPointF)));
    connect(this,SIGNAL(addExitCircle(QPointF,qreal)),map,SLOT(addExitCircle(QPointF,qreal)));
    connect(this,SIGNAL(closeExit())                 ,map,SLOT(closeExit())                 );
    
    connect(this,SIGNAL(beginObstacle(QString,QString))  ,map,SLOT(beginObstacle(QString,QString))  );
    connect(this,SIGNAL(addObstaclePolygon())            ,map,SLOT(addObstaclePolygon())            );
    connect(this,SIGNAL(addObstaclePolygonPoint(QPointF)),map,SLOT(addObstaclePolygonPoint(QPointF)));
    connect(this,SIGNAL(addObstacleCircle(QPointF,qreal)),map,SLOT(addObstacleCircle(QPointF,qreal)));
    connect(this,SIGNAL(closeObstacle())                 ,map,SLOT(closeObstacle())                 );
    
    
//     parseStatus = this->parse();
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
		    emit beginBorder("","b1");
		    parsePolygon(xml,&MapReader::addBorderPolygon,&MapReader::addBorderPolygonPoint);
		    emit closeBorder();
		    break;
		}
		if (xml.name().compare("entrance") == 0) {
		    emit beginEntrance("","en1");
		    parsePolygon(xml,&MapReader::addEntrancePolygon,&MapReader::addEntrancePolygonPoint);
		    emit closeEntrace();
		    break;
		}
		if (xml.name().compare("exit") == 0) {
		    emit beginExit("","ex1");
		    parsePolygon(xml,&MapReader::addExitPolygon,&MapReader::addExitPolygonPoint);
		    emit closeExit();
		    break;
		}
		if (xml.name().compare("obstacle") == 0) {
		    emit beginObstacle("","o1");
		    parsePolygon(xml,&MapReader::addObstaclePolygon,&MapReader::addObstaclePolygonPoint);
		    emit closeObstacle();
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
		emit finished(this->map);
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

bool MapReader::parsePolygon(QXmlStreamReader &xml, void (MapReader::*addPolygon)(), void (MapReader::*addPolygonPoint)(QPointF))
{
    emit (this->*addPolygon)();
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
	emit (this->*addPolygonPoint)(point);
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
    return true;
}


bool MapReader::getMap()
{
    return parseStatus;
}

// #include "map/mapreader.moc"
