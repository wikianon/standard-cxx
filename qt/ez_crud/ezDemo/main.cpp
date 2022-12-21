#include <QApplication>

#include <core/ezapplication.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    crud::EZApplication *ezApp = crud::EZApplication::instance();
    ezApp->start();

    return a.exec();
}
