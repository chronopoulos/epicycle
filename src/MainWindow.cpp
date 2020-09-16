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
#include "Helper.h"

#define TPS 256
#define BPM 120

sq_session_t *SESSION;

extern Delta DELTA;

MainWindow::MainWindow() : QWidget() {

    // initialize the global SESSION and main OUTPORT
    SESSION = sq_session_new("epicycle", TPS);
    sq_session_set_bpm(SESSION, BPM);

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
    initialized = false;

}

void MainWindow::handleDelta(bool delta) {

    QString sessionName = QFileInfo(sessionFile).fileName();

    if (delta) {

        setWindowTitle(QString("%1*  -  epicycle").arg(sessionName));

    } else {

        if (sessionFile.isNull()) {

            setWindowTitle(QString("e p i c y c l e"));

        } else {

            setWindowTitle(QString("%1  -  epicycle").arg(sessionName));

        }

    }

}
void MainWindow::togglePlayState(void) {

    if ((transport == STOPPED) || (transport == PAUSED)) {
        sq_session_start(SESSION);
        transport = PLAYING;
    } else if (transport == PLAYING) {
        sq_session_stop(SESSION);
        transport = STOPPED;
    }

}

void MainWindow::keyPressEvent(QKeyEvent *e) {

    Qt::KeyboardModifiers mod = QApplication::keyboardModifiers();

    if (!e->isAutoRepeat()) {

        if (e->key() == Qt::Key_Space) {

            togglePlayState();

        } else if (e->key() == Qt::Key_N && !(mod & Qt::ShiftModifier)) {

            seqManager->addEditor(new Editor(newDefaultSequence()));
            DELTA.setState(true);

        } else if (e->key() == Qt::Key_P) {

            outportManager->addOutport(new OutportWidget(newDefaultOutport()));
            DELTA.setState(true);

        } else if (e->key() == Qt::Key_Q) {

            inportManager->addInport(new InportWidget(newDefaultInport()));
            DELTA.setState(true);

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

    if (initialized) {
        return save(sessionFile);
    } else {
        return saveAs();
    }

}

bool MainWindow::save(const QString &filename) {

    // open file
    QFile saveFile(filename);
    if (!saveFile.open(QIODevice::WriteOnly)) {

        qWarning("Couldn't open save file.");
        return false; // TODO present warning dialog

    }

    sq_session_save(SESSION, filename.toStdString().c_str());

    // reset flags
    sessionFile = filename;
    DELTA.setState(false);
    initialized = true;

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

    QString filename = QFileDialog::getOpenFileName(Q_NULLPTR, "Open Session", QDir::homePath());

    if (!filename.isNull()) {
        load(filename);
    }

}

void MainWindow::load(const QString &filename) {

    // tear down the current session
    clearSession();

    // TODO check if format is correct

    // load the new one
    SESSION = sq_session_load(filename.toStdString().c_str());

    // now build the GUI up from the sequoia session
    for (int i=0; i<SESSION->nseqs; i++) {
        sq_sequence_set_notifications(SESSION->seqs[i], true);
        seqManager->addEditor(new Editor(SESSION->seqs[i]));
    }
    for (int i=0; i<SESSION->ninports; i++) {
        inportManager->addInport(new InportWidget(SESSION->inports[i]));
    }
    for (int i=0; i<SESSION->noutports; i++) {
        outportManager->addOutport(new OutportWidget(SESSION->outports[i]));
    }

    // reset flags
    sessionFile = filename;
    DELTA.setState(false);
    initialized = true;

}

void MainWindow::clearSession(void) {

    if (!((transport == STOPPED) || (transport == PAUSED))) {
        transport = STOPPED;
    }

    sq_session_teardown(SESSION);

    seqManager->clean(); // this stops the noti threads

    seqManager->removeAllEditors();
    outportManager->removeAllOutports();
    inportManager->removeAllInports();

}

