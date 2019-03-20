#include <QVBoxLayout>
#include <QLabel>
#include <QPalette>
#include <QDebug>
#include <QMenu>

#include "InportWidget.h"
#include "Helper.h"

extern sq_session_t SESSION;

InportWidget::InportWidget() : QFrame() {

    setFrameStyle(QFrame::Box | QFrame::Plain);
    setLineWidth(2);

    QVBoxLayout *layout = new QVBoxLayout();
    setLayout(layout);

    // sequoia data
    QString name = getRandomString(4);
    sq_inport_init(&m_inport,name.toStdString().c_str());
    sq_session_register_inport(&SESSION, &m_inport);

    m_type = m_inport.type;

    layout->addWidget(new QLabel(name));
    resize(80, 80);

}

void InportWidget::paintEvent(QPaintEvent *e) {

    QPalette pal = palette();
    switch (m_type) {
        case INPORT_NONE:
            pal.setColor(QPalette::Background, Qt::gray);
            break;
        case INPORT_TRANSPOSE:
            pal.setColor(QPalette::Background, Qt::cyan);
            break;
        case INPORT_PLAYHEAD:
            pal.setColor(QPalette::Background, Qt::darkGreen);
            break;
        case INPORT_CLOCKDIVIDE:
            pal.setColor(QPalette::Background, Qt::magenta);
            break;
        case INPORT_MUTE:
            pal.setColor(QPalette::Background, Qt::yellow);
            break;
        default:
            break;
    }
    setAutoFillBackground(true);
    setPalette(pal);

    QFrame::paintEvent(e);

}

void InportWidget::setType(enum inport_type type) {

    sq_inport_set_type(&m_inport, type);
    m_type = type;
    update();

}

void InportWidget::mousePressEvent(QMouseEvent *e) {

    QMenu menu;
    std::vector<QAction*> actions;

    if (e->buttons() == Qt::LeftButton) {

        // set the inport type

        actions.push_back(menu.addAction("None"));
        actions.push_back(menu.addAction("Transpose"));
        actions.push_back(menu.addAction("Playhead"));
        actions.push_back(menu.addAction("Clock Divide"));
        actions.push_back(menu.addAction("Mute"));

        QAction *selectedAction = menu.exec(QCursor::pos());

        if (selectedAction) {
            if (selectedAction->text() == "None") setType(INPORT_NONE);
            if (selectedAction->text() == "Transpose") setType(INPORT_TRANSPOSE);
            if (selectedAction->text() == "Playhead") setType(INPORT_PLAYHEAD);
            if (selectedAction->text() == "Clock Divide") setType(INPORT_CLOCKDIVIDE);
            if (selectedAction->text() == "Mute") setType(INPORT_MUTE);
        }

    } else if (e->buttons() == Qt::RightButton) {

        // select a sequence to add
    
        sq_sequence_t *seq;
        for (int i=0; i<SESSION.nseqs; i++) {
            seq = SESSION.seqs[i];
            actions.push_back(menu.addAction(seq->name));
        }
        QAction *selectedAction = menu.exec(QCursor::pos());
        if (selectedAction) {
            for (int i=0; i<SESSION.nseqs; i++) {
                if (actions[i] == selectedAction) {
                    _inport_add_sequence_now(&m_inport, SESSION.seqs[i]);
                    break;
                }
            }
        }

    }

}
