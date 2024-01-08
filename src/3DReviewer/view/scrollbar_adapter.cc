#include "scrollbar_adapter.h"

#include <QLineEdit>

void ScrollBarAdapter::onLineTranslateEditingFinished() {
  scroll_bar_->setValue(sender()->property("text").toInt());
}

void ScrollBarAdapter::onLineRotateEditingFinished() {
  int value = sender()->property("text").toInt();
  if (value > 0) {
    value = (value + 180) % 360 - 180;
  } else {
    value = (value - 180) % 360 + 180;
  }
  scroll_bar_->setValue(value);
}
