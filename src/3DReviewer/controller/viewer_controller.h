#ifndef SRC_CONTROLLER_VIEWER_CONTROLLER_H_
#define SRC_CONTROLLER_VIEWER_CONTROLLER_H_

#include <QString>

#include "model/viewer.h"

namespace s21 {

  class ViewerController {
    public:
      using Axis = Viewer::Axis;

      explicit ViewerController(Viewer* viewer) : viewer_(viewer) {}

      void LoadObject(const QString& filepath) {
        viewer_->LoadObject(filepath.toStdString());
      }

      const Object& GetObject() const noexcept {
        return viewer_->GetObject();
      }

      void SetRotation(Axis axis, double angle) noexcept {
        viewer_->SetRotation(axis, angle);
      }

      void SetTranslation(Axis axis, double shift) noexcept {
        viewer_->SetRotation(axis, shift);
      }

      void SetScale(double scale) noexcept {
        viewer_->SetScale(scale);
      }

    private:
      Viewer* viewer_;
  }


}

#endif  // SRC_CONTROLLER_VIEWER_CONTROLLER_H_