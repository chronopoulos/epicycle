#include <QDebug>
#include <QLabel>
#include <QMenu>

#include "Editor.h"
#include "Helper.h"

extern sq_session_t SESSION;

Editor::Editor(void) : QFrame() {

    // UI stuff

    m_nsteps = 16;

    setFrameStyle(QFrame::Box | QFrame::Plain);
    setLineWidth(2);

    QVBoxLayout *layout = new QVBoxLayout();
    QHBoxLayout *topLayout = new QHBoxLayout();
    QHBoxLayout *midLayout = new QHBoxLayout();
    QHBoxLayout *bottomLayout = new QHBoxLayout();

    playheadIndicator = nullptr;
    lBracketIndicator = nullptr;
    rBracketIndicator = nullptr;

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

        tmpButton = new Button(i);
        buttons.push_back(tmpButton);
        connect(tmpButton, SIGNAL(trigUpdated(int, sq_trigger_t*)),
                this, SLOT(setTrig(int, sq_trigger_t*)));
        midLayout->addWidget(tmpButton);

        tmpIndicator = new Indicator(i);
        indicators.push_back(tmpIndicator);
        connect(tmpIndicator, SIGNAL(lBracketSet(int)), this, SLOT(updateLBracket(int)));
        connect(tmpIndicator, SIGNAL(rBracketSet(int)), this, SLOT(updateRBracket(int)));
        connect(tmpIndicator, SIGNAL(playheadRequested(int)), this, SLOT(setPlayhead(int)));
        if (i==0) {
            tmpIndicator->setLBracket(true);
        }
        if (i==(m_nsteps-1)) tmpIndicator->setRBracket(true);
        bottomLayout->addWidget(tmpIndicator);

    }

    layout->addLayout(topLayout);
    layout->addLayout(midLayout);
    layout->addLayout(bottomLayout);
    setLayout(layout);

    updatePlayhead(0);

    // sequoia data

    sq_sequence_init(&m_seq, 16, 256);
    sq_sequence_set_name(&m_seq, getRandomString(4).toStdString().c_str());
    //sq_sequence_set_outport(&m_seq, &OUTPORT);
    sq_sequence_set_notifications(&m_seq, true);
    sq_session_add_sequence(&SESSION, &m_seq);

    // notifications
    notiThread = new NotificationThread(this, &m_seq);
    connect(notiThread, SIGNAL(playheadUpdated(int)), this, SLOT(updatePlayhead(int)));
    notiThread->start();

    // initialize the ClickLabel's
    nameLabel->setValue(m_seq.name);
    transposeLabel->setValue(m_seq.transpose);
    clockDivLabel->setValue(m_seq.div);
    muteLabel->setValue(m_seq.mute ? "True" : "False");

    // connect the click labels

    connect(nameLabel, SIGNAL(valueChanged(QString)), this, SLOT(setName(QString)));

    connect(transposeLabel, SIGNAL(valueChanged(int)), this, SLOT(setTranspose(int)));
    connect(notiThread, SIGNAL(transposeUpdated(int)), transposeLabel, SLOT(setValue(int)));

    connect(clockDivLabel, SIGNAL(valueChanged(int)), this, SLOT(setClockDivide(int)));
    connect(notiThread, SIGNAL(clockDivideUpdated(int)), clockDivLabel, SLOT(setValue(int)));

    connect(muteLabel, SIGNAL(valueChanged(QString)), this, SLOT(setMute(QString)));
    connect(notiThread, SIGNAL(muteUpdated(QString)), muteLabel, SLOT(setValue(QString)));

}

void Editor::updatePlayhead(int step) {

    if (playheadIndicator) playheadIndicator->setPlayhead(false);
    playheadIndicator = indicators[step];
    playheadIndicator->setPlayhead(true);

}

void Editor::updateLBracket(int step) {


    if (lBracketIndicator) lBracketIndicator->setLBracket(false);
    lBracketIndicator = indicators[step];
    //emit lBracketChanged(step);

}

void Editor::updateRBracket(int step) {

    if (rBracketIndicator) rBracketIndicator->setRBracket(false);
    rBracketIndicator = indicators[step];
    //emit rBracketChanged(step);

}

void Editor::clean(void) {

    notiThread->requestStop();
    notiThread->wait();

}

void Editor::setName(QString name) {

    sq_sequence_set_name(&m_seq, name.toStdString().c_str());
    nameLabel->setValue(name); // there are no notifications for name changes

}

void Editor::setTrig(int step, sq_trigger_t *trig) {

    sq_sequence_set_trig(&m_seq, step, trig);

}

void Editor::setTranspose(int transpose) {

    sq_sequence_set_transpose(&m_seq, transpose);

}

void Editor::setPlayhead(int ph) {

    sq_sequence_set_playhead(&m_seq, ph);

}

void Editor::setClockDivide(int div) {

    sq_sequence_set_clockdivide(&m_seq, div);

}

void Editor::setMute(QString mute) {

    if (mute == "True") {
        sq_sequence_set_mute(&m_seq, true);
    } else {
        sq_sequence_set_mute(&m_seq, false);
    }

}

void Editor::contextMenuEvent(QContextMenuEvent*) {

    QMenu menu;

    std::vector<QAction*> actions;
    std::vector<QAction*>::iterator actionIter;

    // kind of a dirty way to select the outport, but it works

    sq_outport_t *outport;
    for (int i=0; i<SESSION.noutports; i++) {
        outport = SESSION.outports[i];
        actions.push_back(menu.addAction(outport->name));
    }

    QAction *selectedAction = menu.exec(QCursor::pos());

    // get the index
    actionIter = std::find(actions.begin(), actions.end(), selectedAction);
    if (actionIter != actions.end()) {
        int iselected =  std::distance(actions.begin(), actionIter);
        sq_sequence_set_outport(&m_seq, SESSION.outports[iselected]);
    }

}
