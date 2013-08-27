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

#include "groundsensor.h"
#include "../service/mathservice.h"

/**
 * Constructor
 * 
 * @param cx X axis of the center of the sensor
 * @param cy Y axis of the center of the sensor
 * @param angle inclination of the sensor
 * 
 * @param rows number of rows of sensors in the sensors matrix
 * @param cols number of columns of sensors in the sensors matrix
 * 
 * @param marginUp the upper marigin before you encounter the first sensor of the matrix
 * @param marginDown the lower marigin before you encounter the first sensor of the matrix
 * @param marginLeft the left marigin before you encounter the first sensor of the matrix
 * @param marginRight the right marigin before you encounter the first sensor of the matrix
 * 
 * @param paddingX the X padding betweeen the sensors
 * @param paddingY the Y padding betweeen the sensors
 */
GroundSensor::GroundSensor( smReal cx, smReal cy, smReal angle, int rows, int cols, smReal marginUp, smReal marginDown, smReal marginLeft, smReal marginRight, smReal paddingX, smReal paddingY )
    : type("standard"), maxVal(255), minVal(0),
    cx(cx),cy(cy),angle(angle),
    rows(rows),cols(cols),
    marginDown( marginDown ),marginUp   ( marginUp ),
    marginLeft( marginLeft ),marginRight( marginRight ),
    paddingX( paddingX ),paddingY( paddingY )
{
    this->dimX = cols*paddingX + marginLeft + marginRight;
    this->dimY = rows*paddingY + marginUp + marginDown;
 
    this->upperX = (cx-dimX/2);
    this->upperY = (cy-dimY/2);
    
    this->matrix = new smReal[cols*rows];
    resetMatrix();
}


GroundSensor::~GroundSensor()
{
    delete[] matrix;
}

/**
 * Reset the matrix with the untouched values
 */
void GroundSensor::resetMatrix()
//TODO settare a valori casuali
{   
    //memset(this->matrix, 0, cols*rows);   
    for (int x=0; x<rows*cols; x++)
        this->matrix[x] = 0.0;
}

/**
 * Returns the string rappresentation of the sensor Type
 */
QString GroundSensor::getType()
{
    return this->type;
}

/**
 * Calculates the hits for a single Point on the sensors.
 */
void GroundSensor::hit(const QPointF& hitPoint, smReal weight)
{
    for (int x=0; x<cols; x++) {
        for (int y=0; y<rows; y++) {
            hitSingle(x,y,hitPoint,weight);
        }
    }
}

/**
 * Private internal method
 * 
 * Calculates the hit for a single spot of the sensor on a single Point.
 * 
 */
void GroundSensor::hitSingle(int x, int y, const QPointF &hitPoint, smReal weight)
{
    smReal hitValue = 0;
    QPointF sensorPosition = realWorldPosition(x,y);
    QLineF distance(hitPoint,sensorPosition);
    smReal weightedDistance = weight * smMath::normalCurve(distance.length());
    
    int pos = matrixPosition(x,y);
    matrix[pos] += scaleHit(weightedDistance);
    
    if (matrix[pos] > maxVal)
        matrix[pos] = maxVal;
}

/**
 * Draws a rappresentation of the sensor with the painter
 */
void GroundSensor::draw(QPainter& p)
{
    p.save();
    p.setPen(Qt::transparent);
    QColor color = QColor(255,255,255,255);
    color.setHsl(0,0,255);
    //color.setAlphaF(0.2);
    smReal pointDimX = paddingX/5;
    smReal pointDimY = paddingY/5;
    
    for (int x=0; x<cols; x++) {
        for (int y=0; y<rows; y++) {
            smReal weight = matrix[matrixPosition(x,y)];
            color.setHsv(0,weight,255);
            p.setBrush(color);
            QPointF center = realWorldPosition(x,y);
            p.drawEllipse(center,pointDimX,pointDimY);
        }
    }
    
    p.restore();
}

/*
GroundSensor GroundSensor::clone ( smReal cx, smReal cy, smReal angle )
{
    GroundSensor newSensor(cx,cy,angle,
        rows,cols,marginUp,marginDown,marginLeft,marginRight,paddingX,paddingY);
    
    return newSensor;
    
}*/

/**
 * Clone the GroundSensor with cx,cy and angle values changed. It's created with
 * new so the object must be deleted manually elsewhere
 */
GroundSensor *GroundSensor::clone ( smReal cx, smReal cy, smReal angle )
{
    return new GroundSensor(cx,cy,angle,
        rows,cols,marginUp,marginDown,marginLeft,marginRight,paddingX,paddingY);
}
