/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2012  Matteo De Carlo <<matteo.dek@gmail.com>>

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
#include "../engine/groundsensor.h"
#include <QXmlStreamReader>
#include <iostream>
#include <QString>

MapReader::MapReader(QFile &xmlFile, SettingsWidget * settingswidget)
{
    this->xmlFile = &xmlFile;
    map = new Map(settingswidget);
    //groundEngine = new GroundEngine(map);

    connect(this,SIGNAL(beginBorder(QString,QString))  ,map,SLOT(beginBorder(QString,QString))  );
    connect(this,SIGNAL(addBorderPolygon())            ,map,SLOT(addBorderPolygon())            );
    connect(this,SIGNAL(addBorderPolygonPoint(QPointF)),map,SLOT(addBorderPolygonPoint(QPointF)));
    connect(this,SIGNAL(addBorderCircle(QPointF,smReal)),map,SLOT(addBorderCircle(QPointF,smReal)));
    connect(this,SIGNAL(closeBorder())                 ,map,SLOT(closeBorder())                 );

    connect(this,SIGNAL(beginEntrance(QString,QString))  ,map,SLOT(beginEntrance(QString,QString))  );
    connect(this,SIGNAL(addEntrancePolygon())            ,map,SLOT(addEntrancePolygon())            );
    connect(this,SIGNAL(addEntrancePolygonPoint(QPointF)),map,SLOT(addEntrancePolygonPoint(QPointF)));
    connect(this,SIGNAL(addEntranceCircle(QPointF,smReal)),map,SLOT(addEntranceCircle(QPointF,smReal)));
    connect(this,SIGNAL(closeEntrance())                  ,map,SLOT(closeEntrance())                );

    connect(this,SIGNAL(beginExit(QString,QString))  ,map,SLOT(beginExit(QString,QString))  );
    connect(this,SIGNAL(addExitPolygon())            ,map,SLOT(addExitPolygon())            );
    connect(this,SIGNAL(addExitPolygonPoint(QPointF)),map,SLOT(addExitPolygonPoint(QPointF)));
    connect(this,SIGNAL(addExitCircle(QPointF,smReal)),map,SLOT(addExitCircle(QPointF,smReal)));
    connect(this,SIGNAL(closeExit())                 ,map,SLOT(closeExit())                 );

    connect(this,SIGNAL(beginObstacle(QString,QString))  ,map,SLOT(beginObstacle(QString,QString))  );
    connect(this,SIGNAL(addObstaclePolygon())            ,map,SLOT(addObstaclePolygon())            );
    connect(this,SIGNAL(addObstaclePolygonPoint(QPointF)),map,SLOT(addObstaclePolygonPoint(QPointF)));
    connect(this,SIGNAL(addObstacleCircle(QPointF,smReal)),map,SLOT(addObstacleCircle(QPointF,smReal)));
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
            MAPREADER_DEBUG_COUT(xml.text().toString().toStdString())
            break; //parse the next token

        case QXmlStreamReader::Characters :
#ifdef MAPREADER_DEBUG
            if (xml.isWhitespace())
                MAPREADER_DEBUG_COUT("whitespaces")
            else
                MAPREADER_DEBUG_COUT(xml.text().toString().toStdString())
#endif
            break; //parse the next token

        case QXmlStreamReader::NoToken :
            emit error("No Token","unexpected error: next token is a \"NoToken\"");
            return token;
        case QXmlStreamReader::Invalid :
            emit error("Invalid Token","unexpected error: next token is a \"Invalid\"");
            return token;

        default:
#ifdef MAPREADER_DEBUG
            QString tokenString = xml.tokenString();
            tokenString.append(" -> ").append(xml.name());
            MAPREADER_DEBUG_COUT(tokenString.toStdString())
#endif
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
    
    // floorImage and sensorTypes
    this->nextToken();
    while(xml.isStartElement()) {
        if (xml.name().compare("floorImage") == 0) { //TODO at least one
            if (this->parseFloorImage()) {
                this->nextToken();
                continue;
            }
            else
                return false;
        }
        if (xml.name().compare("sensorTypes") == 0) { //TODO maximum one
            if (this->parseSensorTypes()) {
                this->nextToken();
                continue;
            }
            else
                return false;
        }
        // Something got wrong: only floorImage or sensorTypes are expected
        QString errorString = QString("Something got wrong");
        QString errorDetailString = QString("Only floorImage or sensorTypes are expected.\nInstead I've found: ");
        errorDetailString.append(xml.name());
        emit error(errorString,errorDetailString);
        return false;
    }
    
    if (!controlNotEndElement("map"))
        return false;

    MAPREADER_DEBUG_COUT("everything got it right")
    emit finished(this->map);
    return true;

}

