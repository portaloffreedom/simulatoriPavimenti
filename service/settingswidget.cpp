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


#include "settingswidget.h"
#include <QFormLayout>

SettingsWidget::SettingsWidget(QWidget* parent, Qt::WindowFlags f)
    : QTabWidget()
{
    this->setupUI();
}

void SettingsWidget::setupUI()
{
    this->setupUIGraphicTab();
}

void SettingsWidget::setupUIGraphicTab()
{
    this->graphicTap = new QWidget(this);
    this->addTab(graphicTap,tr("Grafica"));
    QLayout *graficLayout = new QFormLayout(graphicTap);
    graphicTap->setLayout(graficLayout);
    
    antialiasing = new QCheckBox(tr("antialiasing"),graphicTap);
    textantialiasing = new QCheckBox(tr("textantialiasing"),graphicTap);
    highQualityAntialiasing = new QCheckBox(tr("highQualityAntialiasing"),graphicTap);
    smoothPixmapTransform = new QCheckBox(tr("smoothPixmapTransform"),graphicTap);
    nonCosmeticDefaultPen = new QCheckBox(tr("NonCosmeticDefaultPen"),graphicTap);
    
    graficLayout->addWidget(antialiasing);
    graficLayout->addWidget(textantialiasing);
    graficLayout->addWidget(highQualityAntialiasing);
    graficLayout->addWidget(smoothPixmapTransform);
    graficLayout->addWidget(nonCosmeticDefaultPen);
}

QPainter::RenderHints SettingsWidget::getRenderingHint()
{
    QPainter::RenderHints hints= 0;
    if (antialiasing->isChecked())
        hints = hints | QPainter::Antialiasing;
    if (textantialiasing->isChecked())
        hints = hints | QPainter::TextAntialiasing;
    if (highQualityAntialiasing->isChecked())
        hints = hints | QPainter::HighQualityAntialiasing;
    if (smoothPixmapTransform->isChecked())
        hints = hints | QPainter::SmoothPixmapTransform;
    if (nonCosmeticDefaultPen->isChecked())
        hints = hints | QPainter::NonCosmeticDefaultPen;
    return hints;
}



