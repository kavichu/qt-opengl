/*
Luis Valdes - 2013
*/

#include <QMouseEvent>
#include <QDebug>
#include <QTimer>
#include <QThread>

//#include <glframe.h>
//#include <math3d.h>
#include <gltools.h>
#include <GL/glu.h>

#include <cmath>

#include "ludoboard.h"

#include "gltools.h"
#include "ludocone.h"

const float KPi = 3.14159265359f;
//const float KPi = 3.1415f;

LudoBoard::LudoBoard(QWidget *parent) :
    QGLWidget(parent)
  //      QGLWidget(parent)
{
    // GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH
    setFormat(QGLFormat(QGL::DoubleBuffer | QGL::Rgba | QGL::DepthBuffer));

    rotationX = 30;
    rotationY = 0;
    scaling = 1.0;

    m_origin = QVector3D(0.0f, -3.0f, 0.0f);
    m_radius = 3.0f;
    m_diameter = 2 * m_radius;

    createPositions();

    GLfloat red[4] = {1.0f, 0.0f, 0.0f, 1.0f};
    GLfloat green[4] = {0.0f, 1.0f, 0.0f, 1.0f};
    GLfloat blue[4] = {0.0f, 0.0f, 1.0f, 1.0f};
    QColor c(Qt::yellow);
    GLfloat yellow[4] = {GLfloat(c.redF()), GLfloat(c.greenF()), GLfloat(c.blueF()), 1.0f};

    for(int i=0; i < 1; i++){
        m_ludoConeList.append(new LudoCone(red));
        m_ludoConeList.append(new LudoCone(blue));
        m_ludoConeList.append(new LudoCone(green));
        m_ludoConeList.append(new LudoCone(yellow));
    }


    foreach(const QVector3D &v, m_globalPositions){
        int c = 0;
        qCount(m_globalPositions, v, c);
        if (c > 1){
            qDebug() << "Found " << v;
        }
    }

    randomize();
}

LudoBoard::~LudoBoard()
{
    glDeleteLists(m_playerBlocksList, 2);
    qDeleteAll(m_ludoConeList);
}

void LudoBoard::initializeGL()
{
    qglClearColor(Qt::black);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // Light values and coordinates
    GLfloat ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat lightPos[] = { -50.f, 6.0f, -24.0f, 1.0f };


    // Enable lighting
    glEnable(GL_LIGHTING);
    // Set up and enable light 0
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0,GL_POSITION,lightPos);

    // Cut-off angle is 60 degrees
    //glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,60.0f);


    glEnable(GL_LIGHT0);
    // Enable color tracking
    glEnable(GL_COLOR_MATERIAL);
    // Set material properties to follow glColor values
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // Create Display Lists
    createLists();
}

void LudoBoard::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    {
        drawBoard();
    }
    glPopMatrix();
}

void LudoBoard::resizeGL(int w, int h)
{
    GLfloat fAspect;

    // Prevent a divide by zero
    if(h == 0)
        h = 1;


    // Set Viewport to window dimensions
    glViewport(0, 0, w, h);

    // Reset coordinate system
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    fAspect = (GLfloat)w/(GLfloat)h;
    gluPerspective(60.0f, fAspect, 1.0, 200.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}


void LudoBoard::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
    qDebug() << "Moused pressed";
}

void LudoBoard::mouseMoveEvent(QMouseEvent *event)
{
    GLfloat dx = GLfloat(event->pos().x() - lastPos.x()) / width();
    GLfloat dy = GLfloat(event->pos().y() - lastPos.y()) / height();

    if(event->buttons() & Qt::LeftButton){
        GLfloat rotation = rotationX + 180 *dy;
        if((rotation <= 90.0f) && (rotation >= 30.0f)){
            rotationX += 180 * dy;
        }
        rotationY += 180 * dx;
        updateGL();
    }else if(event->buttons() & Qt::RightButton){
        GLfloat rotation = rotationX + 180 *dy;
        if((rotation <= 90.0f) && (rotation >= 30.0f)){
            rotationX += 180 * dy;
        }
        rotationY += 180 * dx;
        updateGL();
    }

    lastPos = event->pos();
}

