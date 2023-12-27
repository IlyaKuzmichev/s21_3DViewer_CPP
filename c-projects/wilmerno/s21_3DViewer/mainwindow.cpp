#include "mainwindow.h"

#include <QColorDialog>
#include <QFileDialog>
#include <QOpenGLWidget>
#include <QSettings>
#include <QTimer>
#include <initializer_list>
#include <tuple>

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), timer(new QTimer(0)) {
  ui->setupUi(this);
  LoadSettings();

  translateConnector();
  rotateConnector();
  colourConnector();

  connect(timer, SIGNAL(timeout()), this, SLOT(saveGifFrame()));
  connect(ui->scroll_scale, SIGNAL(valueChanged(int)), this,
          SLOT(updateParams(int)));
  connect(this, SIGNAL(repaintObject(ObjectParameters*)), ui->GLWidget,
          SLOT(UpdateObject(ObjectParameters*)));
  connect(ui->GLWidget, &MyGLWidget::mouseTrigger, this,
          &MainWindow::setMouseRotation);
  connect(ui->GLWidget, &MyGLWidget::wheelTrigger, this,
          &MainWindow::setWheelScale);
  connect(this, SIGNAL(updateWidget()), ui->GLWidget, SLOT(updateFrame()));
  connect(ui->GLWidget, SIGNAL(sendVF(uint64_t, uint64_t)), this,
          SLOT(setVF(uint64_t, uint64_t)));

  group.addButton(ui->radioButton_display_none, DisplayMethod::none);
  group.addButton(ui->radioButton_display_circle, DisplayMethod::circle);
  group.addButton(ui->radioButton_display_square, DisplayMethod::square);
  connect(&group, SIGNAL(idPressed(int)), this,
          SLOT(onRadioButtonDisplayPressed(int)));
}

MainWindow::~MainWindow() {
  SaveSettings();
  if (gif) {
    delete gif;
  }
  delete timer;
  delete ui;
}

void MainWindow::translateConnector() {
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
    connect(obj_pair.first, SIGNAL(returnPressed()),
            scrollBarAdapters.back().get(),
            SLOT(onLineTranslateReturnPressed()));
  }
}

void MainWindow::rotateConnector() {
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
    connect(obj_pair.first, SIGNAL(returnPressed()),
            scrollBarAdapters.back().get(), SLOT(onLineRotateReturnPressed()));
  }
}

void MainWindow::colourConnector() {
  for (auto& obj_pair :
       {std::make_pair(ui->pushButton_bg_colour,
                       &ui->GLWidget->WidgetSettings.bg_colour),
        std::make_pair(ui->pushButton_edges_colour,
                       &ui->GLWidget->WidgetSettings.edges_colour),
        std::make_pair(ui->pushButton_vertices_colour,
                       &ui->GLWidget->WidgetSettings.vertices_colour)}) {
    colorAdapters.push_back(ColorAdapter::create(this, obj_pair.second));
    connect(obj_pair.first, SIGNAL(pressed()), colorAdapters.back().get(),
            SLOT(onButtonPressed()));
  }
}

void MainWindow::SaveSettings() {
  QSettings settings("Aboba Team", "3DViewer");
  settings.setValue("bg_colour", ui->GLWidget->WidgetSettings.bg_colour);
  settings.setValue("edges_colour", ui->GLWidget->WidgetSettings.edges_colour);
  settings.setValue("vertices_colour",
                    ui->GLWidget->WidgetSettings.vertices_colour);
  settings.setValue("projection",
                    ui->GLWidget->WidgetSettings.is_parallel_projection);
  settings.setValue("edges_type", ui->GLWidget->WidgetSettings.is_edges_solid);
  settings.setValue("edges_thickness",
                    ui->GLWidget->WidgetSettings.edges_thickness);
  settings.setValue("vertices_type",
                    ui->GLWidget->WidgetSettings.vertices_type);
  settings.setValue("vertices_size",
                    ui->GLWidget->WidgetSettings.vertices_size);
}

