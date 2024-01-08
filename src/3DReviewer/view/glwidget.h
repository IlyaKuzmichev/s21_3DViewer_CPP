#ifndef GLWIDGET_H
#define GLWIDGET_H

#define GL_SILENCE_DEPRECATION

#include <QObject>
#include <QWidget>


#include <QColor>
#include <QLineEdit>
#include <QMouseEvent>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWidget>
#include <QWheelEvent>
#include <QtMath>

#include "controller/viewer_controller.h"

namespace s21 {

enum DisplayMethod { none = 0, circle, square };

struct  GLSettings{
  QColor bg_colour;
  QColor edges_colour;
  QColor vertices_colour;
  bool is_edges_solid = true;
  bool is_parallel_projection = true;
  int vertices_type = DisplayMethod::none;
  GLfloat edges_thickness = 1.;
  GLfloat vertices_size = 1.;
};

class GLWidget : public QOpenGLWidget {
  Q_OBJECT

 public:
  GLWidget(QWidget *parent = nullptr);
  ~GLWidget() {}
  GLSettings widget_settings;

 private:
  QPoint lastPos;

  GLuint vertex_buffer_;
  GLuint face_buffer_;

  size_t vertices_arr_size_;
  size_t faces_arr_size_;
  static constexpr GLuint faces_separator_ = 0xFFFF;


  void setProjection();
  void drawVertices();
  void drawEdges();

 public slots:
  void updateFrame();
  void repaintObject(const s21::ViewerController::Object* obj, bool fullRepaint);

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

}  // namespace s21



#endif // GLWIDGET_H
