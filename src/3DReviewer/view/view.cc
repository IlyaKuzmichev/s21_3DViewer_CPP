#include "view/view.h"

#include <QColorDialog>
#include <QFileDialog>
#include <QOpenGLWidget>
#include <QSettings>
#include <QTimer>
#include <initializer_list>
#include <tuple>

#include "./ui_view.h"

View::View(QWidget* parent) : QMainWindow(parent), ui(new Ui::View) {
  ui->setupUi(this);
  LoadSettings();

  translateConnector();
  rotateConnector();
  colourConnector();

  connect(timer, SIGNAL(timeout()), this, SLOT(saveGifFrame()));
//  connect(ui->scroll_scale, SIGNAL(valueChanged(int)), this,
//          SLOT(updateParams(int)));
//  connect(this, SIGNAL(repaintObject(ObjectParameters*)), ui->RendererWidget,
//          SLOT(UpdateObject(ObjectParameters*)));
  connect(ui->RendererWidget, &GLWidget::mouseTrigger, this,
          &View::setMouseRotation);
  connect(ui->RendererWidget, &GLWidget::wheelTrigger, this,
          &View::setWheelScale);
  connect(this, SIGNAL(updateWidget()), ui->RendererWidget,
          SLOT(updateFrame()));
  connect(ui->RendererWidget, SIGNAL(sendVF(uint64_t,uint64_t)), this,
          SLOT(setVF(uint64_t,uint64_t)));

  group.addButton(ui->radioButton_display_none, DisplayMethod::none);
  group.addButton(ui->radioButton_display_circle, DisplayMethod::circle);
  group.addButton(ui->radioButton_display_square, DisplayMethod::square);
  connect(&group, SIGNAL(idPressed(int)), this,
          SLOT(onRadioButtonDisplayPressed(int)));
}

View::~View() {
  SaveSettings();
  //    if (gif) {
  //      delete gif;
  //    }
  //    delete timer;
  delete ui;
}

void View::translateConnector() {
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

void View::rotateConnector() {
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
            scrollBarAdapters.back().get(), SLOT(onLineRotateEditingFinished()));
  }
}

void View::colourConnector() {
  for (auto& obj_pair :
       {std::make_pair(ui->pushButton_bg_colour,
                       &ui->RendererWidget->WidgetSettings.bg_colour),
        std::make_pair(ui->pushButton_edges_colour,
                       &ui->RendererWidget->WidgetSettings.edges_colour),
        std::make_pair(ui->pushButton_vertices_colour,
                       &ui->RendererWidget->WidgetSettings.vertices_colour)}) {
    colorAdapters.push_back(ColorAdapter::create(this, obj_pair.second));
    connect(obj_pair.first, SIGNAL(pressed()), colorAdapters.back().get(),
            SLOT(onButtonPressed()));
  }
}

void View::SaveSettings() {
  QSettings settings("Aboba Team", "3DReviewer");
  settings.setValue("bg_colour", ui->RendererWidget->WidgetSettings.bg_colour);
  settings.setValue("edges_colour",
                    ui->RendererWidget->WidgetSettings.edges_colour);
  settings.setValue("vertices_colour",
                    ui->RendererWidget->WidgetSettings.vertices_colour);
  settings.setValue("projection",
                    ui->RendererWidget->WidgetSettings.is_parallel_projection);
  settings.setValue("edges_type",
                    ui->RendererWidget->WidgetSettings.is_edges_solid);
  settings.setValue("edges_thickness",
                    ui->RendererWidget->WidgetSettings.edges_thickness);
  settings.setValue("vertices_type",
                    ui->RendererWidget->WidgetSettings.vertices_type);
  settings.setValue("vertices_size",
                    ui->RendererWidget->WidgetSettings.vertices_size);
}

void View::LoadSettings() {
  QSettings settings("Aboba Team", "3DViewer");
  ui->RendererWidget->WidgetSettings.bg_colour =
      settings.value("bg_colour", QColor(Qt::white)).value<QColor>();
  ui->RendererWidget->WidgetSettings.edges_colour =
      settings.value("edges_colour", QColor(Qt::red)).value<QColor>();
  ui->RendererWidget->WidgetSettings.vertices_colour =
      settings.value("vertices_colour", QColor(Qt::black)).value<QColor>();
  ui->RendererWidget->WidgetSettings.is_parallel_projection =
      settings.value("projection", true).value<bool>();
  if (ui->RendererWidget->WidgetSettings.is_parallel_projection == false) {
    ui->radioButton_central->setChecked(true);
  }
  ui->RendererWidget->WidgetSettings.is_edges_solid =
      settings.value("edges_type", true).value<bool>();
  if (ui->RendererWidget->WidgetSettings.is_edges_solid == false) {
    ui->radioButton_edges_dashed->setChecked(true);
  }
  ui->RendererWidget->WidgetSettings.edges_thickness =
      settings.value("edges_thickness", 1.).value<GLfloat>();
  ui->slider_thickness->setValue(static_cast<int>(
      5. * ui->RendererWidget->WidgetSettings.edges_thickness));
  ui->RendererWidget->WidgetSettings.vertices_type =
      settings.value("vertices_type", 0.).value<int>();
  if (ui->RendererWidget->WidgetSettings.vertices_type != DisplayMethod::none) {
    ui->RendererWidget->WidgetSettings.vertices_type == DisplayMethod::circle
        ? ui->radioButton_display_circle->setChecked(true)
        : ui->radioButton_display_square->setChecked(true);
  }
  ui->RendererWidget->WidgetSettings.vertices_size =
      settings.value("vertices_size", 1.).value<GLfloat>();
  ui->slider_size->setValue(
      static_cast<int>(ui->RendererWidget->WidgetSettings.vertices_size));
  ;
}

