#ifndef INPORTWIDGET_H
#define INPORTWIDGET_H

#include <QFrame>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QLabel>
#include <QMenu>

#include "sequoia.h"

#include "InportWidget.h"

class InportWidget : public QFrame
{
    Q_OBJECT

    public:
        InportWidget(sq_inport_t*);
        void setType(enum inport_type);

    private:
        sq_inport_t *m_inport;
        enum inport_type m_type;
        QString m_name;
        QLabel *nameLabel;
        void launchNameDialog(void);
        void buildMenu(void);

        QMenu *rcmenu, *modMenu, *seqMenu;
        QAction *nameAction, *seqAction;

    protected:
        void paintEvent(QPaintEvent*);
        void mousePressEvent(QMouseEvent*);

    public slots:
        void checkIt(bool);

};

#endif
