#include "view/view.h"

#include <QColorDialog>
#include <QFileDialog>
#include <QOpenGLWidget>
#include <QSettings>
#include <QTimer>
#include <initializer_list>
#include <tuple>

#include "./ui_view.h"
#include "controller/viewer_controller.h"

s21::View::View(ViewerController* controller, QWidget* parent)
    : QMainWindow(parent),
      ui(new Ui::View),
      controller_(controller),
      timer(new QTimer(0)) {
  ui->setupUi(this);
  LoadSettings();

  translateConnector();
  rotateConnector();
  colourConnector();

  connect(timer, SIGNAL(timeout()), this, SLOT(saveGifFrame()));
  connect(ui->scroll_scale, SIGNAL(valueChanged(int)), this,
          SLOT(updateParams(int)));
  connect(this,
          SIGNAL(repaintObject(const s21::ViewerController::Object*, bool)),
          ui->RendererWidget,
          SLOT(repaintObject(const s21::ViewerController::Object*, bool)));
  connect(ui->RendererWidget, &GLWidget::mouseTrigger, this,
          &s21::View::setMouseRotation);
  connect(ui->RendererWidget, &GLWidget::wheelTrigger, this,
          &s21::View::setWheelScale);
  connect(this, SIGNAL(updateWidget()), ui->RendererWidget,
          SLOT(updateFrame()));

  group.addButton(ui->radioButton_display_none, DisplayMethod::none);
  group.addButton(ui->radioButton_display_circle, DisplayMethod::circle);
  group.addButton(ui->radioButton_display_square, DisplayMethod::square);
  connect(&group, SIGNAL(idPressed(int)), this,
          SLOT(onRadioButtonDisplayPressed(int)));
}

s21::View::~View() {
  SaveSettings();
  if (gif) {
    delete gif;
  }
  delete timer;
  delete ui;
}

void s21::View::translateConnector() {
  for (auto& obj_pair :
       {std::make_pair(ui->line_translate_x, ui->scroll_translate_x),
        std::make_pair(ui->line_translate_y, ui->scroll_translate_y),
        std::make_pair(ui->line_translate_z, ui->scroll_translate_z)}) {
    lineEditAdapters.push_back(LineEditAdapter::create(this, obj_pair.first));
    connect(obj_pair.second, SIGNAL(valueChanged(int)),
            lineEditAdapters.back().get(), SLOT(onScrollValueChanged(int)));
    connect(obj_pair.second, SIGNAL(valueChanged(int)), this,
            SLOT(updateParams(int)));

    scrollBarAdapters.push_back(
        ScrollBarAdapter::create(this, obj_pair.second));
    connect(obj_pair.first, SIGNAL(editingFinished()),
            scrollBarAdapters.back().get(),
            SLOT(onLineTranslateEditingFinished()));
  }
}

void s21::View::rotateConnector() {
  for (auto& obj_pair :
       {std::make_pair(ui->line_rotate_x, ui->scroll_rotate_x),
        std::make_pair(ui->line_rotate_y, ui->scroll_rotate_y),
        std::make_pair(ui->line_rotate_z, ui->scroll_rotate_z)}) {
    lineEditAdapters.push_back(LineEditAdapter::create(this, obj_pair.first));
    connect(obj_pair.second, SIGNAL(valueChanged(int)),
            lineEditAdapters.back().get(), SLOT(onScrollValueChanged(int)));
    connect(obj_pair.second, SIGNAL(valueChanged(int)), this,
            SLOT(updateParams(int)));

    scrollBarAdapters.push_back(
        ScrollBarAdapter::create(this, obj_pair.second));
    connect(obj_pair.first, SIGNAL(editingFinished()),
            scrollBarAdapters.back().get(),
            SLOT(onLineRotateEditingFinished()));
  }
}

void s21::View::colourConnector() {
  for (auto& obj_pair :
       {std::make_pair(ui->pushButton_bg_colour,
                       &ui->RendererWidget->widget_settings.bg_colour),
        std::make_pair(ui->pushButton_edges_colour,
                       &ui->RendererWidget->widget_settings.edges_colour),
        std::make_pair(ui->pushButton_vertices_colour,
                       &ui->RendererWidget->widget_settings.vertices_colour)}) {
    colorAdapters.push_back(ColorAdapter::create(this, obj_pair.second));
    connect(obj_pair.first, SIGNAL(pressed()), colorAdapters.back().get(),
            SLOT(onButtonPressed()));
  }
}

