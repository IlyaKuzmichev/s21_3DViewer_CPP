#include "view/glwidget.h"


#include <QDebug>
#include <QElapsedTimer>

GLWidget::GLWidget(QWidget* parrent) : QOpenGLWidget(parrent) {}

void GLWidget::initializeGL() { glEnable(GL_DEPTH_TEST); }

void GLWidget::resizeGL(int, int) {}

void GLWidget::paintGL() {
//  QElapsedTimer debug;
//  debug.start();
  setProjection();
  glClearColor(WidgetSettings.bg_colour.redF(),
               WidgetSettings.bg_colour.greenF(),
               WidgetSettings.bg_colour.blueF(), 1.0f);
  glClear(GL_COLOR_BUFFER_BIT |
          GL_DEPTH_BUFFER_BIT);  // очистка буфера изображения и глубины
  drawVertices();
  drawEdges();

//  qDebug() << "Time of drawing" << debug.elapsed()  << '\n';
}

void GLWidget::drawVertices() {

}

void GLWidget::drawEdges() {

}

void GLWidget::loadNewModel(const QString filename) {
    (void)filename;
}

void GLWidget::mousePressEvent(QMouseEvent* event) { lastPos = event->pos(); }

void GLWidget::mouseMoveEvent(QMouseEvent* event) {
  double diff_x = event->pos().y() - lastPos.y();
  double diff_y = event->pos().x() - lastPos.x();
  lastPos = event->pos();
  emit mouseTrigger(diff_x, diff_y);
}

void GLWidget::wheelEvent(QWheelEvent* event) {
  int increase_scale = 5;
  if (event->angleDelta().y() < 0) {
    increase_scale = -5;
  }
  emit wheelTrigger(increase_scale);
}

void GLWidget::setProjection() {

}

void GLWidget::updateFrame() { update(); }

