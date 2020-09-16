/*

    [epicycle]

    Chris Chronopoulos 20190317

*/

#include <QApplication>
#include <QDebug>

#include "MainWindow.h"
#include "Delta.h"

Delta DELTA;

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);
    app.setApplicationName("epicycle");

    QFont font("Monospace");
    font.setBold(true);
    font.setPointSize(10);
    app.setFont(font);

    MainWindow mainWindow;
    if (app.arguments().length() > 1) {
        mainWindow.load(app.arguments().at(1));
    }

    mainWindow.show();

    return app.exec();

}
