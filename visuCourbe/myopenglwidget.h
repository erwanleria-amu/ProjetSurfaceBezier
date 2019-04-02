#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H

#include <QObject>
#include <QWidget>

#include <QKeyEvent>
#include <QTimer>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

#include "discretisation.h"
#include "surfacesbezier.h"
#include "courbes.h"
#include "math.h"
#include "surface.h"

class myOpenGLWidget : public QOpenGLWidget,
			   protected QOpenGLFunctions
{
	Q_OBJECT

public:
	explicit myOpenGLWidget(QWidget *parent = nullptr);
    ~myOpenGLWidget();
    void setNbCol(int i);
    int getNbCol();
    void upd();
    Point setDeplacementPoint(float u, float v, void* obj);
    void setU(float _u);
    void setV(float _v);
    void setSizeChanged();
    void setStep(double d);

    Discretisation *curDiscreteObj;

public slots:

signals:  // On ne les implémente pas, elles seront générées par MOC ;
		  // les paramètres seront passés aux slots connectés.

protected slots:
	void onTimeout();


protected:
	void initializeGL() override;
	void doProjection();
	void resizeGL(int w, int h) override;
	void paintGL() override;
	void keyPressEvent(QKeyEvent *ev) override;
	void keyReleaseEvent(QKeyEvent *ev) override;
	void mousePressEvent(QMouseEvent *ev) override;
	void mouseReleaseEvent(QMouseEvent *ev) override;
	void mouseMoveEvent(QMouseEvent *ev) override;
    QVector<Point> pointsCtrl;


private:
	double m_angle = 0;
	QTimer *m_timer = nullptr;
	double m_ratio = 1;

    QVector<GLfloat> vertData;

    Point deplacement;
    int nbCol = 4;
    bool sizeChanged = false;
    float u,v, step = 0.05f;

	//RR matrices utiles
	QMatrix4x4 m_modelView;
	QMatrix4x4 m_projection;
	QMatrix4x4 m_model;

	QOpenGLShaderProgram *m_program;
	QOpenGLBuffer m_vbo;

	void makeGLObjects();
	void tearGLObjects();
};


#endif // MYOPENGLWIDGET_H
