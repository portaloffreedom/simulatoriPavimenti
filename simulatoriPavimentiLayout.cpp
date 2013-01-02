#include "simulatoriPavimenti.h"

void SimulatoriPavimenti::createWidgets()
{
    this->setCentralWidget(new QWidget(this));
    this->centralWidget()->setVisible(false);

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
    if (this->map != 0)
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

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    // View Menu
    showLogDockAct = new QAction(tr("&Log"),this);
    showLogDockAct->setCheckable(true);
    connect(showLogDockAct, SIGNAL(toggled(bool)),logDockWidget,SLOT(setShown(bool)));

    showBehaviourDockAct = new QAction(tr("&Behaviours"),this);
    showBehaviourDockAct->setCheckable(true);
    connect(showBehaviourDockAct, SIGNAL(toggled(bool)),behaviourDockWidget,SLOT(setShown(bool)));


    //Settings Menu
    preferencesAct = new QAction(tr("&Preferences"),this);
    preferencesAct->setShortcut(QKeySequence::Preferences);
    //TODO connect preferencesAct

//     saveAsAct = new QAction(tr("&Save As..."), this);
//     saveAsAct->setShortcuts(QKeySequence::SaveAs);
//     connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    aboutAct = new QAction(tr("&About"), this);
    //TODO connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}


void SimulatoriPavimenti::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
//     fileMenu->addAction(saveAsAct);
    fileMenu->addAction(exitAct);

    viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction(showLogDockAct);
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