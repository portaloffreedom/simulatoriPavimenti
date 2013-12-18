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

#ifndef LOGGERSTARTWIDGET_H
#define LOGGERSTARTWIDGET_H

#include <qt4/QtGui/QWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QCheckBox>
#include "logger.h"

class LoggerStartWidget : public QWidget
{
Q_OBJECT

private:
    QLineEdit* savePath;
    QSpinBox* frequency;
    QSpinBox* timeDuration;
    QCheckBox* graphicCheck;
    
public:
    LoggerStartWidget(QWidget *parent = nullptr);
    ~LoggerStartWidget();
signals:
    /** the logger is created in the heap, is set to the programmer to delete it */
    void loggerStart(Logger*,smReal frequency,smReal timeDuration,bool graphicCheck);
    
private:
    
    void setupGraphics();
    
private slots:
    void start();
    void selectPath();
};

#endif // LOGGERSTARTWIDGET_H
