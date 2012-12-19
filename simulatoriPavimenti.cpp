#include "simulatoriPavimenti.h"
#include "map/mapreader.h"

#include <QtGui/QLabel>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QAction>
#include <QFileDialog>
#include <QMessageBox>
#include <QErrorMessage>

#include <iostream>
#include <QXmlStreamReader>

SimulatoriPavimenti::SimulatoriPavimenti()
{
//     QLabel* l = new QLabel( this );
//     l->setText( "Hello World!" );
//     setCentralWidget( l );
//     QAction* a = new QAction(this);
//     a->setText( "Quit" );
//     connect(a, SIGNAL(triggered()), SLOT(close()) );
//     menuBar()->addMenu( "File" )->addAction( a );
    this->createActions();
    this->createMenus();
//     this->setMinimumSize(200,200);
}

SimulatoriPavimenti::~SimulatoriPavimenti()
{}

void SimulatoriPavimenti::createActions()
{
    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

//     saveAsAct = new QAction(tr("&Save As..."), this);
//     saveAsAct->setShortcuts(QKeySequence::SaveAs);
//     connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

//     aboutAct = new QAction(tr("&About"), this);
//     connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

//     aboutQtAct = new QAction(tr("About &Qt"), this);
//     connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}


void SimulatoriPavimenti::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
//     fileMenu->addAction(saveAsAct);
    fileMenu->addAction(exitAct);
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
        QMessageBox::warning(this, tr("SAX Bookmarks"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    std::cout<<"file scelto: "<<fileName.toStdString()<<std::endl;

    MapReader mapreader(file);
    connect(&mapreader,SIGNAL(finished(Map*)),this,SLOT(getMap(Map*)));
    connect(&mapreader,SIGNAL(error(QString,QString)),this,SLOT(printError(QString,QString)));
    mapreader.parse();
    disconnect(&mapreader,SIGNAL(finished(Map*)),this,SLOT(getMap(Map*)));
    disconnect(&mapreader,SIGNAL(error(QString,QString)),this,SLOT(printError(QString,QString)));


}

void SimulatoriPavimenti::getMap(Map* map)
{
    this->map = map;
    this->map->setMinimumSize(100,100);
    this->setCentralWidget(map);

    this->engine = new TrafficEngine(map);
    stepButton = new QPushButton("step");
    connect(stepButton,SIGNAL(pressed()),engine,SLOT(step()));
    stepButton->show(); //TODO show this in the main window
    
//     this->map->resize(400,400); //TODO make this work (preferred size style)
}

void SimulatoriPavimenti::printError(QString name, QString description)
{
    QErrorMessage *err = QErrorMessage::qtHandler();
    QString errorString = QString(name).append(":\n");
    errorString.append(description);
    
    err->showMessage(errorString);
    
    std::cerr<<"#ERROR: "<<errorString.toStdString()<<std::endl;
}



// #include "simulatoriPavimenti.moc"