void s21::View::SaveSettings() {
  QSettings settings("Aboba Team", "3DReviewer");
  settings.setValue("bg_colour", ui->RendererWidget->widget_settings.bg_colour);
  settings.setValue("edges_colour",
                    ui->RendererWidget->widget_settings.edges_colour);
  settings.setValue("vertices_colour",
                    ui->RendererWidget->widget_settings.vertices_colour);
  settings.setValue("projection",
                    ui->RendererWidget->widget_settings.is_parallel_projection);
  settings.setValue("edges_type",
                    ui->RendererWidget->widget_settings.is_edges_solid);
  settings.setValue("edges_thickness",
                    ui->RendererWidget->widget_settings.edges_thickness);
  settings.setValue("vertices_type",
                    ui->RendererWidget->widget_settings.vertices_type);
  settings.setValue("vertices_size",
                    ui->RendererWidget->widget_settings.vertices_size);
}

void s21::View::LoadSettings() {
  QSettings settings("Aboba Team", "3DReviewer");
  ui->RendererWidget->widget_settings.bg_colour =
      settings.value("bg_colour", QColor(Qt::white)).value<QColor>();
  ui->RendererWidget->widget_settings.edges_colour =
      settings.value("edges_colour", QColor(Qt::red)).value<QColor>();
  ui->RendererWidget->widget_settings.vertices_colour =
      settings.value("vertices_colour", QColor(Qt::black)).value<QColor>();
  ui->RendererWidget->widget_settings.is_parallel_projection =
      settings.value("projection", true).value<bool>();
  if (ui->RendererWidget->widget_settings.is_parallel_projection == false) {
    ui->radioButton_central->setChecked(true);
  }
  ui->RendererWidget->widget_settings.is_edges_solid =
      settings.value("edges_type", true).value<bool>();
  if (ui->RendererWidget->widget_settings.is_edges_solid == false) {
    ui->radioButton_edges_dashed->setChecked(true);
  }
  ui->RendererWidget->widget_settings.edges_thickness =
      settings.value("edges_thickness", 1.).value<GLfloat>();
  ui->slider_thickness->setValue(static_cast<int>(
      5. * ui->RendererWidget->widget_settings.edges_thickness));
  ui->RendererWidget->widget_settings.vertices_type =
      settings.value("vertices_type", 0.).value<int>();
  if (ui->RendererWidget->widget_settings.vertices_type !=
      DisplayMethod::none) {
    ui->RendererWidget->widget_settings.vertices_type == DisplayMethod::circle
        ? ui->radioButton_display_circle->setChecked(true)
        : ui->radioButton_display_square->setChecked(true);
  }
  ui->RendererWidget->widget_settings.vertices_size =
      settings.value("vertices_size", 1.).value<GLfloat>();
  ui->slider_size->setValue(
      static_cast<int>(ui->RendererWidget->widget_settings.vertices_size));
  ;
}

void s21::View::setVF(uint64_t vertices, uint64_t faces) {
  ui->line_vertex->setText(QString::number(vertices));
  ui->line_faces->setText(QString::number(faces));
}

void s21::View::setMouseRotation(double x, double y) {
  ui->line_rotate_x->setText(QString::number(ui->scroll_rotate_x->value() +
                                             static_cast<int>(x * 3. / 20.)));
  emit ui->line_rotate_x->editingFinished();

  ui->line_rotate_y->setText(QString::number(ui->scroll_rotate_y->value() +
                                             static_cast<int>(y * 3. / 20.)));
  emit ui->line_rotate_y->editingFinished();
}

void s21::View::setWheelScale(int increase_scale) {
  ui->scroll_scale->setValue(ui->scroll_scale->value() + increase_scale);
}

