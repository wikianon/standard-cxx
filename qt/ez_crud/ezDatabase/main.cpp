#include <QApplication>
#include "ezdemoinstaller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ezDatabase w;
    w.show();
    
    return a.exec();
}
