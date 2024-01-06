#include "view/view.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    View w;
    w.show();
    return app.exec();
}
