#ifndef VIEW_H
#define VIEW_H

#include <QButtonGroup>
#include <QMainWindow>
#include <cstdint>
#include <memory>
#include <vector>

#include "controller/viewer_controller.h"
#include "gif_lib/QtGifImage/include/gifimage/qgifimage.h"
#include "view/coloradapter.h"
#include "view/glwidget.h"
#include "view/lineedit_adapter.h"
#include "view/scrollbar_adapter.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class View;
}
QT_END_NAMESPACE

namespace s21 {
class View : public QMainWindow {
  Q_OBJECT

 public:
  explicit View(ViewerController *controller, QWidget *parent = nullptr);
  ~View();

 signals:
  void repaintObject(const s21::ViewerController::Object *object,
                     s21::GLWidget::RepaintStrategy *strategy);
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
  void updateParams(int);
  void on_action_GIF_triggered();
  void saveGifFrame();

 private:
  void SaveSettings();
  void LoadSettings();
  void translateConnector();
  void rotateConnector();
  void colourConnector();
  void groupButtons();

  Ui::View *ui;
  s21::ViewerController *controller_;
  std::vector<std::unique_ptr<ScrollBarAdapter>> scrollBarAdapters;
  std::vector<std::unique_ptr<LineEditAdapter>> lineEditAdapters;
  std::vector<std::unique_ptr<ColorAdapter>> colorAdapters;
  QButtonGroup group;
  QGifImage *gif = nullptr;
  QTimer *timer = nullptr;
  uint8_t frame_counter = 0;
};

}  // namespace s21
#endif  // VIEW_H
