#include <QGridLayout>
#include <QDebug>

#include <unistd.h>

#include "MainWindow.h"

#include "sequoia.h"

#define TPS 256
#define BPM 120

sq_session_t SESSION;
sq_outport_t OUTPORT;

MainWindow::MainWindow() : QWidget() {

    // initialize the global SESSION and main OUTPORT
    sq_session_init(&SESSION, "epicycle", TPS);
    sq_session_set_bpm(&SESSION, 120);
    sq_outport_init(&OUTPORT, "main");
    sq_session_register_outport(&SESSION, &OUTPORT);

    // GUI stuff

    layout = new QGridLayout();

    manager = new Manager();

    layout->addWidget(manager, 0,0, 6,6);

    this->setLayout(layout);
    this->setWindowTitle("epicycle");

    resize(700,700);

    transport = STOPPED;

}

void MainWindow::togglePlayState(void) {

    if ((transport == STOPPED) || (transport == PAUSED)) {
        sq_session_start(&SESSION);
        transport = PLAYING;
    } else if (transport == PLAYING) {
        sq_session_stop(&SESSION);
        transport = STOPPED;
    }

}

void MainWindow::keyPressEvent(QKeyEvent *e) {

    if (!e->isAutoRepeat()) {

        if (e->key() == Qt::Key_Space) {
                togglePlayState();
        } else if (e->key() == Qt::Key_N) {
                manager->addEditor(new Editor());
        }

    }

}

void MainWindow::closeEvent(QCloseEvent*) {

    manager->clean();

}
