#include <QGridLayout>
#include <QDebug>
#include <QApplication>
#include <QDir>
#include <QFileDialog>

#include <unistd.h> 
#include "sequoia.h"

#include "MainWindow.h"
#include "SequenceEditor.h"
#include "OutportWidget.h"
#include "Dialogs.h"
#include "Delta.h"
#include "Helper.h"

#define BPM_INITIAL 120.00

sq_session_t SESSION;
sq_trigger_t COPYBUF;
Delta DELTA;

MainWindow::MainWindow() : QWidget() {

    // initialize globals
    SESSION = sq_session_new("epicycle");
    COPYBUF = sq_trigger_new();

    // set default BPM
    sq_session_set_bpm(SESSION, BPM_INITIAL);

    // GUI stuff

    layout = new QHBoxLayout();
    leftLayout = new QVBoxLayout();

    inportManager = new InportManager();
    seqManager = new SequenceManager();
    outportManager = new OutportManager();
    transportWidget = new TransportWidget(BPM_INITIAL);

    leftLayout->addWidget(inportManager);
    leftLayout->addWidget(transportWidget);

    layout->addLayout(leftLayout);
    layout->addWidget(seqManager);
    layout->addWidget(outportManager);

    this->setLayout(layout);
    this->setWindowTitle("epicycle");
    this->setWindowIcon(QIcon(":/img/icon.png"));

    connect(&DELTA, SIGNAL(stateChanged(bool)), this, SLOT(handleDelta(bool)));
    connect(transportWidget, SIGNAL(stateChanged(int)), this, SLOT(handleTransport(int)));
    connect(transportWidget, SIGNAL(tempoChanged(double)), this, SLOT(handleTempo(double)));

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

void MainWindow::handleTransport(int state) {

    if (state == TransportWidget::Stopped) {
        sq_session_stop(SESSION);
    } else if (state == TransportWidget::Paused) {
        // TODO
    } else if (state == TransportWidget::Playing) {
        sq_session_start(SESSION);
    }

}

void MainWindow::handleTempo(double tempo) {

    sq_session_set_bpm(SESSION, tempo);
    DELTA.setState(true);

}

void MainWindow::keyPressEvent(QKeyEvent *e) {

    Qt::KeyboardModifiers mod = QApplication::keyboardModifiers();

    if (!e->isAutoRepeat()) {

        if (e->key() == Qt::Key_Space) {

            transportWidget->toggle();

        } else if (e->key() == Qt::Key_N && !(mod & Qt::ShiftModifier)) {

            seqManager->addSequenceEditor(new SequenceEditor(newDefaultSequence()));
            DELTA.setState(true);

        } else if (e->key() == Qt::Key_O) {

            if (mod & Qt::ControlModifier) {
                load();
            } else {
                outportManager->addOutport(new OutportWidget(newDefaultOutport()));
                DELTA.setState(true);
            }

        } else if (e->key() == Qt::Key_I) {

            inportManager->addInport(new InportWidget(newDefaultInport()));
            DELTA.setState(true);

        } else if (e->key() == Qt::Key_S && (mod & Qt::ControlModifier)) {

            if (mod & Qt::ShiftModifier) {
                saveAs();
            } else {
                save();
            }

        }

    }

    seqManager->phocusEvent(e); // phocus events have the possibility of auto-repeat

}

void MainWindow::closeEvent(QCloseEvent *e) {

    if (DELTA.state()) { // if there are changes, then ask to save

        MaybeSaveDialog dlg;
        switch (dlg.exec()) {
            case -1: // Discard
                seqManager->clean();
                e->accept();
                break;
            case 0: // Cancel
                e->ignore();
                break;
            case 1: // Save
                if (save()) {
                    seqManager->clean();
                    e->accept();
                } else {
                    e->ignore();
                }
                break;
        }

    } else { // otherwise, just close

        seqManager->clean();
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

    size_t i;

    // now build the GUI up from the sequoia session
    for (i=0; i<sq_session_get_nseqs(SESSION); i++) {
        sq_sequence_set_notifications(sq_session_get_seq(SESSION, i), true);
        seqManager->addSequenceEditor(new SequenceEditor(sq_session_get_seq(SESSION, i)));
    }
    for (i=0; i<sq_session_get_ninports(SESSION); i++) {
        inportManager->addInport(new InportWidget(sq_session_get_inport(SESSION, i)));
    }
    for (i=0; i<sq_session_get_noutports(SESSION); i++) {
        outportManager->addOutport(new OutportWidget(sq_session_get_outport(SESSION, i)));
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

    sq_session_delete_recursive(SESSION);

    seqManager->clean(); // this stops the noti threads

    seqManager->removeAllSequenceEditors();
    outportManager->removeAllOutports();
    inportManager->removeAllInports();

}

