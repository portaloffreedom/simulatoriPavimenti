#include "simulatoriPavimenti.h"
#include "map/mapreader.h"
#include "engine/behaviors/pathbehavior.h"
#include "engine/agentbehavior.h"

#include <QtGui/QLabel>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QAction>
#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QErrorMessage>
#include <QHBoxLayout>
#include <QXmlStreamReader>

#include <iostream>


SimulatoriPavimenti::SimulatoriPavimenti(QWidget* parent) :
    QMainWindow(parent),
    engine(nullptr),
    map(nullptr)
{
    this->createWidgets();
    this->setLayouts();
    this->createActions();
    this->createMenus();
}

SimulatoriPavimenti::~SimulatoriPavimenti()
{}

void SimulatoriPavimenti::closeEvent(QCloseEvent* event)
{
    QWidget::closeEvent(event);
    emit quitting();
}


void SimulatoriPavimenti::open()
{
    QString fileName =
        QFileDialog::getOpenFileName(this, tr("Open xml File"),
                                     QDir::currentPath(),
                                     tr("XML Files (*.xml)"));
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("XML map"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    std::cout<<"file scelto: "<<fileName.toStdString()<<std::endl;

    MapReader mapreader(file, this->settingswidget);
    connect(&mapreader,SIGNAL(finished(Map*)),this,SLOT(parsingFinished(Map*)));
    connect(&mapreader,SIGNAL(error(QString,QString)),this,SLOT(printError(QString,QString)));
    mapreader.parse();
    disconnect(&mapreader,SIGNAL(finished(Map*)),this,SLOT(parsingFinished(Map*)));
    disconnect(&mapreader,SIGNAL(error(QString,QString)),this,SLOT(printError(QString,QString)));


}

void SimulatoriPavimenti::parsingFinished(Map* map)
{
    this->resetButtons();
    this->setMapWidget(map);
    GroundEngine *groundEngine = new GroundEngine(map);

    delete this->engine;
    engine = new TrafficEngine(this->settingswidget,map,groundEngine, 60);
    //TODO spostare l'hardcode nelle impostazioni

    this->centralWidget()->setVisible(true);

    connect(playButton,SIGNAL(pressed()),engine,SLOT(start()));
    connect(stepButton,SIGNAL(pressed()),engine,SLOT(singleStep()));
    connect(speedSlider,SIGNAL(valueChanged(int)),engine,SLOT(setSpeed(int)));
    connect(engine,SIGNAL(newBehaviour(QWidget*)),this,SLOT(newBehaviourAdded(QWidget*)));

}

void SimulatoriPavimenti::printError(QString name, QString description)
{
    QErrorMessage *err = QErrorMessage::qtHandler();
    QString errorString = QString(name).append(":\n");
    errorString.append(description);
    
    err->showMessage(errorString);
    
    std::cerr<<"#ERROR: "<<errorString.toStdString()<<std::endl;
}

void SimulatoriPavimenti::addNewBehaviour()
{
    //std::cout<<"added new behaviour"<<std::endl;
    //TODO crea il nuovo widget
    engine->addBehavior(new PathBehavior());
}




// #include "simulatoriPavimenti.moc"
