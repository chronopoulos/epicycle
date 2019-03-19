#include <QDebug>

#include "Editor.h"

extern sq_session_t SESSION;
extern sq_outport_t OUTPORT;

Editor::Editor(void) : QFrame() {

    m_nsteps = 16;

    setFrameStyle(QFrame::Box | QFrame::Plain);
    setLineWidth(1);

    QVBoxLayout *layout = new QVBoxLayout();
    QHBoxLayout *topLayout = new QHBoxLayout();
    QHBoxLayout *bottomLayout = new QHBoxLayout();

    playheadIndicator = nullptr;
    lBracketIndicator = nullptr;
    rBracketIndicator = nullptr;

    Button *tmpButton;
    Indicator *tmpIndicator;
    for (int i=0; i < m_nsteps; i++) {

        tmpButton = new Button(i);
        buttons.push_back(tmpButton);
        connect(tmpButton, SIGNAL(trigUpdated(int, sq_trigger_t*)),
                this, SLOT(updateTrig(int, sq_trigger_t*)));
        topLayout->addWidget(tmpButton);

        tmpIndicator = new Indicator(i);
        indicators.push_back(tmpIndicator);
        connect(tmpIndicator, SIGNAL(lBracketSet(int)), this, SLOT(updateLBracket(int)));
        connect(tmpIndicator, SIGNAL(rBracketSet(int)), this, SLOT(updateRBracket(int)));
        if (i==0) {
            tmpIndicator->setLBracket(true);
        }
        if (i==(m_nsteps-1)) tmpIndicator->setRBracket(true);
        bottomLayout->addWidget(tmpIndicator);

    }

    layout->addLayout(topLayout);
    layout->addLayout(bottomLayout);
    setLayout(layout);

    updatePlayhead(0);

    // sequoia data

    sq_sequence_init(&m_seq, 16, 256);
    sq_sequence_set_outport(&m_seq, &OUTPORT);
    sq_sequence_set_notifications(&m_seq, true);
    sq_session_add_sequence(&SESSION, &m_seq);

    // notifications
    notiThread = new NotificationThread(this, &m_seq);
    connect(notiThread, SIGNAL(playheadUpdated(int)), this, SLOT(updatePlayhead(int)));
    notiThread->start();

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

void Editor::updateTrig(int step, sq_trigger_t *trig) {

    sq_sequence_set_trig(&m_seq, step, trig);

}
