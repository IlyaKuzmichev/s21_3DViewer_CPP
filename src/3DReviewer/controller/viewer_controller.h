#ifndef SRC_CONTROLLER_VIEWER_CONTROLLER_H_
#define SRC_CONTROLLER_VIEWER_CONTROLLER_H_

#include <QString>

#include "model/viewer.h"

namespace s21 {

  class ViewerController {
    public:
      using Axis = Viewer::Axis;
      using Object = Object;
      using Vertex = Vertex;

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
        viewer_->SetTranslation(axis, shift);
      }

      void SetScale(double scale) noexcept {
        viewer_->SetScale(scale);
      }

      double GetTranslation(Axis axis) const noexcept {
        return  viewer_->GetTranslation(axis);
      }

      double GetRotation(Axis axis) const noexcept {
        return viewer_->GetRotation(axis);
      }

      double GetScale() const noexcept {
        return viewer_->GetScale();
      }

    private:
      Viewer* viewer_;
  };


}  // namespace s21

#endif  // SRC_CONTROLLER_VIEWER_CONTROLLER_H_
