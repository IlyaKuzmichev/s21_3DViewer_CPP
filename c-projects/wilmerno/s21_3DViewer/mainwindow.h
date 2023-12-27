#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QButtonGroup>
#include <QMainWindow>
#include <memory>

extern "C" {
#include "../backend/3d_viewer.h"
#include "../backend/parser.h"
}

#include "ObjectParameters.h"
#include "QtGifImage/src/gifimage/qgifimage.h"
#include "coloradapter.h"
#include "lineeditadapter.h"
#include "scrollbaradapter.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

enum DisplayMethod { none = 0, circle, square };

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  void SaveSettings();
  void LoadSettings();

 signals:
  void repaintObject(ObjectParameters *params);

  void updateWidget();

 private slots:
  void on_button_open_clicked();
  void on_line_scale_returnPressed();
  void on_scroll_scale_valueChanged(int value);
  void updateParams(int);
  void setMouseRotation(double x, double y);
  void setWheelScale(int increase_scale);
  void on_radioButton_parallel_toggled(bool checked);
  void on_radioButton_edges_solid_toggled(bool checked);
  void on_slider_thickness_valueChanged(int value);
  void onRadioButtonDisplayPressed(int value);
  void on_slider_size_valueChanged(int value);
  void on_action_image_triggered();
  void on_action_GIF_triggered();
  void saveGifFrame();
  void setVF(uint64_t vertices, uint64_t faces);

 private:
  Ui::MainWindow *ui;
  std::vector<std::unique_ptr<ScrollBarAdapter>> scrollBarAdapters;
  std::vector<std::unique_ptr<LineEditAdapter>> lineEditAdapters;
  std::vector<std::unique_ptr<ColorAdapter>> colorAdapters;
  ObjectParameters params = {};
  void translateConnector();
  void rotateConnector();
  void colourConnector();
  QButtonGroup group;
  void groupButtons();
  QGifImage *gif = nullptr;
  QTimer *timer = nullptr;
  uint8_t frame_counter = 0;
};
#endif  // MAINWINDOW_H
