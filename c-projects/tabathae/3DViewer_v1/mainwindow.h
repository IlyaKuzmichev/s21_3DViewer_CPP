#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QTimer>

#include "./QtGifImage-master/src/gifimage/qgifimage.h"

extern "C" {
#include "../object.h"
}

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private slots:
  void read_settings();

  void write_settings();

  void on_file_selection_button_clicked();

  void on_bg_red_spin_valueChanged(int arg1);

  void on_bg_green_spin_valueChanged(int arg1);

  void on_bg_blue_spin_valueChanged(int arg1);

  void on_edges_red_spin_valueChanged(int arg1);

  void on_edges_green_spin_valueChanged(int arg1);

  void on_edges_blue_spin_valueChanged(int arg1);

  void on_vertices_red_spin_valueChanged(int arg1);

  void on_vertices_green_spin_valueChanged(int arg1);

  void on_vertices_blue_spin_valueChanged(int arg1);

  void on_projection_type_combo_box_currentIndexChanged(int index);

  void on_edges_type_combo_box_currentIndexChanged(int index);

  void on_vertices_display_method_combo_box_currentIndexChanged(int index);

  void on_move_x_up_button_clicked();

  void on_move_x_down_button_clicked();

  void on_move_y_down_button_clicked();

  void on_move_y_up_button_clicked();

  void on_move_z_down_button_clicked();

  void on_move_z_up_button_clicked();

  void on_rotation_x_down_button_clicked();

  void on_rotation_x_up_button_clicked();

  void on_rotation_y_down_button_clicked();

  void on_rotation_y_up_button_clicked();

  void on_rotation_z_down_button_clicked();

  void on_rotation_z_up_button_clicked();

  void on_edges_width_slider_valueChanged(int value);

  void on_vertices_size_slider_valueChanged(int value);

  void on_scale_down_button_clicked();

  void on_scale_up_button_clicked();

  void on_screen_button_clicked();

  void on_gif_button_clicked();

  void create_gif();

  void save_gif();

  void normalize_obj();

 private:
  Ui::MainWindow *ui;
  object_t *obj;
  float scale = 1;
  float m_x = 0;
  float m_y = 0;
  float m_z = 0;
  float r_x = 0;
  float r_y = 0;
  float r_z = 0;
  int recording = 0;
  QTimer *timer_for_gif;
  double time = 0;
  QVector<QImage> mas_image;
};
#endif  // MAINWINDOW_H
