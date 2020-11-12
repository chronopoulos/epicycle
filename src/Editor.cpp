#include <QDebug>
#include <QLabel>
#include <QMenu>
#include <QApplication>

#include "Editor.h"
#include "Helper.h"
#include "Delta.h"

extern sq_session_t SESSION;
extern Delta DELTA;

Editor::Editor(sq_sequence_t seq) : QFrame() {

    m_seq = seq;

    // UI stuff

    m_nsteps = 16;

    setFrameStyle(QFrame::Box | QFrame::Plain);
    setLineWidth(2);

    QVBoxLayout *layout = new QVBoxLayout();
    QHBoxLayout *topLayout = new QHBoxLayout();
    QHBoxLayout *midLayout = new QHBoxLayout();
    QHBoxLayout *bottomLayout = new QHBoxLayout();

    playheadIndicator = nullptr;
    firstIndicator = nullptr;
    lastIndicator = nullptr;

    // ClickLabel's
    nameLabel = new ClickLabel("N: %1", ClickLabel::Type_QString, "Set Name", "Name:");
    topLayout->addWidget(nameLabel);
    transposeLabel = new ClickLabel("T: %1", ClickLabel::Type_Int, "Set Transpose", "Transpose:");
    topLayout->addWidget(transposeLabel);
    clockDivLabel = new ClickLabel("D: %1", ClickLabel::Type_Int, "Set Clock Divide", "Divisor:");
    topLayout->addWidget(clockDivLabel);

    muteLabel = new ClickLabel("M: %1", ClickLabel::Type_Item, "Set Mute", "Mute:");
    QStringList muteItems;
    muteItems << "False" << "True";
    muteLabel->setItems(muteItems);
    topLayout->addWidget(muteLabel);

    Button *tmpButton;
    Indicator *tmpIndicator;
    for (int i=0; i < m_nsteps; i++) {

        // create and add button
        tmpButton = new Button(i, sq_sequence_get_trig(m_seq, i), this);
        buttons.push_back(tmpButton);
        midLayout->addWidget(tmpButton);

        // create and add indicator
        tmpIndicator = new Indicator(i, this);
        indicators.push_back(tmpIndicator);
        if (i==sq_sequence_get_first(m_seq)) {
            tmpIndicator->setFirst(true);
            firstIndicator = tmpIndicator;
        }
        if (i==sq_sequence_get_last(m_seq)) {
            tmpIndicator->setLast(true);
            lastIndicator = tmpIndicator;
        }
        bottomLayout->addWidget(tmpIndicator);

        // connect them up
        connect(tmpButton, SIGNAL(trigUpdated(int, sq_trigger_t)),
                this, SLOT(setTrig(int, sq_trigger_t)));
        connect(tmpIndicator, SIGNAL(firstRequested(int)), this, SLOT(setFirst(int)));
        connect(tmpIndicator, SIGNAL(lastRequested(int)), this, SLOT(setLast(int)));
        connect(tmpIndicator, SIGNAL(playheadRequested(int)), this, SLOT(setPlayhead(int)));

    }

    layout->addLayout(topLayout);
    layout->addLayout(midLayout);
    layout->addLayout(bottomLayout);
    setLayout(layout);

    updatePlayhead(0);

    // notifications
    notiThread = new NotificationThread(m_seq, this);

    // initialize the ClickLabel's
    nameLabel->setValue(sq_sequence_get_name(m_seq));
    transposeLabel->setValue(sq_sequence_get_transpose(m_seq));
    clockDivLabel->setValue(sq_sequence_get_clockdivide(m_seq));
    muteLabel->setValue(sq_sequence_get_mute(m_seq) ? "True" : "False");

    // connect the click labels

    connect(nameLabel, SIGNAL(valueChanged(QString)), this, SLOT(setName(QString)));

    connect(transposeLabel, SIGNAL(valueChanged(int)), this, SLOT(setTranspose(int)));
    connect(notiThread, SIGNAL(transposeUpdated(int)), transposeLabel, SLOT(setValue(int)));

    connect(clockDivLabel, SIGNAL(valueChanged(int)), this, SLOT(setClockDivide(int)));
    connect(notiThread, SIGNAL(clockDivideUpdated(int)), clockDivLabel, SLOT(setValue(int)));

    connect(muteLabel, SIGNAL(valueChanged(QString)), this, SLOT(setMute(QString)));
    connect(notiThread, SIGNAL(muteUpdated(QString)), muteLabel, SLOT(setValue(QString)));

    // step-wise notifications
    connect(notiThread, SIGNAL(playheadUpdated(int)), this, SLOT(updatePlayhead(int)));
    connect(notiThread, SIGNAL(firstUpdated(int)), this, SLOT(updateFirst(int)));
    connect(notiThread, SIGNAL(lastUpdated(int)), this, SLOT(updateLast(int)));

    notiThread->start();

    m_phocus = false;

    m_editParameter = Button::Edit_NoteValue;

    phocusIndex = 0;

}

void Editor::paintEvent(QPaintEvent *e) {

    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(0,160,255));
    setAutoFillBackground(true);
    setPalette(pal);

    QFrame::paintEvent(e);

}

