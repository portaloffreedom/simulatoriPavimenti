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

/**
 * Parses the next token ignoring comments and characters.
 * emitting errors is there's some unexpected token (NoToken or Invalid).
 */
QXmlStreamReader::TokenType MapReader::nextToken()
{
    if (xml.atEnd()) {
        emit error("End of file!","The file ended too early");
        return QXmlStreamReader::Invalid;
    }
    if (xml.error()) {
        emit error("Generic Error",xml.errorString());
        return QXmlStreamReader::Invalid;
    }

    while (true) {

        QXmlStreamReader::TokenType token = xml.readNext();
        if (xml.error()) {
            emit error("Generic Error",xml.errorString());
            return token;
        }

        switch (token) {
        case QXmlStreamReader::Comment :
            std::cout<<xml.text().toString().toStdString()<<std::endl;
            break; //parse the next token

        case QXmlStreamReader::Characters :
            if (xml.isWhitespace())
                std::cout<<"whitespaces"<<std::endl;
            else
                std::cout<<xml.text().toString().toStdString()<<std::endl;
            break; //parse the next token

        case QXmlStreamReader::NoToken :
            emit error("No Token","unexpected error: next token is a \"NoToken\"");
            return token;
        case QXmlStreamReader::Invalid :
            emit error("Invalid Token","unexpected error: next token is a \"Invalid\"");
            return token;

        default:
            return token;
        }
    }

}

bool MapReader::parse()
{
    xml.setDevice(xmlFile);
    QXmlStreamReader::TokenType token;

    // Start Document
    this->nextToken();
    if (!xml.isStartDocument()) {
        QString errorString = QString("There should be a Start Document here!");
        QString errorDetailString = QString("Instead i've found: ").append(xml.dtdName());
        emit error(errorString,errorDetailString);
        return false;
    }

    // DTD
    this->nextToken();
    if (!xml.isDTD()) {
        QString errorString = QString("There sconfigurazionehould be a DTD (document type) here!");
        QString errorDetailString = QString("Instead i've found: ").append(xml.dtdName());
        emit error(errorString,errorDetailString);
        return false;
    }

    if (xml.dtdName().compare("floormap") != 0) {
        QString errorString = QString("The document type isn't the one expected");
        QString errorDetailString = QString("floormap is expected. Instead i've found: ").append(xml.dtdName());
        emit error(errorString,errorDetailString);
        return false;
    }

    //  map
    this->nextToken();
    if (!xml.isStartElement() || xml.name().compare("map") != 0) {
        QString errorString = QString("The document isn't well formatted");
        QString errorDetailString = QString("the starting element \"map\" is expected here. Instead I've found: ");
        errorDetailString.append(xml.tokenString());
        errorDetailString.append(" named: ").append(xml.name());
        emit error(errorString,errorDetailString);
        return false;
    }

    // map version
    QXmlStreamAttributes attributes = xml.attributes();
    QStringRef version = attributes.value("version");
    if (!attributes.hasAttribute("version") || version.compare(Map::version) != 0) {
        QString errorString = QString("The document has an unsupported version");
	QString errorDetailString = QString("the version supported is \"");
	errorDetailString.append(Map::version);
	errorDetailString.append("\" instead I've found: \"").append(version).append('"');
	emit error(errorString,errorDetailString);
	return false;
    }
    
    //floorimage and sensorTypes
    this->nextToken();
    
    while(!xml.isEndElement()) {
	
    }
    
    if (xml.name().compare("map") != 0) {
	//something got really wrong! probably the document isn't well formatted
	
    }

    std::cout<<"everithing got it right!"<<std::endl;
    return false;

//     this->

}

bool MapReader::parse2()
{
    xml.setDevice(xmlFile);
//     xml.tokenType();

    //parse all the elements
    for (int i=0; i < 5000; i++) {
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
                parsePolygon(&MapReader::addBorderPolygon,&MapReader::addBorderPolygonPoint);
                emit closeBorder();
                break;
            }
            if (xml.name().compare("entrance") == 0) {
                emit beginEntrance("","en1");
                parsePolygon(&MapReader::addEntrancePolygon,&MapReader::addEntrancePolygonPoint);
                emit closeEntrace();
                break;
            }
            if (xml.name().compare("exit") == 0) {
                emit beginExit("","ex1");
                parsePolygon(&MapReader::addExitPolygon,&MapReader::addExitPolygonPoint);
                emit closeExit();
                break;
            }
            if (xml.name().compare("obstacle") == 0) {
                emit beginObstacle("","o1");
                parsePolygon(&MapReader::addObstaclePolygon,&MapReader::addObstaclePolygonPoint);
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
            std::cout<<xml.dtdName().toString().toStdString()<<std::endl;
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

bool MapReader::parsePolygon(void (MapReader::*addPolygon)(), void (MapReader::*addPolygonPoint)(QPointF))
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

void MapReader::error(QString errorString)
{
    emit error("Generic Error",errorString);
    return;
}


// #include "map/mapreader.moc"
