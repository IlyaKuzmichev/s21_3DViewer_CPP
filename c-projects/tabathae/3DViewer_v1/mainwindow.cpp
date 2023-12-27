#include "mainwindow.h"

#include <QDir>
#include <QFileDialog>
#include <string>

#include "./ui_mainwindow.h"

extern "C" {
#include <stdio.h>

#include "../object_builder.h"
#include "../object_parser.h"
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  read_settings();
  timer_for_gif = new QTimer(0);
  connect(timer_for_gif, SIGNAL(timeout()), this, SLOT(create_gif()));
}

MainWindow::~MainWindow() {
  write_settings();
  delete ui;
}

void MainWindow::write_settings() {
  QSettings settings("C_3DEngine");

  settings.setValue("init", 1);
  settings.setValue("b_r", ui->bg_red_spin->value());
  settings.setValue("b_g", ui->bg_green_spin->value());
  settings.setValue("b_b", ui->bg_blue_spin->value());
  settings.setValue("v_r", ui->vertices_red_spin->value());
  settings.setValue("v_g", ui->vertices_green_spin->value());
  settings.setValue("v_b", ui->vertices_blue_spin->value());
  settings.setValue("e_r", ui->edges_red_spin->value());
  settings.setValue("e_g", ui->edges_green_spin->value());
  settings.setValue("e_b", ui->edges_blue_spin->value());
  settings.setValue("projection_type",
                    ui->projection_type_combo_box->currentIndex());
  settings.setValue("edges_type", ui->edges_type_combo_box->currentIndex());
  settings.setValue("vertices_type",
                    ui->vertices_display_method_combo_box->currentIndex());
  settings.setValue("edges_size", ui->edges_width_slider->value());
  settings.setValue("vertices_size", ui->vertices_size_slider->value());
}

void MainWindow::read_settings() {
  QSettings settings("C_3DEngine");

  int is_setup = settings.value("init").toUInt();
  if (is_setup) {
    ui->bg_red_spin->setValue(settings.value("b_r").toUInt());
    ui->bg_green_spin->setValue(settings.value("b_g").toUInt());
    ui->bg_blue_spin->setValue(settings.value("b_b").toUInt());
    ui->widget->setupBackgroundColor(ui->bg_red_spin->value(),
                                     ui->bg_green_spin->value(),
                                     ui->bg_blue_spin->value());
    ui->vertices_red_spin->setValue(settings.value("v_r").toUInt());
    ui->vertices_green_spin->setValue(settings.value("v_g").toUInt());
    ui->vertices_blue_spin->setValue(settings.value("v_b").toUInt());
    ui->widget->setupVertexColor(ui->vertices_red_spin->value(),
                                 ui->vertices_green_spin->value(),
                                 ui->vertices_blue_spin->value());
    ui->edges_red_spin->setValue(settings.value("e_r").toUInt());
    ui->edges_green_spin->setValue(settings.value("e_g").toUInt());
    ui->edges_blue_spin->setValue(settings.value("e_b").toUInt());
    ui->widget->setupEdgeColor(ui->edges_red_spin->value(),
                               ui->edges_green_spin->value(),
                               ui->edges_blue_spin->value());
    ui->projection_type_combo_box->setCurrentIndex(
        settings.value("projection_type").toUInt());
    ui->widget->setupProjectionType(
        ui->projection_type_combo_box->currentIndex());
    ui->edges_type_combo_box->setCurrentIndex(
        settings.value("edges_type").toUInt());
    ui->widget->setupEdgesType(ui->edges_type_combo_box->currentIndex());
    ui->vertices_display_method_combo_box->setCurrentIndex(
        settings.value("vertices_type").toInt());
    ui->widget->setupVerticesType(
        ui->vertices_display_method_combo_box->currentIndex());
    ui->edges_width_slider->setValue(settings.value("edges_size").toInt());
    ui->widget->setupEdgesSize(ui->edges_width_slider->value());
    ui->vertices_size_slider->setValue(settings.value("vertices_size").toInt());
    ui->widget->setupVerticesSize(ui->vertices_size_slider->value());
  } else {
    ui->widget->setupBackgroundColor(0, 0, 0);
    ui->bg_red_spin->setValue(0);
    ui->bg_green_spin->setValue(0);
    ui->bg_blue_spin->setValue(0);
    ui->widget->setupVertexColor(0, 0, 255);
    ui->vertices_red_spin->setValue(0);
    ui->vertices_green_spin->setValue(0);
    ui->vertices_blue_spin->setValue(255);
    ui->widget->setupEdgeColor(0, 255, 0);
    ui->edges_red_spin->setValue(0);
    ui->edges_green_spin->setValue(255);
    ui->edges_blue_spin->setValue(0);
    ui->widget->setupProjectionType(0);
    ui->projection_type_combo_box->setCurrentIndex(0);
    ui->widget->setupEdgesType(0);
    ui->edges_type_combo_box->setCurrentIndex(0);
    ui->widget->setupVerticesType(0);
    ui->vertices_display_method_combo_box->setCurrentIndex(0);
    ui->widget->setupEdgesSize(1);
    ui->edges_width_slider->setValue(1);
    ui->widget->setupVerticesSize(1);
    ui->vertices_size_slider->setValue(1);
  }
  ui->widget->update();
}

