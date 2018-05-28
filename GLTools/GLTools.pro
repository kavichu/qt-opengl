#-------------------------------------------------
#
# Project created by QtCreator 2013-04-01T05:44:04
#
#-------------------------------------------------

QT       -= core gui

TARGET = GLTools
TEMPLATE = lib
CONFIG += staticlib

SOURCES += gltools.cpp \
    GLee.c \
    math3d.cpp \
    TriangleMesh.cpp \
    VBOMesh.cpp

HEADERS += gltools.h \
    GLee.h \
    math3d.h \
    glframe.h \
    stopwatch.h \
    glut.h \
    VBOMesh.h \
    TriangleMesh.h \
    glfrustum.h


#DESTDIR = $$quote(/home/luisvaldes/Dev-Packages/GLTools/lib/)
