#ifndef INPORTWIDGET_H
#define INPORTWIDGET_H

#include <QFrame>
#include <QPaintEvent>
#include <QMouseEvent>

#include "sequoia.h"

#include "InportWidget.h"

class InportWidget : public QFrame
{
    Q_OBJECT

    public:
        InportWidget(void);
        void setType(enum inport_type);

    private:
        sq_inport_t m_inport;
        enum inport_type m_type;

    protected:
        void paintEvent(QPaintEvent*);
        void mousePressEvent(QMouseEvent*);

};

#endif
