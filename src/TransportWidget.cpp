#include "TransportWidget.h"

#include <QDebug>

#define DEFAULT_TEMPO 100

int TransportWidget::Stopped = 0;
int TransportWidget::Paused = 1;
int TransportWidget::Playing = 2;

TransportWidget::TransportWidget(void) {

    // qobjects

    tempoLabel = new ClickLabel("Tempo: %1 BPM", ClickLabel::Type_Double, "Set Tempo", "Tempo");
    connect(tempoLabel, SIGNAL(valueChanged(double)), this, SLOT(setTempo(double)));

    stopButton = new QPushButton();
    stopButton->setIcon(QIcon(":/img/stop.png"));
    stopButton->setCheckable(true);
    stopButton->setFocusPolicy(Qt::NoFocus);
    connect(stopButton, SIGNAL(pressed(void)), this, SLOT(stop(void)));

    pauseButton = new QPushButton();
    pauseButton->setIcon(QIcon(":/img/pause.png"));
    pauseButton->setCheckable(true);
    pauseButton->setFocusPolicy(Qt::NoFocus);
    pauseButton->setDisabled(true);
    connect(pauseButton, SIGNAL(pressed(void)), this, SLOT(pause(void)));

    playButton = new QPushButton();
    playButton->setIcon(QIcon(":/img/play.png"));
    playButton->setCheckable(true);
    playButton->setFocusPolicy(Qt::NoFocus);
    connect(playButton, SIGNAL(pressed(void)), this, SLOT(play(void)));

    buttonGroup = new QButtonGroup();
    buttonGroup->addButton(stopButton);
    buttonGroup->addButton(pauseButton);
    buttonGroup->addButton(playButton);

    // ui

    layout = new QGridLayout();
    layout->addWidget(tempoLabel, 0,0, 1,3);
    layout->addWidget(stopButton, 1,0, 1,1);
    layout->addWidget(pauseButton, 1,1, 1,1);
    layout->addWidget(playButton, 1,2, 1,1);

    setLayout(layout);
    
    setFrameStyle(QFrame::Box | QFrame::Plain);
    setLineWidth(1);
    setMaximumHeight(100);

    // init

    tempoLabel->setValue(double(DEFAULT_TEMPO));
    stopButton->setChecked(true);
    state = TransportWidget::Stopped;

}

void TransportWidget::setTempo(double bpm) {

    emit tempoChanged(bpm);

}

void TransportWidget::stop(void) {

    state = TransportWidget::Stopped;
    emit stateChanged(state);

}

void TransportWidget::pause(void) {

    state = TransportWidget::Paused;
    emit stateChanged(state);

}

void TransportWidget::play(void) {

    state = TransportWidget::Playing;
    emit stateChanged(state);

}

void TransportWidget::toggle(void) {

    if ((state == TransportWidget::Stopped) || (state == TransportWidget::Paused)) {
        playButton->click();
    } else if (state == TransportWidget::Playing) {
        stopButton->click();
    }

}