void Editor::updatePlayhead(int step) {

    if (playheadIndicator) playheadIndicator->setPlayhead(false);
    playheadIndicator = indicators[step];
    playheadIndicator->setPlayhead(true);

}

void Editor::updateFirst(int step) {

    if (firstIndicator) firstIndicator->setFirst(false);
    firstIndicator = indicators[step];
    firstIndicator->setFirst(true);

}

void Editor::updateLast(int step) {

    if (lastIndicator) lastIndicator->setLast(false);
    lastIndicator = indicators[step];
    lastIndicator->setLast(true);

}

void Editor::clean(void) {

    notiThread->requestStop();
    notiThread->wait();

}

void Editor::setName(QString name) {

    sq_sequence_set_name(m_seq, name.toStdString().c_str());
    nameLabel->setValue(name); // there are no notifications for name changes
    DELTA.setState(true);

}

void Editor::setTrig(int step, sq_trigger_t trig) {

    sq_sequence_set_trig(m_seq, step, trig);
    DELTA.setState(true);

}

void Editor::setTranspose(int transpose) {

    sq_sequence_set_transpose(m_seq, transpose);
    DELTA.setState(true);

}

void Editor::setPlayhead(int ph) {

    sq_sequence_set_playhead(m_seq, ph);
    DELTA.setState(true);

}

void Editor::setClockDivide(int div) {

    sq_sequence_set_clockdivide(m_seq, div);
    DELTA.setState(true);

}

void Editor::setMute(QString mute) {

    if (mute == "True") {
        sq_sequence_set_mute(m_seq, true);
    } else {
        sq_sequence_set_mute(m_seq, false);
    }

    DELTA.setState(true);

}

void Editor::setFirst(int first) {

    sq_sequence_set_first(m_seq, first);

    DELTA.setState(true);

}

void Editor::setLast(int last) {

    sq_sequence_set_last(m_seq, last);

    DELTA.setState(true);

}

void Editor::contextMenuEvent(QContextMenuEvent*) {

    QMenu menu;

    std::vector<QAction*> actions;
    std::vector<QAction*>::iterator actionIter;

    // kind of a dirty way to select the outport, but it works

    sq_outport_t outport;
    for (size_t i=0; i<sq_session_get_noutports(SESSION); i++) {
        outport = sq_session_get_outport(SESSION, i);
        actions.push_back(menu.addAction(sq_outport_get_name(outport)));
    }

    QAction *selectedAction = menu.exec(QCursor::pos());

    // get the index
    actionIter = std::find(actions.begin(), actions.end(), selectedAction);
    if (actionIter != actions.end()) {
        int iselected =  std::distance(actions.begin(), actionIter);
        sq_sequence_set_outport(m_seq, sq_session_get_outport(SESSION, iselected));
        DELTA.setState(true);
    }

}

void Editor::cycleEditParameter(void) {

    int newParameter = Button::Edit_NoteValue;

    if (m_editParameter == Button::Edit_NoteValue) {
        newParameter = Button::Edit_NoteVelocity;
    } else if (m_editParameter == Button::Edit_NoteVelocity){
        newParameter = Button::Edit_NoteLength;
    } else if (m_editParameter == Button::Edit_NoteLength){
        newParameter = Button::Edit_Microtime;
    } else if (m_editParameter == Button::Edit_Microtime){
        newParameter = Button::Edit_Probability;
    } else if (m_editParameter == Button::Edit_Probability){
        newParameter = Button::Edit_NoteValue;
    }

    setEditParameter(newParameter);

}

void Editor::setEditParameter(int parameter) {

    for (int i=0; i<m_nsteps; i++) {
        buttons[i]->setEditParameter(parameter);
    }

    m_editParameter = parameter;

}

void Editor::phocusEvent(QKeyEvent *e) {

    Qt::KeyboardModifiers mod = QApplication::keyboardModifiers();

    if (e->key() == Qt::Key_L) {
        advancePhocus(1);
    } else if (e->key() == Qt::Key_H) {
        advancePhocus(-1);
    } else if (e->key() == Qt::Key_Tab) {
        cycleEditParameter();
    }

    if (mod & Qt::ShiftModifier) {
        if (e->key() == Qt::Key_N) {
            nameLabel->runDialog();
        } else if (e->key() == Qt::Key_T) {
            transposeLabel->runDialog();
        } else if (e->key() == Qt::Key_D) {
            clockDivLabel->runDialog();
        } else if (e->key() == Qt::Key_M) {
            setMute(sq_sequence_get_mute(m_seq) ? "False" : "True");
        }
    }

    if (phocusIndex >= 0) {
        buttons[phocusIndex]->phocusEvent(e);
    }

}

void Editor::setPhocus(bool phocus) {

    buttons[phocusIndex]->setPhocus(phocus);
    m_phocus = phocus;
    update();

}

void Editor::advancePhocus(int increment) {

    if (phocusIndex >= 0) {

        buttons[phocusIndex]->setPhocus(false);
        phocusIndex = phocusIndex + increment;
        if (phocusIndex >= m_nsteps) phocusIndex = m_nsteps - 1;
        if (phocusIndex < 0) phocusIndex = 0;
        buttons[phocusIndex]->setPhocus(true);

    }

}
