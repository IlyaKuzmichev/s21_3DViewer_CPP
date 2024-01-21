#ifndef GLWIDGET_H
#define GLWIDGET_H

#define GL_SILENCE_DEPRECATION

#include <QColor>
#include <QDebug>
#include <QLineEdit>
#include <QMouseEvent>
#include <QObject>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWidget>
#include <QWheelEvent>
#include <QWidget>
#include <QtMath>
#include <vector>

#include "controller/viewer_controller.h"

namespace s21 {

enum DisplayMethod { none = 0, circle, square };

struct GLSettings {
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
  class RepaintStrategy;
  class FullRepaintStrategy;
  class UpdateOnlyRepaintStrategy;

  explicit GLWidget(QWidget* parent = nullptr);
  ~GLWidget() {}

  GLSettings widget_settings;

 public slots:
  void updateFrame();
  void repaintObject(const s21::ViewerController::Object* obj,
                     s21::GLWidget::RepaintStrategy* strategy);

 signals:
  void mouseTrigger(double x, double y);
  void wheelTrigger(int increase_scale);
  void sendVF(uint64_t vertices, uint64_t faces);

 protected:
  void initializeGL() override;
  void paintGL() override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void wheelEvent(QWheelEvent* event) override;

 private:
  void setProjection();
  void drawVertices();
  void drawEdges();

  QPoint lastPos;
  std::vector<uint32_t> faces_in_lines_;
  const s21::ViewerController::Object* current_obj_;
};

class GLWidget::RepaintStrategy {
 public:
  virtual void Repaint(const s21::ViewerController::Object* obj,
                       std::vector<uint32_t>& faces_in_lines,
                       QOpenGLWidget* glwidget) = 0;
  virtual ~RepaintStrategy() = default;
};

class GLWidget::FullRepaintStrategy : public RepaintStrategy {
 public:
  FullRepaintStrategy(const FullRepaintStrategy& other) = delete;
  void operator=(const FullRepaintStrategy& other) = delete;

  static FullRepaintStrategy* GetInstance() {
    if (fp_strategy_ == nullptr) {
      fp_strategy_ = new FullRepaintStrategy();
    }
    return fp_strategy_;
  }

  void Repaint(const s21::ViewerController::Object* obj,
               std::vector<uint32_t>& faces_in_lines,
               QOpenGLWidget* glwidget) override {
    faces_in_lines.clear();
    faces_in_lines.reserve(obj->faces.size() *
                           obj->faces[0].vertices_indices.size() * 2);
    for (const auto& f : obj->faces) {
      faces_in_lines.push_back(f.vertices_indices[0]);
      for (size_t i = 1; i < f.vertices_indices.size(); ++i) {
        faces_in_lines.push_back(f.vertices_indices[i]);
        faces_in_lines.push_back(f.vertices_indices[i]);
      }
      faces_in_lines.push_back(f.vertices_indices[0]);
    }
    glwidget->update();
  }

 private:
  FullRepaintStrategy() = default;
  static FullRepaintStrategy* fp_strategy_;
};

class GLWidget::UpdateOnlyRepaintStrategy : public RepaintStrategy {
 public:
  UpdateOnlyRepaintStrategy(const UpdateOnlyRepaintStrategy& other) = delete;
  void operator=(const UpdateOnlyRepaintStrategy& other) = delete;

  static UpdateOnlyRepaintStrategy* GetInstance() {
    if (uo_strategy_ == nullptr) {
      uo_strategy_ = new UpdateOnlyRepaintStrategy();
    }
    return uo_strategy_;
  }

  void Repaint(const s21::ViewerController::Object*, std::vector<uint32_t>&,
               QOpenGLWidget* glwidget) override {
    glwidget->update();
  }

 private:
  UpdateOnlyRepaintStrategy() = default;
  static UpdateOnlyRepaintStrategy* uo_strategy_;
};

}  // namespace s21

#endif  // GLWIDGET_H
