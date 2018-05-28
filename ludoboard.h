/*
Luis Valdes - 2013
*/

#ifndef LUDOBOARD_H
#define LUDOBOARD_H

#include <QGLWidget>
#include <QVector3D>

class GLFrame;
class LudoCone;

class LudoBoard : public QGLWidget
{
    Q_OBJECT
public:

    enum Plane {K_XY, K_XZ, K_YZ};
    enum Players {KRed = 1, KGreen, KBlue, KYellow};

    explicit LudoBoard(QWidget *parent = 0);
    ~LudoBoard();
    
protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    void drawBoard();

    void drawLudo();
    void drawLudoCones();

    void drawCircle(QVector3D center, GLfloat size);

    void createPositions();
    void createLists();

    GLFrame *m_glFrame;

    GLfloat rotationX;
    GLfloat rotationY;
    GLfloat rotationZ;
    GLfloat scaling;

    QPoint lastPos;

    QVector3D m_origin;
    GLfloat m_radius;
    GLfloat m_diameter;

    QMap<int, QVector<QVector3D> > m_playerPositions;
    QVector<QVector3D> m_normalPositions;
    QVector<QVector3D> m_globalPositions;

    GLuint m_whiteBlocksList;
    GLuint m_playerBlocksList;

    QVector<LudoCone *> m_ludoConeList;

    QVector3D lerp(const QVector3D &p0, const QVector3D &p1, GLfloat b);

private slots:
    void randomize();
    void advance();

    void animateLudoCone();

};





#endif // LUDOBOARD_H
