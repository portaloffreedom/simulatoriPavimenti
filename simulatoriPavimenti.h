#ifndef simulatoriPavimenti_H
#define simulatoriPavimenti_H

#include <QApplication>
#include <QtGui/QMainWindow>
#include <QDockWidget>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSlider>
#include <QPushButton>
#include <QVBoxLayout>
#include <QSlider>
#include "map/map.h"
#include "engine/trafficengine.h"
#include "engine/groundengine.h"
#include "../service/simulatorePavimentiTypes.h"
#include "../service/settingswidget.h"

class SimulatoriPavimenti : public QMainWindow
{
    Q_OBJECT

private:
    void createWidgets();
    void setDockWidgets();
    void setLayouts();
    void createActions();
    void createMenus();
    
    void setMapWidget(Map* map);
    void resetButtons();

    QMenu *fileMenu;
    QAction *openAct;
    QAction *exitAct;
    
    QMenu* viewMenu;
    QAction* showLogDockAct;
    QAction* showBehaviourDockAct;

    QMenu* settingsMenu;
    QAction* preferencesAct;
    SettingsWidget* settingswidget;
    
    QMenu* aboutMenu;
    QAction* aboutAct;
    QAction* aboutQtAct;

    Map *map;
    TrafficEngine *engine;

    
    QVBoxLayout* mainVerticalLayout;
    QSlider* timeSlider;
    QPushButton* playButton;
    QPushButton *stepButton;
    QSlider* speedSlider;
    
    QDockWidget* behaviourDockWidget;
    QVBoxLayout* behaviourLayout;
    QPushButton* newBehaviourButton;
    QDockWidget* logDockWidget;
    

protected:
    virtual void closeEvent(QCloseEvent* );    

signals:
    void quitting();

public:
    SimulatoriPavimenti(QWidget *parent = 0);
    virtual ~SimulatoriPavimenti();

private slots:
    void addNewBehaviour();
    
public slots:
    void open();
    void parsingFinished(Map*);
    void printError(QString,QString);
    void newBehaviourAdded(QWidget* behaviourWidget);
};

#endif // simulatoriPavimenti_H
