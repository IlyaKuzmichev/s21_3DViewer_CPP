#include "view/lineedit_adapter.h"

void LineEditAdapter::onScrollValueChanged(int value) {
  line_edit_->setText(QString::number(value));
}
