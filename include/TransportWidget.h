#include <QFrame>
#include <QGridLayout>
#include <QPushButton>
#include <QButtonGroup>
#include <QIcon>

#include "ClickLabel.h"

class TransportWidget : public QFrame
{
    Q_OBJECT

    public:
        TransportWidget(double);
        static int Stopped, Playing, Paused;

    private:
        int state;
        float tempo;
        QGridLayout *layout;
        ClickLabel *tempoLabel;
        QButtonGroup *buttonGroup;
        QPushButton *stopButton, *pauseButton, *playButton;

    public slots:
        void setTempo(double);
        void stop(void);
        void pause(void);
        void play(void);
        void toggle(void);

    signals:
        void stateChanged(int);
        void tempoChanged(double);

};