bool MapReader::parseFloorImage()
{
    bool sensorParsed=false, roomParsed=false;
    for (int i = 0; i<2; i++) {
        this->nextToken();
        if (!xml.isStartElement()) {
            QString errorString = QString("Something got wrong");
            QString errorDetailString = QString("Probably the document isn't well formatted.\nI should have find startElement."
                                                "Instead I've found: ");
            errorDetailString.append(xml.tokenString());
            emit error(errorString,errorDetailString);
            return false;
        }
        if ( (!roomParsed) && (xml.name().compare("room") == 0) ) {
            if (!this->parseRoom())
                return false;
            roomParsed = true;
            continue;
        }
        if ( (!sensorParsed) && (xml.name().compare("sensors") == 0) ) {
            if (!this->parseSensors())
                return false;
            sensorParsed = true;
            continue;
        }
        
        QString errorString = QString("Something got wrong");
        QString errorDetailString = QString("Probably the document isn't well formatted.\nI should have find \"room\" or \"sensors\" elements."
                                            "Instead I've found: ");
        errorDetailString.append(xml.name());
        errorDetailString.append("Or maybe there is more than one \"room\" or \"sensors\" element (only one allowed for each one)");
        emit error(errorString,errorDetailString);
        return false;
    }

    this->nextToken();
    if (!controlNotEndElement("floorImage"))
        return false;

    return true;
}

/**
 * Funzione che si occupa di fare il parsing dell'elemento <room>
 * 
 * Possiede:
 * - almeno un <border>: limiti della stanza.
 * - almeno un <entrance>: zona in cui nuove entità mobili possono apparire
 * - almeno un <exit>: zona in cui le entità mobili possono sparire
 * - nessuno, uno o tanti <obstacle>: ostacolo fisso non intersecabile con le entità mobili.
 */
bool MapReader::parseRoom()
{
    bool borderParsed = false;
    bool entranceParsed = false;
    bool exitParsed = false;
    
    this->nextToken();
    while(xml.isStartElement()) {
        if ( xml.name().compare("border") == 0) {
            if (!this->parseFigure(&MapReader::beginBorder,
                                   &MapReader::addBorderPolygon,
                                   &MapReader::addBorderPolygonPoint,
                                   &MapReader::addBorderCircle,
                                   &MapReader::closeBorder ))
                return false;
            borderParsed = true;
            this->nextToken();
            continue;
        }
        if ( xml.name().compare("entrance") == 0) {
            if (!this->parseFigure(&MapReader::beginEntrance,
                                   &MapReader::addEntrancePolygon,
                                   &MapReader::addEntrancePolygonPoint,
                                   &MapReader::addEntranceCircle,
                                   &MapReader::closeEntrance ))
                return false;
            entranceParsed = true;
            this->nextToken();
            continue;
        }
        if ( xml.name().compare("exit") == 0) {
            if (!this->parseFigure(&MapReader::beginExit,
                                   &MapReader::addExitPolygon,
                                   &MapReader::addExitPolygonPoint,
                                   &MapReader::addExitCircle,
                                   &MapReader::closeExit ))
                return false;
            exitParsed = true;
            this->nextToken();
            continue;
        }
        if ( xml.name().compare("obstacle") == 0) {
            if (!this->parseFigure(&MapReader::beginObstacle,
                                   &MapReader::addObstaclePolygon,
                                   &MapReader::addObstaclePolygonPoint,
                                   &MapReader::addObstacleCircle,
                                   &MapReader::closeObstacle ))
                return false;
            this->nextToken();
            continue;
        }

        //TODO emit error
        emit error("ciccia","pappa");
        return false;
    }
    
    if (!controlNotEndElement("room"))
        return false;

    if ( !borderParsed || !entranceParsed || !exitParsed ) {
        QString errorString = QString("Not all elements necessary found.");
        QString errorDetailString = QString("There must be at least one Border, one Entrace and one Exit");
        emit error(errorString,errorDetailString);
        return false;
    }

    return true;

}

bool MapReader::parseFigure(void (MapReader::*begin)(const QString description, const QString id),
                            void (MapReader::*addPolygon)(),
                            void (MapReader::*addPolygonPoint)(QPointF),
                            void (MapReader::*addCircle)(QPointF center, smReal radius),
                            void (MapReader::*close)())
{
    QXmlStreamAttributes attributes = xml.attributes();
    const QString description = attributes.value("description").toString();
    const QString id = attributes.value("id").toString();

    emit (this->*begin)(description,id);
    QStringRef figureName = xml.name();

    int i;
    this->nextToken();
    for (i=0; xml.isStartElement(); i++) {
        if ( xml.name().compare("polygon") == 0) {
            if (!this->parsePolygon(addPolygon,addPolygonPoint))
                return false;
            this->nextToken();
            continue;
        }
        if ( xml.name().compare("circle") == 0) {
            if (!this->parseCircle(addCircle))
                return false;
            this->nextToken();
            continue;
        }
        // Only polygons or circles allowed
        //TODO emit error
        emit error("ciccia","pappa");
        return false;
    }

    if (!controlNotEndElement(figureName.toString()))
        return false;

    if (i==0) {
        // There should be at least one polygon or circle
        emit error("Error parsing figure","There should be at least "
                   "one polygon or circle. None found");
        return false;
    }
    
    emit (this->*close)();
    return true;
}

