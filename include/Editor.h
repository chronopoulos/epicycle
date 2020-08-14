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
        Editor(sq_sequence_t*);
        void clean(void);
        void setPhocus(bool);
        void phocusEvent(QKeyEvent*);
        void advancePhocus(int);
        int phocusIndex;

    private:
        std::vector<Button*> buttons;
        std::vector<Indicator*> indicators;
        int m_nsteps;

        Indicator *playheadIndicator;
        Indicator *firstIndicator;
        Indicator *lastIndicator;

        sq_sequence_t *m_seq;

        NotificationThread *notiThread;

        ClickLabel *nameLabel;
        ClickLabel *transposeLabel, *clockDivLabel, *muteLabel;

        bool m_phocus;

        int m_editParameter;

        void cycleEditParameter(void);
        void setEditParameter(int);

    public slots:
        void updatePlayhead(int);
        void updateFirst(int);
        void updateLast(int);
        void setName(QString);
        void setTrig(int, sq_trigger_t*);
        void setTranspose(int);
        void setPlayhead(int);
        void setClockDivide(int);
        void setMute(QString);
        void setFirst(int);
        void setLast(int);

    protected:
        void contextMenuEvent(QContextMenuEvent*);
        void paintEvent(QPaintEvent*);

};

#endif
