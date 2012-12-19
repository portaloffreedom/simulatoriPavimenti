#ifndef simulatoriPavimenti_H
#define simulatoriPavimenti_H

#include <QtGui/QMainWindow>
#include <QPushButton>
#include "map/map.h"
#include "engine/trafficengine.h"

class SimulatoriPavimenti : public QMainWindow
{
    Q_OBJECT

private:
    void createActions();
    void createMenus();

    QMenu *fileMenu;
    QAction *openAct;
    QAction *exitAct;
    QPushButton *stepButton;

    Map *map;
    TrafficEngine *engine;
    


public:
    SimulatoriPavimenti();
    virtual ~SimulatoriPavimenti();

public slots:
    void open();
    void getMap(Map* );
    void printError(QString,QString);
};

#endif // simulatoriPavimenti_H
