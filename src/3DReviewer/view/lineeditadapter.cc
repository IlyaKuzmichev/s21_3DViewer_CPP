#include "lineeditadapter.h"

void LineEditAdapter::onScrollValueChanged(int value) {
  line_edit_->setText(QString::number(value));
}