void LudoBoard::wheelEvent(QWheelEvent *event)
{
    double numDegrees = -event->delta() / 8.0;
    double numSteps = numDegrees / 15.0;
    scaling *= std::pow(1.125, numSteps);
    update();
}

void LudoBoard::drawBoard()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPushMatrix();

    glTranslatef(0.0f, 0.0f, -60.0f);

    glScalef(scaling, scaling, scaling);

    glColor3f(1.0f, 0.0f, 0.0f);

    glRotatef(rotationX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotationY, 0.0f, 1.0f, 0.0f);

    drawLudo();

    glPushMatrix();
    {
        //glLoadIdentity();
        //glTranslatef(0.0f, 0.0f, -60.0f);

        drawLudoCones();
    }
    glPopMatrix();

    glPopMatrix();

    gltools::printErrors();

}

void LudoBoard::drawLudo()
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    {
        glCallList(m_whiteBlocksList);
        glCallList(m_playerBlocksList);
    }
    glPopMatrix();
}

void LudoBoard::drawLudoCones()
{
    foreach(LudoCone *ld, m_ludoConeList){
        glPushMatrix();
        {
            GLfloat pos[] = {GLfloat(ld->position().x()),
                             GLfloat(ld->position().y()) + 0.1f,
                             GLfloat(ld->position().z())};
            glTranslatef(pos[0], pos[1], pos[2]);
            //            glTranslatef(0.0f, 0.0f, 2 * 6.0f);
            ld->drawCone();
        }
        glPopMatrix();
    }
}

void LudoBoard::drawCircle(QVector3D center, GLfloat size)
{
    GLfloat step = KPi / 12.0;
    glBegin(GL_TRIANGLE_FAN);
    {
        glNormal3f(0.0f, 1.0f, 0.0f);
        GLfloat c[3] = {GLfloat(center.x()),
                        GLfloat(center.y()),
                        GLfloat(center.z())};
        glVertex3fv(c);
        for(GLfloat angle = 0.0; angle < (2.0f * KPi); angle += step){
            GLfloat x = size * sin(angle);
            GLfloat y = c[1];
            GLfloat z = size * cos(angle);
            GLfloat p[3] = {x + c[0], y , z + c[2]};
            glVertex3fv(p);
        }
    }
    glEnd();
}

