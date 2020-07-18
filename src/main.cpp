/*

    [epicycle]

    Chris Chronopoulos 20190317

*/

#include <QApplication>

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
    mainWindow.show();

    return app.exec();

}
