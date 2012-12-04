#ifndef simulatoriPavimenti_H
#define simulatoriPavimenti_H

#include <QtGui/QMainWindow>
#include "map/map.h"

class simulatoriPavimenti : public QMainWindow
{
Q_OBJECT
public:
    simulatoriPavimenti();
    virtual ~simulatoriPavimenti();
    
public slots:
    void open();
    void getMap(Map* );
    void printError(QString,QString);


private:
    void createActions();
    void createMenus();

    QMenu *fileMenu;
    QAction *openAct;
    QAction *exitAct;
    
    QWidget *map;

};

#endif // simulatoriPavimenti_H
