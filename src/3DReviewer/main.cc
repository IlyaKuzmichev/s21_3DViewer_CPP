#include "model/viewer.h"
#include "view/view.h"
#include "controller/viewer_controller.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    s21::Viewer viewer;
    s21::ViewerController controller(&viewer);
    s21::View view(&controller);
    view.show();
    return app.exec();
}
