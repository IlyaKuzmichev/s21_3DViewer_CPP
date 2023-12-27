#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#define GL_SILENCE_DEPRECATION

#include <QColor>
#include <QLineEdit>
#include <QMouseEvent>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWidget>
#include <QWheelEvent>
#include <QtMath>

#include "ObjectParameters.h"
#include "mainwindow.h"
#include "myobject.h"

extern "C" {
#include "../backend/3d_viewer.h"
}

typedef struct glsettings_s {
  QColor bg_colour;
  QColor edges_colour;
  QColor vertices_colour;
  bool is_edges_solid = true;
  bool is_parallel_projection = true;
  int vertices_type = DisplayMethod::none;
  GLfloat edges_thickness = 1.;
  GLfloat vertices_size = 1.;
} glsettings_t;

class MyGLWidget : public QOpenGLWidget {
  Q_OBJECT

 public:
  MyGLWidget(QWidget *parent = nullptr);
  ~MyGLWidget() {}
  void loadNewModel(const QString filename);
  glsettings_t WidgetSettings;

 private:
  MyObject object;
  QPoint lastPos;
  void setProjection();
  void drawVertices();
  void drawEdges();

 public slots:
  void UpdateObject(ObjectParameters *params);
  void updateFrame();

 signals:
  void mouseTrigger(double x, double y);
  void wheelTrigger(int increase_scale);
  void sendVF(uint64_t vertices, uint64_t faces);

 protected:
  void initializeGL() override;
  void paintGL() override;
  void resizeGL(int width, int height) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void wheelEvent(QWheelEvent *event) override;
};

#endif  // MYGLWIDGET_H
