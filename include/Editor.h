#ifndef EDITOR_H
#define EDITOR_H

#include <QFrame>
#include <QHBoxLayout>
#include <QContextMenuEvent>

#include "sequoia.h"

#include "Button.h"
#include "NotificationThread.h"
#include "ClickLabel.h"

class Editor : public QFrame
{
    Q_OBJECT

    public:
        Editor(void);
        void clean(void);

    private:
        std::vector<Button*> buttons;
        std::vector<Indicator*> indicators;
        int m_nsteps;

        Indicator *playheadIndicator;
        Indicator *lBracketIndicator;
        Indicator *rBracketIndicator;

        sq_sequence_t m_seq;

        NotificationThread *notiThread;

        ClickLabel *nameLabel;
        ClickLabel *transposeLabel, *clockDivLabel, *muteLabel;

    public slots:
        void updatePlayhead(int);
        void updateLBracket(int);
        void updateRBracket(int);
        void setTrig(int, sq_trigger_t*);
        void setTranspose(int);
        void setClockDivide(int);
        void setMute(QString);

    protected:
        void contextMenuEvent(QContextMenuEvent*);

};

#endif
