#include "myglwidget.h"
extern "C" {
#include <stdio.h>

#include "../object.h"
}

MyGLWidget::MyGLWidget(QWidget* parent) : QOpenGLWidget(parent){};
void MyGLWidget::initializeGL() {
  // Set up the rendering context, load shaders and other resources, etc.:
  //    this->b_r = 0;
  //    this->b_g = 0;
  //    this->b_b = 0;
  //    this->e_r = 0;
  //    this->e_g = 1;
  //    this->e_b = 0;
  //    this->v_r = 0;
  //    this->v_g = 0;
  //    this->v_b = 1;
  //    this->projection_type = 0;
  //    this->edges_type = 0;
  //    this->vertices_type = 0;
  //    this->obj = NULL;
  //    this->edges_size = 1;
  //    this->vertices_size = 1;
  glEnable(GL_DEPTH_TEST);
}

void MyGLWidget::setupBackgroundColor(float r, float g, float b) {
  this->b_r = r / 255;
  this->b_g = g / 255;
  this->b_b = b / 255;
}

void MyGLWidget::setupEdgeColor(float r, float g, float b) {
  this->e_r = r / 255;
  this->e_g = g / 255;
  this->e_b = b / 255;
}

void MyGLWidget::setupVertexColor(float r, float g, float b) {
  this->v_r = r / 255;
  this->v_g = g / 255;
  this->v_b = b / 255;
}

void MyGLWidget::setupModel(object_t* obj) { this->obj = obj; }

void MyGLWidget::setupProjectionType(int value) {
  this->projection_type = value;
}

void MyGLWidget::setupEdgesType(int value) { this->edges_type = value; }

void MyGLWidget::setupVerticesType(int value) { this->vertices_type = value; }

object_t* MyGLWidget::getModel() { return this->obj; }

void MyGLWidget::setupEdgesSize(int value) { this->edges_size = value; }

void MyGLWidget::setupVerticesSize(int value) {
  this->vertices_size = value;
  update();
}

void MyGLWidget::resizeGL(int w, int h) { glViewport(0, 0, w, h); }

void MyGLWidget::paintGL() {
  glClearColor(this->b_r, this->b_g, this->b_b, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  if (this->obj) drawObj();
}

void MyGLWidget::drawObj() {
  glMatrixMode(GL_PROJECTION);
  glOrtho(-1, 1, -1, 1, -1, 1);

  if (this->projection_type == 0) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, -1, 1);
  }

  if (this->projection_type == 1) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1, 1, -1, 1, 2, 6);
    glTranslatef(0, 0, -3);
  }

  if (this->vertices_type != 0) {
    if (this->vertices_type == 1) {
      glEnable(GL_POINT_SMOOTH);
    } else {
      glDisable(GL_POINT_SMOOTH);
    }
    glColor3f(this->v_r, this->v_g, this->v_b);
    glPointSize(this->vertices_size);
    glBegin(GL_POINTS);
    for (unsigned int i = 0; i < obj->vertices_amount; i++) {
      glVertex3d(obj->vertices[i].x, obj->vertices[i].y, obj->vertices[i].z);
    }
    glEnd();
  }

  glLineWidth(this->edges_size);
  glLineStipple(1, 0x00F0);
  glColor3f(this->e_r, this->e_g, this->e_b);
  if (this->edges_type == 0) {
    glDisable(GL_LINE_STIPPLE);
  } else {
    glEnable(GL_LINE_STIPPLE);
  }
  glBegin(GL_LINES);
  for (unsigned int i = 0; i < this->obj->faces_amount; ++i) {
    const auto& face = this->obj->faces[i];

    const auto& first_point = face.vertices[0];
    glVertex3d(first_point->x, first_point->y, first_point->z);
    for (unsigned int j = 1; j < face.vertices_amount; ++j) {
      const auto& point = face.vertices[j];
      glVertex3d(point->x, point->y, point->z);
      glVertex3d(point->x, point->y, point->z);
    }
    glVertex3d(first_point->x, first_point->y, first_point->z);
  }
  glEnd();
}

void MyGLWidget::screen(QString filename, QString fileext) {
  this->grabFramebuffer().save(filename, fileext.toStdString().c_str());
}
