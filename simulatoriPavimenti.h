#ifndef simulatoriPavimenti_H
#define simulatoriPavimenti_H

#include <QtGui/QMainWindow>

class simulatoriPavimenti : public QMainWindow
{
Q_OBJECT
public:
    simulatoriPavimenti();
    virtual ~simulatoriPavimenti();
    
public slots:
    void open();


private:
    void createActions();
    void createMenus();

    QMenu *fileMenu;
    QAction *openAct;
    QAction *exitAct;

};

#endif // simulatoriPavimenti_H
