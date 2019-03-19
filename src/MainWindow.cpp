#include <QGridLayout>
#include <QDebug>

#include <unistd.h>

#include "MainWindow.h"

#include "sequoia.h"

sq_session_t sq_sesh;
sq_outport_t sq_port;
sq_sequence_t sq_seq;

MainWindow::MainWindow() : QWidget() {

    // for now, collect all the sequoia stuff here

    sq_session_init(&sq_sesh, "epicycle", 256);
    sq_session_set_bpm(&sq_sesh, 120);

    sq_outport_init(&sq_port, "perc");
    sq_session_register_outport(&sq_sesh, &sq_port);

    sq_sequence_init(&sq_seq, 16, sq_sesh.tps);
    sq_trigger_t kick;
    sq_trigger_init(&kick);
    sq_trigger_set_note(&kick, 60, 100, 0.5);
    for (int i=0; i<16; i++) {
        if (i%4==0) {
            sq_sequence_set_trig(&sq_seq, i, &kick);
        }
    }
    sq_sequence_set_outport(&sq_seq, &sq_port);
    sq_session_add_sequence(&sq_sesh, &sq_seq);

    sq_sequence_set_notifications(&sq_seq, true);

    // end sequoia

    layout = new QGridLayout();

    manager = new Manager();
    editor = new Editor();

    layout->addWidget(manager, 0,0, 5,6);
    layout->addWidget(editor, 6,0, 1,6);

    this->setLayout(layout);
    this->setWindowTitle("epicycle");

    resize(700,700);

    transport = STOPPED;

    notiThread = new NotificationThread(this, &sq_seq);
    connect(notiThread, SIGNAL(playheadUpdated(int)), editor, SLOT(updatePlayhead(int)));
    notiThread->start();

}

void MainWindow::togglePlayState(void) {

    if ((transport == STOPPED) || (transport == PAUSED)) {
        sq_session_start(&sq_sesh);
        transport = PLAYING;
    } else if (transport == PLAYING) {
        sq_session_stop(&sq_sesh);
        transport = STOPPED;
    }

}

void MainWindow::keyPressEvent(QKeyEvent *e) {

    if (!e->isAutoRepeat()) {

        if (e->key() == Qt::Key_Space) {

                togglePlayState();
                return;

        }

    }

}

void MainWindow::closeEvent(QCloseEvent*) {

    notiThread->requestStop();
    notiThread->wait();

}
