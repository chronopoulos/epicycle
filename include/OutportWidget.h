#ifndef OUTPORTWIDGET_H
#define OUTPORTWIDGET_H

#include <QFrame>
#include <QLabel>
#include <QPaintEvent>

#include "sequoia.h"

#include "OutportWidget.h"

class OutportWidget : public QFrame
{
    Q_OBJECT

    public:
        OutportWidget(void);

    private:
        sq_outport_t *m_outport;
        QString m_name;
        QLabel *nameLabel;

    protected:
        void paintEvent(QPaintEvent*);
        void mousePressEvent(QMouseEvent*);

};

#endif
