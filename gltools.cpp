#include <GL/gl.h>
#include <GL/glu.h>
#include <QDebug>
#include <iostream>

#include "gltools.h"

namespace gltools{

void printErrors()
{
    GLenum errorCode = glGetError();
    if(errorCode != GL_NO_ERROR){
        const GLubyte *errorStr = gluErrorString(errorCode);
        std::cout << errorStr << std::endl;
        qDebug() << errorCode;
    }else{
        qDebug() << "No Errors";
    }
}

}

