#include <QtGui/QApplication>
#include <QErrorMessage>
#include "simulatoriPavimenti.h"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    simulatoriPavimenti foo;
    foo.show();
    QErrorMessage::qtHandler();
    return app.exec();
}