void View::setVF(uint64_t vertices, uint64_t faces) {
  ui->line_vertex->setText(QString::number(vertices));
  ui->line_faces->setText(QString::number(faces));
}

void View::setMouseRotation(double x, double y) {
  ui->line_rotate_x->setText(QString::number(ui->scroll_rotate_x->value() +
                                             static_cast<int>(x * 3. / 20.)));
  emit ui->line_rotate_x->returnPressed();

  ui->line_rotate_y->setText(QString::number(ui->scroll_rotate_y->value() +
                                             static_cast<int>(y * 3. / 20.)));
  emit ui->line_rotate_y->returnPressed();
}

void View::setWheelScale(int increase_scale) {
  ui->scroll_scale->setValue(ui->scroll_scale->value() + increase_scale);
}

void View::on_button_open_clicked() {
  QString fileName = QFileDialog::getOpenFileName(this, "Open File", "/home",
                                                  "Object files (*.obj)");
  if (!fileName.isEmpty()) {
    ui->line_filepath->setText(fileName);
    ui->scroll_rotate_x->setValue(0);
    ui->scroll_rotate_y->setValue(0);
    ui->scroll_rotate_z->setValue(0);
    ui->scroll_translate_x->setValue(0);
    ui->scroll_translate_y->setValue(0);
    ui->scroll_translate_z->setValue(0);
    ui->scroll_scale->setValue(0);
    ui->RendererWidget->loadNewModel(fileName);
  }
}

void View::on_scroll_scale_valueChanged(int value) {
//    ui->scroll_scale->maximum();
  ui->line_scale->setText(
      QString::number(pow(10., static_cast<double>(value / 10000.))));
}

void View::on_line_scale_editingFinished() {
  ui->scroll_scale->setValue(
      static_cast<int>(10000. * log10(ui->line_scale->text().toDouble())));
}

void View::on_radioButton_parallel_toggled(bool checked) {
  ui->RendererWidget->WidgetSettings.is_parallel_projection = checked;
  emit updateWidget();
}

void View::on_radioButton_edges_solid_toggled(bool checked) {
  ui->RendererWidget->WidgetSettings.is_edges_solid = checked;
  emit updateWidget();
}

void View::on_slider_thickness_valueChanged(int value) {
  ui->RendererWidget->WidgetSettings.edges_thickness =
      static_cast<GLfloat>(value / 5.);
  emit updateWidget();
}

void View::onRadioButtonDisplayPressed(int value) {
  ui->RendererWidget->WidgetSettings.vertices_type = value;
  emit updateWidget();
}

void View::on_slider_size_valueChanged(int value) {
  ui->RendererWidget->WidgetSettings.vertices_size =
      static_cast<GLfloat>(value);
  emit updateWidget();
}

void View::on_action_image_triggered() {
  QString file = QFileDialog::getSaveFileName(
      this, "Save as...", "name", "BMP (*.bmp);; JPEG (*.jpg *.jpeg)");
  ui->RendererWidget->grab().save(file);
}

// void View::updateParams(int) {
//   params.translate_x =
//       static_cast<double>(ui->scroll_translate_x->value()) / 100.;
//   params.translate_y =
//       static_cast<double>(ui->scroll_translate_y->value()) / 100.;
//   params.translate_z =
//       static_cast<double>(ui->scroll_translate_z->value()) / 100.;

//  params.rotate_x = ui->scroll_rotate_x->value() / 180.0 * M_PI;
//  params.rotate_y = ui->scroll_rotate_y->value() / 180.0 * M_PI;
//  params.rotate_z = ui->scroll_rotate_z->value() / 180.0 * M_PI;

//  params.scale = ui->line_scale->text().toDouble();

//  emit repaintObject(&params);
//}

// void View::on_action_GIF_triggered() {
//   if (gif) {
//     delete gif;
//   }
//   gif = new QGifImage;
//   gif->setDefaultDelay(100);
//   frame_counter = 0;
//   timer->start(100);
// }

// void View::saveGifFrame() {
//   if (frame_counter < 50) {
//     QImage frame = ui->RendererWidget->grabFramebuffer().scaled(
//         640, 480, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
//     gif->addFrame(frame);
//     ++frame_counter;
//   } else {
//     timer->stop();
//     QString gifSavePath =
//         QFileDialog::getSaveFileName(this, "Save as...", "name", "GIF
//         (*.gif)");
//     if (!gifSavePath.isNull()) {
//       gif->save(gifSavePath);
//     }
//   }
// }
