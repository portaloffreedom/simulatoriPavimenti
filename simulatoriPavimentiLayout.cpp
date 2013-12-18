#include "simulatoriPavimenti.h"
#include "engine/loggerstartwidget.h"
#include <iostream>
#include <QMessageBox>

void SimulatoriPavimenti::createWidgets()
{
    this->setCentralWidget(new QWidget(this));
    this->centralWidget()->setVisible(false);
    reinterpret_cast<QApplication*>(QApplication::instance())->setWindowIcon(QIcon("icona_florimage1.ico"));

    timeSlider = new QSlider(this);
    timeSlider->setOrientation(Qt::Horizontal);
    
    playButton = new QPushButton(tr("Play"),this);
    playButton->setCheckable(true);

    stepButton = new QPushButton(tr("Step"),this);

    speedSlider = new QSlider(this);
    speedSlider->setOrientation(Qt::Horizontal);
    speedSlider->setMinimum(1);
    speedSlider->setMaximum(1000);

    behaviourDockWidget = new QDockWidget(tr("Behaviours"),this);
    behaviourDockWidget->setShown(false);
    logDockWidget = new QDockWidget(tr("Log"),this);
    logDockWidget->setShown(false);

    setDockWidgets();
    
    this->settingswidget = new SettingsWidget(this);
    
    this->loggerStartWindow = new LoggerStartWidget();
    connect(loggerStartWindow,SIGNAL(loggerStart(Logger*,smReal,smReal,bool)),
                  this,SLOT(startRegisterSensors(Logger*,smReal,smReal,bool)));
}

void SimulatoriPavimenti::setDockWidgets()
{
    // behaviourDockWidget
    behaviourLayout = new QVBoxLayout(behaviourDockWidget);
    QWidget* behaviourLayoutWidget = new QWidget(behaviourDockWidget);
    behaviourDockWidget->setWidget(behaviourLayoutWidget);
    behaviourLayoutWidget->setLayout(behaviourLayout);

    newBehaviourButton = new QPushButton(tr("New Behaviour"));
    behaviourLayout->addWidget(newBehaviourButton);
    connect(newBehaviourButton,SIGNAL(clicked()),this,SLOT(addNewBehaviour()));
    
    behaviourLayout->addSpacerItem(
        new QSpacerItem(0,0,QSizePolicy::Minimum,QSizePolicy::MinimumExpanding)
    );
    
    // logDockWidget
    
}

void SimulatoriPavimenti::newBehaviourAdded(QWidget* behaviourWidget)
{
    int pos = behaviourLayout->count() - 2; // 2 -> button and spacer
    behaviourLayout->insertWidget(pos,behaviourWidget);
}

void SimulatoriPavimenti::setLayouts()
{

    mainVerticalLayout = new QVBoxLayout(this);
    this->centralWidget()->setLayout(mainVerticalLayout);
//     mainVerticalLayout->addWidget(map);
    mainVerticalLayout->addWidget(timeSlider);

    QHBoxLayout* horizontalLayout = new QHBoxLayout(this);
    mainVerticalLayout->addLayout(horizontalLayout);
    horizontalLayout->addWidget(playButton);
    horizontalLayout->addWidget(stepButton);
    horizontalLayout->addWidget(speedSlider);

    this->addDockWidget(Qt::LeftDockWidgetArea,behaviourDockWidget);
    this->addDockWidget(Qt::RightDockWidgetArea,logDockWidget);
}

void SimulatoriPavimenti::setMapWidget(Map* map)
{
    if (this->map != nullptr)
        mainVerticalLayout->removeWidget(this->map);
    //delete this->map; //questa delete è già eseguita dal traffic engine
    
    this->map = map;
    mainVerticalLayout->insertWidget(0,map,1);
}

void SimulatoriPavimenti::createActions()
{
    // File menu
    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));
    
    registerAct = new QAction(tr("&Register Sensor Output"),this);
    connect(registerAct, SIGNAL(triggered()),this, SLOT(registerSensors()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    // View Menu
    showLogDockAct = new QAction(tr("&Log"),this);
    showLogDockAct->setCheckable(true);
    connect(showLogDockAct, SIGNAL(toggled(bool)),logDockWidget,SLOT(setShown(bool)));
    connect(logDockWidget, SIGNAL(visibilityChanged(bool)), showLogDockAct, SLOT(setChecked(bool)));

    showBehaviourDockAct = new QAction(tr("&Behaviours"),this);
    showBehaviourDockAct->setCheckable(true);
    connect(showBehaviourDockAct, SIGNAL(toggled(bool)),behaviourDockWidget,SLOT(setShown(bool)));
    connect(behaviourDockWidget, SIGNAL(visibilityChanged(bool)),showBehaviourDockAct,SLOT(setChecked(bool)));
    behaviourDockWidget->setVisible(true);


    //Settings Menu
    preferencesAct = new QAction(tr("&Preferences"),this);
    preferencesAct->setShortcut(QKeySequence::Preferences);
    connect(preferencesAct,SIGNAL(triggered()),settingswidget,SLOT(show()));

    aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}


void SimulatoriPavimenti::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
    fileMenu->addAction(registerAct);
    fileMenu->addAction(exitAct);

    viewMenu = menuBar()->addMenu(tr("&View"));
    //viewMenu->addAction(showLogDockAct); //never used, so it got removed. TODO reminder, use it.
    viewMenu->addAction(showBehaviourDockAct);

    settingsMenu = menuBar()->addMenu(tr("&Settings"));
    settingsMenu->addAction(preferencesAct);

    aboutMenu = menuBar()->addMenu(tr("&About"));
    aboutMenu->addAction(aboutAct);
    aboutMenu->addAction(aboutQtAct);
}

void SimulatoriPavimenti::resetButtons()
{
    playButton->setChecked(false);
    speedSlider->setValue(100);
}

void SimulatoriPavimenti::about()
{
    QMessageBox::about(this,
                       tr("About SimulatoriPavimenti"),
                       tr("This is a floor simulator"));
}