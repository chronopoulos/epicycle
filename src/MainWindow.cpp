#include <QGridLayout>
#include <QDebug>
#include <QApplication>
#include <QDir>
#include <QFileDialog>

#include <unistd.h>

#include "sequoia.h"

#include "MainWindow.h"
#include "Editor.h"
#include "OutportWidget.h"
#include "Dialogs.h"
#include "Delta.h"

#define TPS 256
#define BPM 120

sq_session_t SESSION;

extern Delta DELTA;

MainWindow::MainWindow() : QWidget() {

    // initialize the global SESSION and main OUTPORT
    sq_session_init(&SESSION, "epicycle", TPS);
    sq_session_set_bpm(&SESSION, BPM);

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

    connect(&DELTA, SIGNAL(stateChanged(bool)), this, SLOT(handleDelta(bool)));

    DELTA.setState(false);

    resize(700,700);

    transport = STOPPED;

    sessionFile = QDir::homePath().append("/untitled.sqa");
    uninitialized = true;

}

void MainWindow::handleDelta(bool delta) {

    if (delta) {

        setWindowTitle(QString("epicycle*"));

    } else {

        setWindowTitle(QString("epicycle"));

    }

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

        } else if (e->key() == Qt::Key_S && (mod & Qt::ControlModifier)) {

            if (mod & Qt::ShiftModifier) {
                saveAs();
            } else {
                save();
            }

        } else if (e->key() == Qt::Key_O && (mod & Qt::ControlModifier)) {

            load();

        }

    }

    seqManager->phocusEvent(e); // phocus events have the possibility of auto-repeat

}

void MainWindow::closeEvent(QCloseEvent *e) {

    seqManager->clean();

    if (DELTA.state()) { // if there are changes, then ask to save

        MaybeSaveDialog dlg;
        switch (dlg.exec()) {
            case -1: // Discard
                e->accept();
                break;
            case 0: // Cancel
                e->ignore();
                break;
            case 1: // Save
                if (save()) {
                    e->accept();
                } else {
                    e->ignore();
                }
                break;
        }

    } else { // otherwise, just close

        e->accept();

    }

}

bool MainWindow::save(void) {

    if (uninitialized) {
        return saveAs();
    } else {
        return save(sessionFile);
    }

}

bool MainWindow::save(const QString &filename) {

    // open file
    QFile saveFile(filename);
    if (!saveFile.open(QIODevice::WriteOnly)) {

        qWarning("Couldn't open save file.");
        return false; // TODO present warning dialog

    }

    sq_session_save(&SESSION, filename.toStdString().c_str());

    // reset flags
    sessionFile = filename;
    DELTA.setState(false);
    uninitialized = false;

    return true;

}

bool MainWindow::saveAs(void) {

    QString filename = QFileDialog::getSaveFileName(Q_NULLPTR, "Save Session", sessionFile);
    if (filename.isNull()) {
        return false;
    }

    return save(filename);

}

void MainWindow::load(void) {

    // if there are changes to be saved, ask to save them
    if (DELTA.state()) {

        MaybeSaveDialog dlg;
        switch (dlg.exec()) {
            case -1: // Discard
                break;
            case 0: // Cancel
                return;
            case 1: // Save
                if (save()) {
                    break;
                } else {
                    return;
                }
        }

    }

    // TODO teardown current session (if there is one)

    QString filename = QFileDialog::getOpenFileName(Q_NULLPTR, "Open Session", QDir::homePath());

    if (!filename.isNull()) {
        load(filename);
    }

}

void MainWindow::load(const QString &filename) {

    // open file
    QFile loadFile(filename);
    if (!loadFile.open(QIODevice::ReadOnly)) {

        qWarning("Couldn't open load file."); // TODO present warning dialog
        return;

    }

    // TODO check if format is correct

    // clear out the current session
    clearSession();

    // load the new one

}

void MainWindow::clearSession(void) {

    if (!((transport == STOPPED) || (transport == PAUSED))) {
        sq_session_stop(&SESSION);
        transport = STOPPED;
    }

    sq_session_disconnect_jack(&SESSION);

    seqManager->clean(); // this stops the noti threads
    seqManager->removeAllEditors();

    outportManager->removeAllOutports();
    inportManager->removeAllInports();

}