void MainWindow::on_file_selection_button_clicked() {
  std::string filename;
  filename =
      QFileDialog::getOpenFileName(this, "Open file with object",
                                   QDir::homePath(), "Object files (*.obj)")
          .toStdString()
          .c_str();
  ui->selected_file_name_line_edit->setText(QString::fromStdString(filename));
  std::string tmp = filename.substr(filename.rfind("/") + 1);
  ui->objfile_name_line_edit->setText(QString::fromStdString(tmp));
  FILE *objfile = fopen(filename.c_str(), "r");
  if (NULL == objfile) {
    ui->error_line_edit->setText("File open ERROR");
  } else {
    object_builder_t *objbuilder = create_object_builder();
    if (NULL == objbuilder) {
      ui->error_line_edit->setText("Object creation ERROR");
    } else {
      int err = parse_to_builder(objfile, objbuilder);
      if (OK != err) {
        ui->error_line_edit->setText("Object parse ERROR");
      } else {
        err = get_object(objbuilder, &this->obj);
        if (OK != err) {
          ui->error_line_edit->setText("Get object ERROR");
        } else {
          ui->error_line_edit->setText("");
          ui->vertices_amount_line_edit->setText(
              QString::number(this->obj->vertices_amount));
          ui->edges_amount_line_edt->setText(
              QString::number(this->obj->faces_amount));
          normalize_obj();
          ui->widget->setupModel(this->obj);
          ui->widget->update();
        }
      }
    }
  }
}

void MainWindow::on_bg_red_spin_valueChanged(int arg1) {
  ui->widget->setupBackgroundColor(arg1, ui->bg_green_spin->value(),
                                   ui->bg_blue_spin->value());
  ui->widget->update();
}

void MainWindow::on_bg_green_spin_valueChanged(int arg1) {
  ui->widget->setupBackgroundColor(ui->bg_red_spin->value(), arg1,
                                   ui->bg_blue_spin->value());
  ui->widget->update();
}

void MainWindow::on_bg_blue_spin_valueChanged(int arg1) {
  ui->widget->setupBackgroundColor(ui->bg_red_spin->value(),
                                   ui->bg_green_spin->value(), arg1);
  ui->widget->update();
}

void MainWindow::on_edges_red_spin_valueChanged(int arg1) {
  ui->widget->setupEdgeColor(arg1, ui->edges_green_spin->value(),
                             ui->edges_blue_spin->value());
  ui->widget->update();
}

