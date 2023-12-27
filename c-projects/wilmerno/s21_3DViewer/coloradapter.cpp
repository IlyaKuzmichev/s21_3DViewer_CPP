#include "coloradapter.h"

#include <QColorDialog>

void ColorAdapter::onButtonPressed() { *color_ = QColorDialog::getColor(); }