void MainWindow::LoadSettings() {
  QSettings settings("Aboba Team", "3DViewer");
  ui->GLWidget->WidgetSettings.bg_colour =
      settings.value("bg_colour", QColor(Qt::white)).value<QColor>();
  ui->GLWidget->WidgetSettings.edges_colour =
      settings.value("edges_colour", QColor(Qt::red)).value<QColor>();
  ui->GLWidget->WidgetSettings.vertices_colour =
      settings.value("vertices_colour", QColor(Qt::black)).value<QColor>();
  ui->GLWidget->WidgetSettings.is_parallel_projection =
      settings.value("projection", true).value<bool>();
  if (ui->GLWidget->WidgetSettings.is_parallel_projection == false) {
    ui->radioButton_central->setChecked(true);
  }
  ui->GLWidget->WidgetSettings.is_edges_solid =
      settings.value("edges_type", true).value<bool>();
  if (ui->GLWidget->WidgetSettings.is_edges_solid == false) {
    ui->radioButton_edges_dashed->setChecked(true);
  }
  ui->GLWidget->WidgetSettings.edges_thickness =
      settings.value("edges_thickness", 1.).value<GLfloat>();
  ui->slider_thickness->setValue(
      static_cast<int>(5. * ui->GLWidget->WidgetSettings.edges_thickness));
  ui->GLWidget->WidgetSettings.vertices_type =
      settings.value("vertices_type", 0.).value<int>();
  if (ui->GLWidget->WidgetSettings.vertices_type != DisplayMethod::none) {
    ui->GLWidget->WidgetSettings.vertices_type == DisplayMethod::circle
        ? ui->radioButton_display_circle->setChecked(true)
        : ui->radioButton_display_square->setChecked(true);
  }
  ui->GLWidget->WidgetSettings.vertices_size =
      settings.value("vertices_size", 1.).value<GLfloat>();
  ui->slider_size->setValue(
      static_cast<int>(ui->GLWidget->WidgetSettings.vertices_size));
  ;
}

void MainWindow::setVF(uint64_t vertices, uint64_t faces) {
  ui->line_vertex->setText(QString::number(vertices));
  ui->line_faces->setText(QString::number(faces));
}

void MainWindow::setMouseRotation(double x, double y) {
  ui->line_rotate_x->setText(QString::number(ui->scroll_rotate_x->value() +
                                             static_cast<int>(x * 3. / 20.)));
  emit ui->line_rotate_x->returnPressed();

  ui->line_rotate_y->setText(QString::number(ui->scroll_rotate_y->value() +
                                             static_cast<int>(y * 3. / 20.)));
  emit ui->line_rotate_y->returnPressed();
}

void MainWindow::setWheelScale(int increase_scale) {
  ui->scroll_scale->setValue(ui->scroll_scale->value() + increase_scale);
}

void MainWindow::on_button_open_clicked() {
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
    ui->GLWidget->loadNewModel(fileName);
  }
}

void MainWindow::on_scroll_scale_valueChanged(int value) {
  ui->line_scale->setText(
      QString::number(pow(10., static_cast<double>(value / 100.))));
}

void MainWindow::on_line_scale_returnPressed() {
  ui->scroll_scale->setValue(
      static_cast<int>(10. * log10(ui->line_scale->text().toDouble())));
}

void MainWindow::updateParams(int) {
  params.translate_x =
      static_cast<double>(ui->scroll_translate_x->value()) / 100.;
  params.translate_y =
      static_cast<double>(ui->scroll_translate_y->value()) / 100.;
  params.translate_z =
      static_cast<double>(ui->scroll_translate_z->value()) / 100.;

  params.rotate_x = ui->scroll_rotate_x->value() / 180.0 * M_PI;
  params.rotate_y = ui->scroll_rotate_y->value() / 180.0 * M_PI;
  params.rotate_z = ui->scroll_rotate_z->value() / 180.0 * M_PI;

  params.scale = ui->line_scale->text().toDouble();

  emit repaintObject(&params);
}

void MainWindow::on_radioButton_parallel_toggled(bool checked) {
  ui->GLWidget->WidgetSettings.is_parallel_projection = checked;
  emit updateWidget();
}

void MainWindow::on_radioButton_edges_solid_toggled(bool checked) {
  ui->GLWidget->WidgetSettings.is_edges_solid = checked;
  emit updateWidget();
}

void MainWindow::on_slider_thickness_valueChanged(int value) {
  ui->GLWidget->WidgetSettings.edges_thickness =
      static_cast<GLfloat>(value / 5.);
  emit updateWidget();
}

void MainWindow::onRadioButtonDisplayPressed(int value) {
  ui->GLWidget->WidgetSettings.vertices_type = value;
  emit updateWidget();
}

void MainWindow::on_slider_size_valueChanged(int value) {
  ui->GLWidget->WidgetSettings.vertices_size = static_cast<GLfloat>(value);
  emit updateWidget();
}

void MainWindow::on_action_image_triggered() {
  QString file = QFileDialog::getSaveFileName(
      this, "Save as...", "name", "BMP (*.bmp);; JPEG (*.jpg *.jpeg)");
  ui->GLWidget->grab().save(file);
}

void MainWindow::on_action_GIF_triggered() {
  if (gif) {
    delete gif;
  }
  gif = new QGifImage;
  gif->setDefaultDelay(100);
  frame_counter = 0;
  timer->start(100);
}

void MainWindow::saveGifFrame() {
  if (frame_counter < 50) {
    QImage frame = ui->GLWidget->grabFramebuffer().scaled(
        640, 480, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    gif->addFrame(frame);
    ++frame_counter;
  } else {
    timer->stop();
    QString gifSavePath =
        QFileDialog::getSaveFileName(this, "Save as...", "name", "GIF (*.gif)");
    if (!gifSavePath.isNull()) {
      gif->save(gifSavePath);
    }
  }
}
