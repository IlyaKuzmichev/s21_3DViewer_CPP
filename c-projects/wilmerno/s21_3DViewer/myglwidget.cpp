#include "myglwidget.h"

#include <stdlib.h>

#include <QDebug>
#include <QElapsedTimer>

MyGLWidget::MyGLWidget(QWidget* parrent) : QOpenGLWidget(parrent) {}

void MyGLWidget::initializeGL() { glEnable(GL_DEPTH_TEST); }

void MyGLWidget::resizeGL(int w, int h) {}

void MyGLWidget::paintGL() {
  QElapsedTimer debug;
  debug.start();
  setProjection();
  glClearColor(WidgetSettings.bg_colour.redF(),
               WidgetSettings.bg_colour.greenF(),
               WidgetSettings.bg_colour.blueF(), 1.0f);
  glClear(GL_COLOR_BUFFER_BIT |
          GL_DEPTH_BUFFER_BIT);  // очистка буфера изображения и глубины
  glMatrixMode(GL_MODELVIEW);  // устанавливает положение и ориентацию матрице
                               // моделирования
  glLoadIdentity();  // загружает единичную матрицу моделирования
  drawVertices();
  drawEdges();

  qDebug() << "Time of drawing" << debug.elapsed()  << '\n';
}

void MyGLWidget::drawVertices() {
  const object_t* obj = object.GetPrintable();
  if (WidgetSettings.vertices_type != DisplayMethod::none) {
    if (WidgetSettings.vertices_type == DisplayMethod::circle) {
      glEnable(GL_POINT_SMOOTH);
    } else {
      glDisable(GL_POINT_SMOOTH);
    }
    glColor3d(WidgetSettings.vertices_colour.redF(),
              WidgetSettings.vertices_colour.greenF(),
              WidgetSettings.vertices_colour.blueF());
    glPointSize(WidgetSettings.vertices_size);
    glBegin(GL_POINTS);
    for (size_t i = 0; i != obj->v_count; ++i) {
      glVertex3d(obj->v_array[i].x, obj->v_array[i].y, obj->v_array[i].z);
    }
    glEnd();
  }
}

void MyGLWidget::drawEdges() {
  const object_t* obj = object.GetPrintable();
  glLineWidth(WidgetSettings.edges_thickness);
  glLineStipple(1, 0x00F0);
  glColor3d(WidgetSettings.edges_colour.redF(),
            WidgetSettings.edges_colour.greenF(),
            WidgetSettings.edges_colour.blueF());
  if (WidgetSettings.is_edges_solid) {
    glDisable(GL_LINE_STIPPLE);
  } else {
    glEnable(GL_LINE_STIPPLE);
  }
  for (size_t i = 0; i < obj->f_count; ++i) {
    const auto& face = obj->f_array[i];
    glBegin(GL_LINES);
    const auto& first_point = obj->v_array[face.v_array[0]];
    glVertex3d(first_point.x, first_point.y, first_point.z);
    for (size_t j = 1; j < face.v_count; ++j) {
      const auto& point = obj->v_array[face.v_array[j]];
      glVertex3d(point.x, point.y, point.z);
      glVertex3d(point.x, point.y, point.z);
    }
    glVertex3d(first_point.x, first_point.y, first_point.z);
    glEnd();
  }
}

void MyGLWidget::loadNewModel(const QString filename) {
  object.clearContents();
  object.path = filename;
  object.Parsing();
  uint64_t v = 0, f = 0;
  object.getVerticesAndFaces(&v, &f);
  emit sendVF(v, f);
  update();
}

void MyGLWidget::UpdateObject(ObjectParameters* params) {
  object.UpdateObject(params);
  update();
}

void MyGLWidget::mousePressEvent(QMouseEvent* event) { lastPos = event->pos(); }

void MyGLWidget::mouseMoveEvent(QMouseEvent* event) {
  double diff_x = event->pos().y() - lastPos.y();
  double diff_y = event->pos().x() - lastPos.x();
  lastPos = event->pos();
  emit mouseTrigger(diff_x, diff_y);
}

void MyGLWidget::wheelEvent(QWheelEvent* event) {
  int increase_scale = 5;
  if (event->angleDelta().y() < 0) {
    increase_scale = -5;
  }
  emit wheelTrigger(increase_scale);
}

void MyGLWidget::setProjection() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  GLfloat fov = 60.0 * M_PI / 180;
  GLfloat near = 1 / tan(fov / 2);
  if (WidgetSettings.is_parallel_projection) {
    glOrtho(-2., 2., -2., 2., -10, 10.);
  } else {
    glFrustum(-0.5, 0.5, -0.5, 0.5, near, 100.);
    glTranslated(0, 0, -near * 3);
  }
}

void MyGLWidget::updateFrame() { update(); }
