//TODO Use VAO's to support multiple models at once

#include <iostream>

#include <QtCore/qmath.h>
#include <QtGui/QGuiApplication>

#include "paramwindow.h"

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    ParamWindow window;
    window.resize(640, 480);
    window.show();

    window.setAnimating(true);

    return app.exec();
}
