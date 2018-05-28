/*
Luis Valdes - 2013
*/

#ifndef LUDOCONE_H
#define LUDOCONE_H

#include <QColor>
#include <QVector3D>
#include <QDebug>

#include <GL/gl.h>
#include <GL/glu.h>

class LudoConePrivate;

class LudoCone
{
public:
    LudoCone(GLfloat *color);
    ~LudoCone();

    void createLists();

    void drawCone();

    const QVector3D & position() const;
    void setPosition(const QVector3D &position);


private:
    LudoConePrivate *d;
    QVector3D m_position;
};

QDebug operator <<(QDebug dbg, const LudoCone &c);

#endif // LUDOCONE_H
