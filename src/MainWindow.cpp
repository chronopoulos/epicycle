#include <QGridLayout>
#include <QDebug>
#include <QApplication>

#include <unistd.h>

#include "sequoia.h"

#include "MainWindow.h"
#include "Editor.h"
#include "OutportWidget.h"

#define TPS 256
#define BPM 120

sq_session_t SESSION;

MainWindow::MainWindow() : QWidget() {

    // initialize the global SESSION and main OUTPORT
    sq_session_init(&SESSION, "epicycle", TPS);
    sq_session_set_bpm(&SESSION, 120);

    // GUI stuff

    layout = new QHBoxLayout();

    inportManager = new InportManager();
    seqManager = new SequenceManager();
    outportManager = new OutportManager();

    layout->addWidget(inportManager);
    layout->addWidget(seqManager);
    layout->addWidget(outportManager);

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

    Qt::KeyboardModifiers mod = QApplication::keyboardModifiers();

    if (!e->isAutoRepeat()) {

        if (e->key() == Qt::Key_Space) {
            togglePlayState();
        } else if (e->key() == Qt::Key_N && !(mod & Qt::ShiftModifier)) {
            seqManager->addEditor(new Editor());
        } else if (e->key() == Qt::Key_P) {
            outportManager->addOutport(new OutportWidget());
        } else if (e->key() == Qt::Key_Q) {
            inportManager->addInport(new InportWidget());
        }

    }

    seqManager->phocusEvent(e); // phocus events have the possibility of auto-repeat

}

void MainWindow::closeEvent(QCloseEvent*) {

    seqManager->clean();

}
