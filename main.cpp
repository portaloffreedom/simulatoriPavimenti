#include <QtGui/QApplication>
#include <QErrorMessage>
#include "simulatoriPavimenti.h"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    SimulatoriPavimenti *mainWindow = new SimulatoriPavimenti();
    app.connect(mainWindow, SIGNAL(quitting()), &app, SLOT(quit()));
    mainWindow->show();

    //sposta il debug dal terminale a finestre grafiche
    QErrorMessage::qtHandler();  

    return app.exec();
}
