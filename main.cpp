/*
Luis Valdes - 2013
*/

#include <QApplication>
#include <iostream>

#include "widget.h"
#include "ludoboard.h"

int main(int argc, char *argv[])
{
//    QGL::setPreferredPaintEngine(QPaintEngine::OpenGL);

    QApplication a(argc, argv);

    if (!QGLFormat::hasOpenGL()) {
        std::cerr << "This system has no OpenGL support" << std::endl;
        return 1;
    }

//    Widget w;
//    w.show();
    LudoBoard lb;
    lb.show();
    
    return a.exec();
}