bool MapReader::parseSensors()
{
    while ( (!xml.isEndElement()) || (xml.name().compare("sensors"))!=0 ) {
        if (xml.name().compare("sensor") == 0) {
            sensorsPrototype sP;
            QXmlStreamAttributes attributes = xml.attributes();
            sP.cx = attributes.value("cx").toString().toFloat();
            sP.cy = attributes.value("cy").toString().toFloat();
            sP.angle = attributes.value("angle").toString().toFloat();
            sP.type = attributes.value("typeid").toString();
            
            this->sensorsPrototypes.append(sP);
            this->nextToken();
            controlNotEndElement("sensor");
        }
        this->nextToken();
    }
    return true;
}


    
// <type typeid="standard" placement="grid" rows="8" cols="8" margin="100,100,100,100" padding="200,200"/>
bool MapReader::parseSensorTypes()
{
    QMap<QString,GroundSensor*> sensorTypeMap;
    
    while ( (!xml.isEndElement()) || (xml.name().compare("sensorTypes"))!=0 ) {
        if (xml.name().compare("type") == 0) {
            QXmlStreamAttributes attributes = xml.attributes();
            QString placement = attributes.value("placement").toString();
            int rows = attributes.value("rows").toString().toInt();
            int cols = attributes.value("cols").toString().toInt();
            
            QStringList margin = attributes.value("margin").toString().split(',');
            smReal marginUp    = margin[0].toFloat();
            smReal marginDown  = margin[1].toFloat();
            smReal marginLeft  = margin[2].toFloat();
            smReal marginRight = margin[3].toFloat();
            
            QStringList padding = attributes.value("padding").toString().split(',');
            smReal paddingX = padding[0].toFloat();
            smReal paddingY = padding[1].toFloat();
            
            
            QString id = attributes.value("typeid").toString();
            //TODO implement other sensors
            if (id.compare("standard") != 0)
                emit error ("Errore nel parsing dell'xml","Per adesso solo il sensore standard è implementato");
            
            GroundSensor *sensor = new GroundSensor(0,0,0,rows,cols,marginUp,marginDown,marginLeft,marginRight,paddingX,paddingY);
            sensorTypeMap[id] = sensor;
            
            this->nextToken();
            controlNotEndElement("type");
        }
        this->nextToken();
    }
    
    foreach( sensorsPrototype sProto, this->sensorsPrototypes )  {
        GroundSensor *sensorType = sensorTypeMap[sProto.type];
        GroundSensor *newSensor = sensorType->clone(sProto.cx,sProto.cy,sProto.angle);
        std::cout<<newSensor->toString().toStdString()<<std::endl;
        this->map->addSensor(newSensor);
    }
    foreach( GroundSensor *s, sensorTypeMap)
        delete s;
    
    //TODO create real sensors
    return true;
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
                emit closeEntrance();
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
    QString errorString("formattazione del poligono non corretta: ");

    int counter = 0;
    while (true) {
        this->nextToken();

        if(!xml.isStartElement()) {
            if (!controlNotEndElement("polygon"))
                return false;
            else
                break;
        }

        if(xml.name().compare("point") != 0) {
            emit error(errorString,"non è un punto");
            return false;
        }

        QXmlStreamAttributes attributes = xml.attributes();
        float x = attributes.value("x").toString().toFloat();
        float y = attributes.value("y").toString().toFloat();
        QPointF point(x,y);
        emit (this->*addPolygonPoint)(point);
#ifdef MAPREADER_DEBUG
        QString trace_string("Aggiunto un punto a polygon: ");
        trace_string.append(QString::number(point.x())).append(',').append(QString::number(point.y()));
        MAPREADER_DEBUG_COUT(trace_string.toStdString())
        
#endif

        this->nextToken();
        if (!controlNotEndElement("point"))
            return false;
        
        counter++;
    }
    
    if (counter < 3) {
        emit error(errorString.append("numero di punti non sufficenti per formare un poligono"));
        return false;
    }
    return true;
}

bool MapReader::parseCircle(void (MapReader::*addCircle)(QPointF center, smReal radius))
{
    QXmlStreamAttributes attributes = xml.attributes();
    smReal cx = attributes.value("cx").toString().toDouble();
    smReal cy = attributes.value("cy").toString().toDouble();
    smReal radius = attributes.value("radius").toString().toDouble();
    
    QPointF center(cx,cy);
    
    emit (this->*addCircle)(center,radius);

    this->nextToken();
    if (!controlNotEndElement("circle"))
        return false;

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


bool MapReader::controlNotEndElement(QString element)
{
    if ( (!xml.isEndElement()) || (xml.name().compare(element) != 0) ) {
        //something got really wrong! probably the document isn't well formatted
        QString errorString = QString("Something got really wrong");
        QString errorDetailString = QString("Probably the document isn't well formatted."
                                            "\nI should have found \"");
        errorDetailString.append(element).append("\" endElement. Instead I've found: \"");
        errorDetailString.append(xml.name()).append("\" ").append(xml.tokenString());
        emit error(errorString,errorDetailString);
        return false;
    }
    return true;
}



// #include "map/mapreader.moc"