void MainWindow::on_edges_green_spin_valueChanged(int arg1) {
  ui->widget->setupEdgeColor(ui->edges_red_spin->value(), arg1,
                             ui->edges_blue_spin->value());
  ui->widget->update();
}

void MainWindow::on_edges_blue_spin_valueChanged(int arg1) {
  ui->widget->setupEdgeColor(ui->edges_red_spin->value(),
                             ui->edges_green_spin->value(), arg1);
  ui->widget->update();
}

void MainWindow::on_vertices_red_spin_valueChanged(int arg1) {
  ui->widget->setupVertexColor(arg1, ui->vertices_green_spin->value(),
                               ui->vertices_blue_spin->value());
  ui->widget->update();
}

void MainWindow::on_vertices_green_spin_valueChanged(int arg1) {
  ui->widget->setupVertexColor(ui->vertices_red_spin->value(), arg1,
                               ui->vertices_blue_spin->value());
  ui->widget->update();
}

void MainWindow::on_vertices_blue_spin_valueChanged(int arg1) {
  ui->widget->setupVertexColor(ui->vertices_red_spin->value(),
                               ui->vertices_green_spin->value(), arg1);
  ui->widget->update();
}

void MainWindow::on_projection_type_combo_box_currentIndexChanged(int index) {
  ui->widget->setupProjectionType(index);
  ui->widget->update();
}

void MainWindow::on_edges_type_combo_box_currentIndexChanged(int index) {
  ui->widget->setupEdgesType(index);
  ui->widget->update();
}

void MainWindow::on_vertices_display_method_combo_box_currentIndexChanged(
    int index) {
  ui->widget->setupVerticesType(index);
  ui->widget->update();
}

void MainWindow::on_move_x_up_button_clicked() {
  move_x(this->obj, 0.1);
  this->m_x += 0.1;
  ui->x_move_line_edit->setText(QString::number(this->m_x));
  ui->widget->update();
}

void MainWindow::on_move_x_down_button_clicked() {
  move_x(this->obj, -0.1);
  this->m_x -= 0.1;
  ui->x_move_line_edit->setText(QString::number(this->m_x));
  ui->widget->update();
}

void MainWindow::on_move_y_down_button_clicked() {
  move_y(this->obj, -0.1);
  this->m_y -= 0.1;
  ui->y_move_line_edit->setText(QString::number(this->m_y));
  ui->widget->update();
}

void MainWindow::on_move_y_up_button_clicked() {
  move_y(this->obj, 0.1);
  this->m_y += 0.1;
  ui->y_move_line_edit->setText(QString::number(this->m_y));
  ui->widget->update();
}

void MainWindow::on_move_z_down_button_clicked() {
  move_z(this->obj, -0.1);
  this->m_z -= 0.1;
  ui->z_move_line_edit->setText(QString::number(this->m_z));
  ui->widget->update();
}

void MainWindow::on_move_z_up_button_clicked() {
  move_z(this->obj, 0.1);
  this->m_z += 0.1;
  ui->z_move_line_edit->setText(QString::number(this->m_z));
  ui->widget->update();
}

void MainWindow::on_rotation_x_down_button_clicked() {
  rotate_x(this->obj, -0.1);
  this->r_x -= 0.1;
  ui->x_rotate_line_edit->setText(QString::number(this->r_x));
  ui->widget->update();
}

void MainWindow::on_rotation_x_up_button_clicked() {
  rotate_x(this->obj, 0.1);
  this->r_x += 0.1;
  ui->x_rotate_line_edit->setText(QString::number(this->r_x));
  ui->widget->update();
}

void MainWindow::on_rotation_y_down_button_clicked() {
  rotate_y(this->obj, -0.1);
  this->r_y -= 0.1;
  ui->y_rotate_line_edit->setText(QString::number(this->r_y));
  ui->widget->update();
}

void MainWindow::on_rotation_y_up_button_clicked() {
  rotate_y(this->obj, 0.1);
  this->r_y += 0.1;
  ui->y_rotate_line_edit->setText(QString::number(this->r_y));
  ui->widget->update();
}

