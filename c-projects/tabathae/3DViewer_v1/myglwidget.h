#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H
#define GL_SILENCE_DEPRECATION

#include <QColor>
#include <QColorDialog>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QMainWindow>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWidget>
#include <QSettings>
#include <QTimer>
#include <cstring>

#include "./QtGifImage-master/src/gifimage/qgifimage.h"

extern "C" {
#include <stdio.h>

#include "../object.h"
}

class MyGLWidget : public QOpenGLWidget {
 public:
  MyGLWidget(QWidget *parent);
  void setupBackgroundColor(float r, float g, float b);
  void setupEdgeColor(float r, float g, float b);
  void setupVertexColor(float r, float g, float b);
  void setupModel(object_t *obj);
  void drawObj();
  void setupProjectionType(int value);
  void setupEdgesType(int value);
  void setupVerticesType(int value);
  object_t *getModel();
  void setupEdgesSize(int value);
  void setupVerticesSize(int value);
  void screen(QString filename, QString fileext);

 protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

 private:
  int projection_type = 0;
  int edges_type = 0;
  double edges_size = 1;
  int vertices_type = 0;
  double vertices_size = 1;
  object_t *obj = NULL;
  float b_r = 0;
  float b_g = 0;
  float b_b = 0;
  float e_r = 0;
  float e_g = 0;
  float e_b = 0;
  float v_r = 0;
  float v_g = 0;
  float v_b = 0;
};

#endif  // MYGLWIDGET_H
