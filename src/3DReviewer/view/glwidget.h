#ifndef GLWIDGET_H
#define GLWIDGET_H

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
  void loadNewModel(const QString filename);
  GLSettings WidgetSettings;

 private:
  QPoint lastPos;
  void setProjection();
  void drawVertices();
  void drawEdges();

 public slots:
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


#endif // GLWIDGET_H
