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

#include "loggerstartwidget.h"
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>
#include <QFormLayout>
#include <QMessageBox>
// #include <iostream>

const int timeDurationMax = 3600;
const int frequencyMax = 100000;

LoggerStartWidget::LoggerStartWidget(QWidget* parent) : QWidget(parent)
{
    this->setWindowTitle(tr("Logger Settings"));
    this->setupGraphics();
}

LoggerStartWidget::~LoggerStartWidget()
{
    
}

void LoggerStartWidget::setupGraphics()
{
    //Set widgets
    QLabel *graphicLabel     = new QLabel(tr("Graphic output"),this);
    QLabel *timeLabel        = new QLabel(tr("Time Duration (sec)"),this);
    QLabel *frequencyLabel   = new QLabel(tr("Frequency (Hz)"),this);
    QPushButton *selectPathB = new QPushButton(tr("Select Save Path"),this);
    
    graphicCheck = new QCheckBox(this);
    timeDuration = new QSpinBox(this);
    timeDuration->setMinimum(1);
    timeDuration->setMaximum(timeDurationMax);
    frequency    = new QSpinBox(this);
    frequency->setMinimum(1);
    frequency->setMaximum(frequencyMax);
    savePath     = new QLineEdit(tr("/path/to/folder/save"),this);
    QPushButton *startButton = new QPushButton(tr("Start"),this);
    
    //connects
    connect(selectPathB,SIGNAL(clicked()),this,SLOT(selectPath()));
    connect(startButton,SIGNAL(clicked()),this,SLOT(start()));
    
    //Set Layouts
    QFormLayout *layout = new QFormLayout(this);
    layout->addRow(graphicLabel,graphicCheck);
    layout->addRow(timeLabel,timeDuration);
    layout->addRow(frequencyLabel,frequency);
    layout->addRow(selectPathB,savePath);
    layout->addRow(startButton);
    this->setLayout(layout);
    
}

void LoggerStartWidget::selectPath()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);
    if (dialog.exec())
        savePath->setText(dialog.selectedFiles().first());
}

void LoggerStartWidget::start()
{    
    Logger *logger = new Logger(this->savePath->text());
//     std::cout<<"#@2#_data_analysis; Frequency; Duration;"<<std::endl;
//     std::cout<<"#@2#_data_analysis; "<<frequency->value()<<"; "<<timeDuration->value()<<std::endl;
//     std::cout<<std::scientific;
    emit loggerStart(logger,frequency->value(),timeDuration->value(),graphicCheck->isChecked());
}
