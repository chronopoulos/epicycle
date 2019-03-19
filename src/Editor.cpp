#include "Editor.h"

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

