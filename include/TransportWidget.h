#include <QFrame>
#include <QGridLayout>
#include <QPushButton>
#include <QButtonGroup>
#include <QIcon>
#include <QDoubleSpinBox>

#include "ClickLabel.h"

class TransportWidget : public QFrame
{
    Q_OBJECT

    public:
        TransportWidget(void);
        int state;
        static int Stopped, Playing, Paused;
        QPushButton *stopButton, *pauseButton, *playButton;

    private:
        QGridLayout *layout;
        ClickLabel *tempoLabel;
        QButtonGroup *buttonGroup;

    public slots:
        void setTempo(double);
        void stop(void);
        void pause(void);
        void play(void);

    signals:
        void ticked(void);
        void stopped(void);

};
