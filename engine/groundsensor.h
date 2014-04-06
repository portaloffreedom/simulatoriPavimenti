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

#ifndef GROUNDSENSOR_H
#define GROUNDSENSOR_H
#include <qstring.h>
#include <QPointF>
#include <QLineF>
#include <QPainter>
#include "../service/simulatorePavimentiTypes.h"

class GroundSensor
{
public:
    GroundSensor(smReal cx, smReal cy, smReal angle,
                 int rows, int cols,
                 smReal marginUp, smReal marginDown,
                 smReal marginLeft, smReal marginRight,
                 smReal paddingX, smReal paddingY);

    ~GroundSensor();

    virtual QString getType() const;
    virtual smReal getCX() const {return cx;}
    virtual smReal getCY() const {return cy;}
    virtual smReal getAngle() const {return angle;}
    virtual unsigned int getDim() const {return rows*cols;}
    virtual QPointF realWorldSensorPos(int sensorNumber) const {
        return realWorldPosition(reverseMatrixPosition(sensorNumber));
    }
    virtual smReal sensorStatus(int sensorNumber) const {
        return matrix[sensorNumber];
    }
    
//     virtual smReal getMarginUp() const;
//     virtual smReal getMarginDown() const;
//     virtual smReal getMarginLeft() const;
//     virtual smReal getMarginRight() const;
//     virtual smReal getPaddingX() const;
//     virtual smReal getPaddingY() const;
    

    virtual void hit(const QPointF& hitPoint, smReal weight = 1.0);
    virtual void hit(const QLineF& hitLine, smReal radius, smReal weight = 1.0) {
      qWarning("groundSensor.hit(line) ANCORA DA IMPlEMENTARE!");  
    };
    virtual void draw(QPainter &p);
    
    virtual GroundSensor *clone(smReal cx, smReal cy, smReal angle);
    
    
    QString toString() {
        return QString()+"sensore typeid("+type+")"
            " cx("+QString::number(cx)+")"
            " cy("+QString::number(cy)+")"
            " angle("+QString::number(angle)+")"
            " rows=("+QString::number(rows)+")"
            " cols=("+QString::number(cols)+")"
            " margin("+QString::number(marginUp)+','+QString::number(marginDown)+','+QString::number(marginLeft)+','+QString::number(marginRight)+")"
            " padding("+QString::number(paddingX)+','+QString::number(paddingY)+')';
    }
    
    virtual void reset() { resetMatrix();}

private:
    QString type;
    const smReal maxVal;
    const smReal minVal;

    const unsigned int rows, cols;
    smReal marginUp, marginDown;
    smReal marginLeft, marginRight;
    smReal paddingX, paddingY;
    smReal dimX, dimY;

    smReal cx, cy;
    smReal upperX,upperY;
    smReal angle;

    smReal *matrix;

    void resetMatrix();

    void hitSingle(int x, int y, const QPointF& hitPoint, smReal weight);

    int matrixPosition(int x,int y) const {
        return x*rows+y;
    }
    QPoint reverseMatrixPosition(int pos) const{
        int x = pos / rows;
        int y = pos - x;
    }
    
    QPointF realWorldPosition(const QPoint& p) const {
        return realWorldPosition(p.x(), p.y());
    }
    
    QPointF realWorldPosition(int x, int y) const {
        QPointF p(
            realWorldPositionX(x),
            realWorldPositionY(y)
        );
        return p;
    }
    
    smReal realWorldPositionY(int y) const {
        return upperY+marginDown+(y*paddingY);
    }
    
    smReal realWorldPositionX(int x) const {
        return upperX+marginLeft+(x*paddingX);
    }
    
    smReal scaleHit(smReal input) const {
        return input*(maxVal-minVal) + minVal;
    }
};

#endif // GROUNDSENSOR_H