void s21::View::on_button_open_clicked() {
  QString filename = QFileDialog::getOpenFileName(this, "Open File", "/home",
                                                  "Object files (*.obj)");

  if (filename == ui->line_filepath->text()) {
    return;
  }
  controller_->LoadObject(filename);

  const auto& obj = controller_->GetObject();

  emit repaintObject(&obj, true);

  setVF(obj.vertices.size(), obj.faces.size());
  ui->line_filepath->setText(filename);
  ui->scroll_rotate_x->setValue(
      controller_->GetRotation(s21::ViewerController::Axis::kX));
  ui->scroll_rotate_y->setValue(
      controller_->GetRotation(s21::ViewerController::Axis::kY));
  ui->scroll_rotate_z->setValue(
      controller_->GetRotation(s21::ViewerController::Axis::kZ));
  ui->scroll_translate_x->setValue(
      controller_->GetTranslation(s21::ViewerController::Axis::kX));
  ui->scroll_translate_y->setValue(
      controller_->GetTranslation(s21::ViewerController::Axis::kY));
  ui->scroll_translate_z->setValue(
      controller_->GetTranslation(s21::ViewerController::Axis::kZ));
  ui->scroll_scale->setValue(controller_->GetScale());
}

void s21::View::on_scroll_scale_valueChanged(int value) {
  double scale =
      pow(10., static_cast<double>(value) / ui->scroll_scale->maximum());
  ui->line_scale->setText(QString::number(scale));
}

void s21::View::on_line_scale_editingFinished() {
  double scale =
      ui->scroll_scale->maximum() * log10(ui->line_scale->text().toDouble());
  ui->scroll_scale->setValue(static_cast<int>(scale));
}

void s21::View::on_radioButton_parallel_toggled(bool checked) {
  ui->RendererWidget->widget_settings.is_parallel_projection = checked;
  emit updateWidget();
}

void s21::View::on_radioButton_edges_solid_toggled(bool checked) {
  ui->RendererWidget->widget_settings.is_edges_solid = checked;
  emit updateWidget();
}

void s21::View::on_slider_thickness_valueChanged(int value) {
  ui->RendererWidget->widget_settings.edges_thickness =
      static_cast<GLfloat>(value / 5.);
  emit updateWidget();
}

void s21::View::onRadioButtonDisplayPressed(int value) {
  ui->RendererWidget->widget_settings.vertices_type = value;
  emit updateWidget();
}

void s21::View::on_slider_size_valueChanged(int value) {
  ui->RendererWidget->widget_settings.vertices_size =
      static_cast<GLfloat>(value);
  emit updateWidget();
}

void s21::View::on_action_image_triggered() {
  QString file = QFileDialog::getSaveFileName(
      this, "Save as...", "name", "BMP (*.bmp);; JPEG (*.jpg *.jpeg)");
  ui->RendererWidget->grab().save(file);
}

void s21::View::updateParams(int) {
  controller_->SetTranslation(
      s21::ViewerController::Axis::kX,
      static_cast<double>(ui->scroll_translate_x->value()) / 100.);
  controller_->SetTranslation(
      s21::ViewerController::Axis::kY,
      static_cast<double>(ui->scroll_translate_y->value()) / 100.);
  controller_->SetTranslation(
      s21::ViewerController::Axis::kZ,
      static_cast<double>(ui->scroll_translate_z->value()) / 100.);

  controller_->SetRotation(s21::ViewerController::Axis::kX,
                           ui->scroll_rotate_x->value() / 180.0 * M_PI);
  controller_->SetRotation(s21::ViewerController::Axis::kY,
                           ui->scroll_rotate_y->value() / 180.0 * M_PI);
  controller_->SetRotation(s21::ViewerController::Axis::kZ,
                           ui->scroll_rotate_z->value() / 180.0 * M_PI);

  controller_->SetScale(ui->line_scale->text().toDouble());

  emit repaintObject(&controller_->GetObject(), false);
}

void s21::View::on_action_GIF_triggered() {
  if (gif) {
    delete gif;
  }
  gif = new QGifImage;
  gif->setDefaultDelay(100);
  frame_counter = 0;
  timer->start(100);
}

void s21::View::saveGifFrame() {
  if (frame_counter < 50) {
    QImage frame = ui->RendererWidget->grabFramebuffer().scaled(
        640, 480, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    gif->addFrame(frame);
    ++frame_counter;
  } else {
    timer->stop();
    QString gifSavePath =
        QFileDialog::getSaveFileName(this, "Save as...", "name", "GIF(*.gif)");
    if (!gifSavePath.isNull()) {
      gif->save(gifSavePath);
    }
  }
}
