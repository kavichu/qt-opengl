/*
Luis Valdes - 2013
*/

#include <GL/glu.h>

#include "ludocone.h"

struct LudoConePrivate {
    LudoConePrivate()
    {
        quadricObj = gluNewQuadric();
    }

    ~LudoConePrivate()
    {
        gluDeleteQuadric(quadricObj);
    }

    GLfloat color[4];
    GLuint coneDisplayList;
    GLUquadric *quadricObj;
};

LudoCone::LudoCone(GLfloat *color) :
    d(new LudoConePrivate)
{
    d->color[0] = color[0];
    d->color[1] = color[1];
    d->color[2] = color[2];
    d->color[3] = color[3];
}

LudoCone::~LudoCone()
{
    delete d;
}

void LudoCone::drawCone()
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    {
        //glLoadIdentity();
        //glTranslatef(0.0f, 2.0f, -60.0f);



        glCallList(d->coneDisplayList);
    }
    glPopMatrix();
}

const QVector3D &LudoCone::position() const
{
    return m_position;
}

void LudoCone::setPosition(const QVector3D &position)
{
    m_position = position;
}

//        _//
//       ( )//
//       / \ //
//      /   \//
//     (     )//
void LudoCone::createLists()
{
    d->coneDisplayList = glGenLists(1);

    double totalHeight = 0.0;
    glNewList(d->coneDisplayList, GL_COMPILE);
    {
        // Draw Black base
        glPushMatrix();
        {
            double baseRadius = 2.4;
            double topRadius = 2.35;
            double height = 0.36;

            //glTranslatef(-6.0f, -2.9f, 6.0f);
            glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
            glColor3f(0.0f, 0.0f, 0.0f);
            // Draw BaseDisk Color Black
            gluDisk(d->quadricObj, 1, 2.6, 30, 30);
            gluCylinder(d->quadricObj, baseRadius, topRadius, height, 30, 20);

            totalHeight += height;
        }
        glPopMatrix();

        // Draw Cone 1
        // Height = 1.4
        glPushMatrix();
        {
            double baseRadius = 2.3;
            double topRadius = 1.5;
            double height = 1.584;

            glTranslatef(0.0f, totalHeight, 0.0f);
            glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

            glColor4fv(d->color);
            gluCylinder(d->quadricObj, baseRadius, topRadius, height, 30, 20);

            totalHeight += height;
        }
        glPopMatrix();

        // Draw Cone 2
        // Height = 1.4
        glPushMatrix();
        {
            double baseRadius = 1.5;
            double topRadius = 0.28;
            double height = 3.6;


            glTranslatef(0.0f, totalHeight, 0.0f);
            glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

            glColor4fv(d->color);
            gluCylinder(d->quadricObj, baseRadius, topRadius, height, 30, 20);
            totalHeight += height;
        }
        glPopMatrix();

        // Esfere
        // Height = 1.4
        glPushMatrix();
        {
            double radius = (1.584/2);

            glTranslatef(0.0f, totalHeight, 0.0f);
            glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

            glColor4fv(d->color);
            gluSphere(d->quadricObj, radius, 30, 20);
        }
        glPopMatrix();

    }
    glEndList();
}


QDebug operator<<(QDebug dbg, const LudoCone &c)
{
    dbg.nospace() << QString("0x%1 = (%2, %3, %4)").arg(qint64(&c)).
                     arg(c.position().x()).
                     arg(c.position().y()).
                     arg(c.position().z());

    return dbg.space();
}
