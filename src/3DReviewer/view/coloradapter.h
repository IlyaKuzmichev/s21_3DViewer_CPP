#ifndef COLORADAPTER_H
#define COLORADAPTER_H

#include <QColor>
#include <QObject>

class ColorAdapter : public QObject {
  Q_OBJECT
 public:
  explicit ColorAdapter(QObject *parent, QColor *color)
      : QObject{parent}, color_{color} {}

  static std::unique_ptr<ColorAdapter> create(QObject *parent, QColor *color) {
    return std::make_unique<ColorAdapter>(parent, color);
  }

 private slots:
  void onButtonPressed();

 private:
  QColor *color_;
};

#endif  // COLORADAPTER_H
