#include "view/glwidget.h"

#include <QDebug>
#include <QElapsedTimer>

#include "controller/viewer_controller.h"

s21::GLWidget::GLWidget(QWidget* parent)
    : QOpenGLWidget(parent), current_obj_(nullptr) {}

void s21::GLWidget::initializeGL() { glEnable(GL_DEPTH_TEST); }

void s21::GLWidget::paintGL() {
  QElapsedTimer debug;
  debug.start();
  setProjection();
  glClearColor(widget_settings.bg_colour.redF(),
               widget_settings.bg_colour.greenF(),
               widget_settings.bg_colour.blueF(), 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  if (current_obj_ == nullptr || current_obj_->vertices.empty()) {
    return;
  }
  drawVertices();
  drawEdges();

  qDebug() << "Time of drawing" << debug.elapsed()  << '\n';
}

void s21::GLWidget::drawVertices() {
  if (widget_settings.vertices_type == DisplayMethod::none) {
    return;
  }

  if (widget_settings.vertices_type == DisplayMethod::circle) {
    glEnable(GL_POINT_SMOOTH);
  } else {
    glDisable(GL_POINT_SMOOTH);
  }
  glPointSize(widget_settings.vertices_size);
  glColor3d(widget_settings.vertices_colour.redF(),
            widget_settings.vertices_colour.greenF(),
            widget_settings.vertices_colour.blueF());

  glVertexPointer(3, GL_DOUBLE, 0, &current_obj_->vertices[0]);
  glEnableClientState(GL_VERTEX_ARRAY);
  glDrawArrays(GL_POINTS, 0, current_obj_->vertices.size());

  glDisableClientState(GL_VERTEX_ARRAY);
}

void s21::GLWidget::drawEdges() {
  glLineWidth(widget_settings.edges_thickness);
  glLineStipple(1, 0x00F0);
  glColor3d(widget_settings.edges_colour.redF(),
            widget_settings.edges_colour.greenF(),
            widget_settings.edges_colour.blueF());
  if (widget_settings.is_edges_solid) {
    glDisable(GL_LINE_STIPPLE);
  } else {
    glEnable(GL_LINE_STIPPLE);
  }

  glVertexPointer(3, GL_DOUBLE, 0, &current_obj_->vertices[0]);
  glEnableClientState(GL_VERTEX_ARRAY);
  glDrawElements(GL_LINES, faces_in_lines_.size(), GL_UNSIGNED_INT,
                 &faces_in_lines_[0]);

  glDisableClientState(GL_VERTEX_ARRAY);
}

void s21::GLWidget::mousePressEvent(QMouseEvent* event) {
  lastPos = event->pos();
}

void s21::GLWidget::mouseMoveEvent(QMouseEvent* event) {
  double diff_x = event->pos().y() - lastPos.y();
  double diff_y = event->pos().x() - lastPos.x();
  lastPos = event->pos();
  emit mouseTrigger(diff_x, diff_y);
}

void s21::GLWidget::wheelEvent(QWheelEvent* event) {
  int increase_scale = 500;
  if (event->angleDelta().y() < 0) {
    increase_scale = -500;
  }
  emit wheelTrigger(increase_scale);
}

void s21::GLWidget::setProjection() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  GLfloat fov = 60.0 * M_PI / 180;
  GLfloat near = 1 / tan(fov / 2);
  if (widget_settings.is_parallel_projection) {
    glOrtho(-2., 2., -2., 2., -10, 10.);
  } else {
    glFrustum(-0.5, 0.5, -0.5, 0.5, near, 100.);
    glTranslated(0, 0, -near * 3);
  }
}

void s21::GLWidget::updateFrame() { update(); }

void s21::GLWidget::repaintObject(const s21::ViewerController::Object* obj,
                                  bool fullRepaint) {
  if (!fullRepaint) {
    current_obj_ = obj;
    update();
    return;
  }

  faces_in_lines_.clear();
  faces_in_lines_.reserve(obj->faces.size() * obj->faces[0].vertices_indices.size() * 2);
  for (const auto& f : obj->faces) {
    faces_in_lines_.push_back(f.vertices_indices[0]);
    for (size_t i = 1; i < f.vertices_indices.size(); ++i) {
      faces_in_lines_.push_back(f.vertices_indices[i]);
      faces_in_lines_.push_back(f.vertices_indices[i]);
    }
    faces_in_lines_.push_back(f.vertices_indices[0]);
  }

  update();
}
