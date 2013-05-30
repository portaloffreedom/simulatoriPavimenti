/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2013  Matteo De Carlo <email>

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


#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <qtabwidget.h>
#include <QTabWidget>
#include <QPainter>
#include <QCheckBox>
class SettingsWidget;
class TrafficEngine;
#include "../engine/trafficengine.h"


class SettingsWidget : public QTabWidget
{
    Q_OBJECT
private:
    QWidget *graphicTap;
    void setupUI();
    void setupUIGraphicTab();
    
    QCheckBox* antialiasing;
    QCheckBox* textantialiasing;
    QCheckBox* highQualityAntialiasing;
    QCheckBox* smoothPixmapTransform;
    QCheckBox* nonCosmeticDefaultPen;
    QCheckBox* engineDebug;
    
    TrafficEngine *trafficEngine;
    
public:
    explicit SettingsWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);
    /**
     * set to 0 to unbound the settings widget to the any trafficEngine
     **/ 
    void setTrafficEngine(TrafficEngine *trafficEngine);
    
    QPainter::RenderHints getRenderingHint();
};

#endif // SETTINGSWIDGET_H