void LudoBoard::createPositions()
{
    QVector3D center = m_origin;
    // ##### Player Positions #####
    // Red
    {
        QVector< QVector3D > redPositions;
        // Only one circle on the left
        center.setX(m_origin.x() - m_diameter);
        center.setZ(m_origin.z() + 5 * m_diameter);
        redPositions.append(center);

        // Four central circles
        center.setX(m_origin.x());
        center.setZ(m_origin.z() + m_diameter);
        for(int i=0; i < 4; i++){
            redPositions.append(center);
            center.setZ(center.z() + m_diameter);
        }
        m_playerPositions.insert(KRed, redPositions);
    }

    // Green
    {
        QVector< QVector3D > greenPositions;
        // Only one circle on the left
        center.setX(m_origin.x() - 5 * m_diameter);
        center.setZ(m_origin.z() - m_diameter);
        greenPositions.append(center);

        // Four central circles
        center.setX(m_origin.x() - m_diameter);
        center.setZ(m_origin.z());
        for(int i=0; i < 4; i++){
            greenPositions.append(center);
            center.setX(center.x() - m_diameter);
        }
        m_playerPositions.insert(KGreen, greenPositions);
    }

    // Blue
    {
        QVector< QVector3D > bluePositions;
        // Only one circle on the left
        center.setX(m_origin.x() + m_diameter);
        center.setZ(m_origin.z() - 5 * m_diameter);
        bluePositions.append(center);

        // Four central circles
        center.setX(m_origin.x());
        center.setZ(m_origin.z() - m_diameter);
        for(int i=0; i < 4; i++){
            bluePositions.append(center);
            center.setZ(center.z() - m_diameter);
        }
        m_playerPositions.insert(KBlue, bluePositions);
    }

    // Yellow
    {
        QVector< QVector3D > yellowPositions;
        // Only one circle on the left
        center.setX(m_origin.x() + 5 * m_diameter);
        center.setZ(m_origin.z() + m_diameter);
        yellowPositions.append(center);

        // Four central circles
        center.setX(m_origin.x() + m_diameter);
        center.setZ(m_origin.z());
        for(int i=0; i < 4; i++){
            yellowPositions.append(center);
            center.setX(center.x() + m_diameter);
        }
        m_playerPositions.insert(KYellow, yellowPositions);
    }

    // ##### Normal Positions #####
    {
        // Red side Blocks
        center.setX(m_origin.x() - 5 * m_diameter);
        center.setZ(m_origin.z());
        m_normalPositions.append(center);

        center.setX(m_origin.x() - m_diameter);
        center.setZ(m_origin.z() + 4 * m_diameter);
        for(int i=0; i < 4; i++){
            m_normalPositions.append(center);
            center.setZ(center.z() - m_diameter);
        }
        center.setX(m_origin.x() - 2 * m_diameter);
        center.setZ(m_origin.z() + m_diameter);
        for(int i=0; i < 4; i++){
            m_normalPositions.append(center);
            center.setX(center.x() - m_diameter);
        }

        // Green side Blocks
        center.setX(m_origin.x());
        center.setZ(m_origin.z() - 5 * m_diameter);
        m_normalPositions.append(center);

        center.setX(m_origin.x() - m_diameter);
        center.setZ(m_origin.z() - m_diameter);
        for(int i=0; i < 4; i++){
            m_normalPositions.append(center);
            center.setX(center.x() - m_diameter);
        }
        center.setX(m_origin.x() - m_diameter);
        center.setZ(m_origin.z() - 2 * m_diameter);
        for(int i=0; i < 4; i++){
            m_normalPositions.append(center);
            center.setZ(center.z() - m_diameter);
        }

        // Blue side Blocks
        center.setX(m_origin.x() + 5 * m_diameter);
        center.setZ(m_origin.z());
        m_normalPositions.append(center);

        center.setX(m_origin.x() + m_diameter);
        center.setZ(m_origin.z() - m_diameter);
        for(int i=0; i < 4; i++){
            m_normalPositions.append(center);
            center.setZ(center.z() - m_diameter);
        }
        center.setX(m_origin.x() + 2 * m_diameter);
        center.setZ(m_origin.z() - m_diameter);
        for(int i=0; i < 4; i++){
            m_normalPositions.append(center);
            center.setX(center.x() + m_diameter);
        }

        // Yellow side Blocks
        center.setX(m_origin.x() );
        center.setZ(m_origin.z() + 5 * m_diameter);
        m_normalPositions.append(center);

        center.setX(m_origin.x() + m_diameter);
        center.setZ(m_origin.z() + m_diameter);
        for(int i=0; i < 4; i++){
            m_normalPositions.append(center);
            center.setX(center.x() + m_diameter);
        }
        center.setX(m_origin.x() + m_diameter);
        center.setZ(m_origin.z() + 2 * m_diameter);
        for(int i=0; i < 4; i++){
            m_normalPositions.append(center);
            center.setZ(center.z() + m_diameter);
        }
    }

    m_globalPositions.resize(36);
    int offSet = 0;

    qCopy(m_normalPositions.begin(),
          m_normalPositions.end(),
          m_globalPositions.begin());

    m_globalPositions.insert(m_globalPositions.begin() + offSet,
                             m_playerPositions.value(KRed)[0]);
    offSet += 10;
    m_globalPositions.insert(m_globalPositions.begin() + offSet,
                             m_playerPositions.value(KGreen)[0]);
    offSet += 10;
    m_globalPositions.insert(m_globalPositions.begin() + offSet,
                             m_playerPositions.value(KBlue)[0]);
    offSet += 10;
    m_globalPositions.insert(m_globalPositions.begin() + offSet,
                             m_playerPositions.value(KYellow)[0]);
}

