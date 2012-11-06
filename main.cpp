#include <QtGui/QApplication>
#include "simulatoriPavimenti.h"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    simulatoriPavimenti foo;
    foo.show();
    return app.exec();
}
