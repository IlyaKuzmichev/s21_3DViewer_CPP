#include "view/glwidget.h"

#include <QDebug>
#include <QElapsedTimer>
#include "controller/viewer_controller.h"

s21::GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent), vertices_arr_size_(0), faces_arr_size_(0) {

}

void s21::GLWidget::initializeGL() {
    glEnable(GL_DEPTH_TEST);

    glGenBuffers(1, &vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
//    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STATIC_DRAW);

    glGenBuffers(1, &face_buffer_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, face_buffer_);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, nullptr, GL_STATIC_DRAW);
}

//void s21::GLWidget::resizeGL(int, int) {}

#include <QDebug>

void s21::GLWidget::paintGL() {
 QElapsedTimer debug;
  debug.start();
  setProjection();
  glClearColor(widget_settings.bg_colour.redF(),
               widget_settings.bg_colour.greenF(),
               widget_settings.bg_colour.blueF(), 1.0f);
  glClear(GL_COLOR_BUFFER_BIT |
          GL_DEPTH_BUFFER_BIT);  // очистка буфера изображения и глубины
  drawVertices();
  drawEdges();

  qDebug() << "Sizes:" << vertices_arr_size_ << " " << faces_arr_size_ << '\n';

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
    glColor3d(widget_settings.vertices_colour.redF(), widget_settings.vertices_colour.greenF(), widget_settings.vertices_colour.blueF());

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, sizeof(Vertex), 0);
    qDebug() << vertices_arr_size_ << '\n';
    glDrawArrays(GL_POINTS, 0, vertices_arr_size_);


    glDisableVertexAttribArray(0);
}

void s21::GLWidget::drawEdges() {
    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(faces_separator_);



    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, face_buffer_);
    glDrawElements(GL_TRIANGLE_FAN, faces_arr_size_, GL_UNSIGNED_INT, 0);

    glDisable(GL_PRIMITIVE_RESTART);
}

void s21::GLWidget::mousePressEvent(QMouseEvent* event) { lastPos = event->pos(); }

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
      glOrtho(-2., 2., -2., 2., -50, 50.);
    } else {
      glFrustum(-0.5, 0.5, -0.5, 0.5, 0.9, 100.);
      glTranslated(0, 0, -near * 3);
    }
}

void s21::GLWidget::updateFrame() { update(); }

void s21::GLWidget::repaintObject(const s21::ViewerController::Object* obj, bool fullRepaint) {
    if (fullRepaint) {
        vertices_arr_size_ = obj->vertices.size() / 3;
        glBufferData(vertex_buffer_, obj->vertices.size() * sizeof(double) * 3, &obj->vertices[0], GL_STATIC_DRAW);

        std::vector<int> all_indices;
        for (const auto& f : obj->faces) {
            all_indices.insert(all_indices.end(), f.vertices_indices.begin(), f.vertices_indices.end());
            all_indices.push_back(faces_separator_);
        }

        faces_arr_size_ = all_indices.size();
        glBufferData(face_buffer_, all_indices.size() * sizeof(int), &all_indices[0], GL_STATIC_DRAW);
    } else {
        glBufferSubData(vertex_buffer_, 0, obj->vertices.size() * sizeof(double) * 3, &obj->vertices[0]);
    }

    update();
}
