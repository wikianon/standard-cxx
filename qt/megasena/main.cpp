#include <QApplication>
#include <QIcon>
#include "mygui.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    
    MyGui window;
    window.setWindowTitle("Gerador Apostas Megasena");
    window.setToolTip("Gerador Apostas Megasena");
    window.setWindowIcon(QIcon("images/bookmark.gif"));
    window.resize(450, 300);
    window.move(300, 300);
    window.show();
    
    return app.exec();    
}