void LudoBoard::createLists()
{
    m_whiteBlocksList = glGenLists(2);
    m_playerBlocksList = m_whiteBlocksList + 1;

    // White block List
    glNewList(m_whiteBlocksList, GL_COMPILE);
    {
        glColor3f(1.0f, 1.0f, 1.0f);
        drawCircle(QVector3D(-30, -3, 0), m_radius);
        foreach(const QVector3D &pos, m_normalPositions){
            drawCircle(pos, m_radius);
        }
    }
    glEndList();

    // Player block List
    glNewList(m_playerBlocksList, GL_COMPILE);
    {
        // Red
        glColor3f(1.0f, 0.0f, 0.0f);
        foreach(const QVector3D &pos, m_playerPositions.value(KRed)){
            drawCircle(pos, m_radius);
        }
        // Green
        glColor3f(0.0f, 1.0f, 0.0f);
        foreach(const QVector3D &pos, m_playerPositions.value(KGreen)){
            drawCircle(pos, m_radius);
        }
        // Blue
        glColor3f(0.0f, 0.0f, 1.0f);
        foreach(const QVector3D &pos, m_playerPositions.value(KBlue)){
            drawCircle(pos, m_radius);
        }
        // Yellow
        qglColor(Qt::yellow);
        foreach(const QVector3D &pos, m_playerPositions.value(KYellow)){
            drawCircle(pos, m_radius);
        }

    }
    glEndList();

    foreach(LudoCone *ld, m_ludoConeList){
        ld->createLists();
    }
}

QVector3D LudoBoard::lerp(const QVector3D &p0, const QVector3D &p1, GLfloat b)
{
    GLfloat x = 0.f;
    GLfloat y = 0.f;
    GLfloat z = 0.f;
    x = (1 - b) * p0.x() + b * p1.x();
    y = (1 - b) * p0.y() + b * p1.y();
    z = (1 - b) * p0.z() + b * p1.z();
    return QVector3D(x, y, z);
}

void LudoBoard::randomize()
{
    qsrand(time(NULL));
    QVector<int> randomIndex(40, 0);

    foreach(LudoCone *ld, m_ludoConeList){
        int i = qrand() % 40;
        while(randomIndex[i]){
            i = qrand() % 40;
        }
        randomIndex[i] = 1;
        ld->setPosition(m_globalPositions[i]);
        qDebug() << *ld;
    }
    updateGL();

    QTimer::singleShot(5000, this, SLOT(animateLudoCone()));
}

void LudoBoard::advance()
{
    //    QTimer::singleShot(1000, this, SLOT(randomize()));

    //    QVector< const QVector3D & > p1Vector(0);
    //    foreach (LudoCone *ldc, m_ludoConeList) {
    //        const QVector3D &p0 = ldc->position();
    //        int p1Index = m_globalPositions.indexOf(p0);
    //        if(p1Index >= m_globalPositions.size())
    //            p1Index -= m_globalPositions.size();
    //        p1Vector.append(m_globalPositions.at(p1Index));
    //    }
}

void LudoBoard::animateLudoCone()
{
    int steps = 1;
    QMap< LudoCone *, QVector3D > p1Vector;
    foreach (LudoCone *ldc, m_ludoConeList) {
        QVector3D p0 = ldc->position();
        int p1Index = m_globalPositions.indexOf(p0) + steps;
        if(p1Index >= m_globalPositions.size())
            p1Index -= m_globalPositions.size();
        p1Vector.insert(ldc, m_globalPositions.at(p1Index));
    }

    GLfloat timeMilli = steps * 1000.f;
    GLfloat frMilli = 1000.f / 30.f;
    QMap< LudoCone *, QVector<QVector3D> > positions;

//    foreach (LudoCone *ldc, m_ludoConeList) {
//        positions.insert(ldc, QVector<QVector3D>());
//    }

    foreach (LudoCone *ldc, m_ludoConeList) {
        QVector< QVector3D > points;
        for(GLfloat ellapsedTime = 0.f; ellapsedTime < timeMilli; ellapsedTime += frMilli){
            GLfloat factor = ellapsedTime / timeMilli;

            QVector3D l = lerp(ldc->position(), p1Vector.value(ldc), factor);
            points.append(l);
        }
        positions.insert(ldc, points);
    }

    foreach (LudoCone *ldc, m_ludoConeList) {
        QVector<QVector3D> posVect = positions.value(ldc);
        foreach(QVector3D v, posVect){
            ldc->setPosition(v);
            updateGL();
        }
    }

    QTimer::singleShot(5000, this, SLOT(randomize()));
}


