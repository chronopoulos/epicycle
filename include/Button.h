#ifndef BUTTON_H
#define BUTTON_H

#include <QFrame>
#include <QVBoxLayout>
#include <QPaintEvent>
#include <QPainter>

#include "sequoia.h"

class Button : public QFrame
{
    Q_OBJECT

    public:
        Button(int);
        void toggle(void);
        void setEditParameter(int);

        static int Edit_NoteValue;
        static int Edit_NoteVelocity;
        static int Edit_NoteLength;
        static int Edit_Microtime;

        void setPhocus(bool);
        void phocusEvent(QKeyEvent*);
        void adjustEditParameter(int);

    private:
        bool m_isActive;
        int m_step;
        int m_editParameter;

        bool m_phocus;

        sq_trigger_t m_trig;

    protected:
        void mousePressEvent(QMouseEvent*);
        void paintEvent(QPaintEvent*);
        void wheelEvent(QWheelEvent*);

    signals:
        void trigUpdated(int, sq_trigger_t*);

};

class Indicator : public QWidget
{
    Q_OBJECT

    public:
        Indicator(int);
        void setPlayhead(bool);
        void setLBracket(bool);
        void setRBracket(bool);

    private:
        int m_step;
        bool m_hasPlayhead;
        bool m_hasLBracket;
        bool m_hasRBracket;

    protected:
        void paintEvent(QPaintEvent*);
        void mousePressEvent(QMouseEvent*);

    signals:
        void lBracketSet(int);
        void rBracketSet(int);
        void playheadRequested(int);

};

#endif