void MainWindow::on_rotation_z_down_button_clicked() {
  rotate_z(this->obj, -0.1);
  this->r_z -= 0.1;
  ui->z_rotate_line_edit->setText(QString::number(this->r_z));
  ui->widget->update();
}

void MainWindow::on_rotation_z_up_button_clicked() {
  rotate_z(this->obj, 0.1);
  this->r_z += 0.1;
  ui->z_rotate_line_edit->setText(QString::number(this->r_z));
  ui->widget->update();
}

void MainWindow::on_edges_width_slider_valueChanged(int value) {
  ui->widget->setupEdgesSize(value);
  ui->widget->update();
}

void MainWindow::on_vertices_size_slider_valueChanged(int value) {
  ui->widget->setupVerticesSize(value);
  ui->widget->update();
}

void MainWindow::on_scale_down_button_clicked() {
  scale_x(this->obj, 0.9);
  scale_y(this->obj, 0.9);
  scale_z(this->obj, 0.9);
  this->scale *= 0.9;
  ui->scale_line_edit->setText(QString::number(this->scale));
  ui->widget->update();
}

void MainWindow::on_scale_up_button_clicked() {
  scale_x(this->obj, 1.1);
  scale_y(this->obj, 1.1);
  scale_z(this->obj, 1.1);
  this->scale *= 1.1;
  ui->scale_line_edit->setText(QString::number(this->scale));
  ui->widget->update();
}

void MainWindow::on_screen_button_clicked() {
  QString filter = "jpg;; bmp";
  QString fileName = QFileDialog::getSaveFileName(
      NULL, "save screenshot:", NULL, filter, &filter);
  if (!fileName.isNull()) {
    ui->widget->screen(fileName + "." + filter, filter);
  }
}

void MainWindow::on_gif_button_clicked() {
  if (!this->recording) {
    this->recording = 1;
    ui->error_line_edit->setText("RECORDING GIF");
    timer_for_gif->start(100);
    time = 0;
    create_gif();
  }
}

void MainWindow::create_gif() {
  if (recording == 1 && time <= 5.0) {
    mas_image.push_back(ui->widget->grab().toImage());
    time += 0.1;
  } else if (time >= 5.0) {
    save_gif();
    timer_for_gif->stop();
  }
}

void MainWindow::save_gif() {
  QString filter = "gif";
  QString fileName =
      QFileDialog::getSaveFileName(NULL, "save gif:", NULL, filter, &filter);
  if (!fileName.isNull()) {
    QGifImage gif(QSize(640, 480));
    gif.setDefaultTransparentColor(Qt::black);
    gif.setDefaultDelay(100);
    for (QVector<QImage>::Iterator img = mas_image.begin();
         img != mas_image.end(); ++img) {
      gif.addFrame(*img);
    }
    gif.save(fileName + "." + filter);
    mas_image.clear();
    time = 0.0;
    recording = 0;
    ui->error_line_edit->setText("");
  } else {
    ui->error_line_edit->setText("ERROR FILEPATH");
  }
}

void MainWindow::normalize_obj() {
  if (obj != NULL && obj->vertices_amount > 0) {
    float max_lenght = obj->vertices[0].x * obj->vertices[0].x +
                       obj->vertices[0].y * obj->vertices[0].y +
                       obj->vertices[0].z * obj->vertices[0].z;
    float tmp;
    for (unsigned int i = 1; i < obj->vertices_amount; i++) {
      tmp = obj->vertices[i].x * obj->vertices[i].x +
            obj->vertices[i].y * obj->vertices[i].y +
            obj->vertices[i].z * obj->vertices[i].z;
      if (tmp > max_lenght) max_lenght = tmp;
    }
    max_lenght = 1 / sqrtf(max_lenght);
    scale_x(obj, max_lenght);
    scale_y(obj, max_lenght);
    scale_z(obj, max_lenght);
  }
}
