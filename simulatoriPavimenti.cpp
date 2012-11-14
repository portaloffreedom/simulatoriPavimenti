#include "simulatoriPavimenti.h"
#include "map/mapreader.h"

#include <QtGui/QLabel>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QAction>
#include <QFileDialog>
#include <QMessageBox>

#include <iostream>
#include <QXmlStreamReader>

simulatoriPavimenti::simulatoriPavimenti()
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
}

simulatoriPavimenti::~simulatoriPavimenti()
{}

void simulatoriPavimenti::createActions()
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


void simulatoriPavimenti::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
//     fileMenu->addAction(saveAsAct);
    fileMenu->addAction(exitAct);
}

bool parse(QFile &xmlFile);
void simulatoriPavimenti::open()
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

}
 
// #include "simulatoriPavimenti.moc"
