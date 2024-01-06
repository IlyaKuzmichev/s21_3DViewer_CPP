#ifndef VIEW_H
#define VIEW_H

#include <QButtonGroup>
#include <QMainWindow>
#include <vector>
#include <cstdint>

#include "view/coloradapter.h"
#include "view/lineeditadapter.h"
#include "view/scrollbaradapter.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class View;
}
QT_END_NAMESPACE

class View : public QMainWindow {
  Q_OBJECT

 public:
  View(QWidget *parent = nullptr);
  ~View();

 signals:
  void repaintObject();
  void updateWidget();

 private slots:
  void setMouseRotation(double x, double y);
  void setWheelScale(int increase_scale);
  void setVF(uint64_t vertices, uint64_t faces);
  void on_button_open_clicked();
  void on_scroll_scale_valueChanged(int value);
  void on_line_scale_editingFinished();
  void on_radioButton_parallel_toggled(bool checked);
  void on_radioButton_edges_solid_toggled(bool checked);
  void on_slider_thickness_valueChanged(int value);
  void onRadioButtonDisplayPressed(int value);
  void on_slider_size_valueChanged(int value);
  void on_action_image_triggered();
  // void on_action_GIF_triggered();
  // void saveGifFrame();

 private:
  void SaveSettings();
  void LoadSettings();
  void translateConnector();
  void rotateConnector();
  void colourConnector();
  void groupButtons();

  Ui::View *ui;
  std::vector<std::unique_ptr<ScrollBarAdapter>> scrollBarAdapters;
  std::vector<std::unique_ptr<LineEditAdapter>> lineEditAdapters;
  std::vector<std::unique_ptr<ColorAdapter>> colorAdapters;
  QButtonGroup group;
  //  QGifImage *gif = nullptr;
  QTimer *timer = nullptr;
  uint8_t frame_counter = 0;
};
#endif  // VIEW_H
