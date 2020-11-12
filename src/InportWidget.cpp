#include <QVBoxLayout>
#include <QPalette>
#include <QDebug>
#include <QInputDialog>

#include "InportWidget.h"
#include "Helper.h"
#include "Delta.h"

extern sq_session_t SESSION;
extern Delta DELTA;

InportWidget::InportWidget(sq_inport_t inport) : QFrame() {

    m_inport = inport;
    m_name = QString(sq_inport_get_name(inport));

    setFrameStyle(QFrame::Box | QFrame::Plain);
    setLineWidth(2);

    QVBoxLayout *layout = new QVBoxLayout();
    setLayout(layout);

    nameLabel = new QLabel(m_name);

    layout->addWidget(nameLabel);

    m_type = sq_inport_get_type(m_inport);

    resize(80, 80);

    buildMenu();

}

void InportWidget::buildMenu(void) {

    rcmenu = new QMenu();

    nameAction = rcmenu->addAction("set name");

    QActionGroup *ag = new QActionGroup(this);
    QAction *tmpAction;
    //
    modMenu = rcmenu->addMenu("set modulation");
    tmpAction = modMenu->addAction("None");
    tmpAction->setCheckable(true);
    tmpAction->setChecked(true);
    connect(tmpAction, SIGNAL(toggled(bool)), this, SLOT(checkIt(bool)));
    ag->addAction(tmpAction);
    tmpAction = modMenu->addAction("Mute");
    tmpAction->setCheckable(true);
    connect(tmpAction, SIGNAL(toggled(bool)), this, SLOT(checkIt(bool)));
    ag->addAction(tmpAction);
    tmpAction = modMenu->addAction("Transpose");
    tmpAction->setCheckable(true);
    connect(tmpAction, SIGNAL(toggled(bool)), this, SLOT(checkIt(bool)));
    ag->addAction(tmpAction);
    tmpAction = modMenu->addAction("Playhead");
    tmpAction->setCheckable(true);
    connect(tmpAction, SIGNAL(toggled(bool)), this, SLOT(checkIt(bool)));
    ag->addAction(tmpAction);
    tmpAction = modMenu->addAction("Clock Divide");
    tmpAction->setCheckable(true);
    connect(tmpAction, SIGNAL(toggled(bool)), this, SLOT(checkIt(bool)));
    ag->addAction(tmpAction);
    tmpAction = modMenu->addAction("First");
    tmpAction->setCheckable(true);
    connect(tmpAction, SIGNAL(toggled(bool)), this, SLOT(checkIt(bool)));
    ag->addAction(tmpAction);
    tmpAction = modMenu->addAction("Last");
    tmpAction->setCheckable(true);
    connect(tmpAction, SIGNAL(toggled(bool)), this, SLOT(checkIt(bool)));
    ag->addAction(tmpAction);

    seqMenu = rcmenu->addMenu("set sequences");

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
        case INPORT_FIRST:
            pal.setColor(QPalette::Background, Qt::darkYellow);
            break;
        case INPORT_LAST:
            pal.setColor(QPalette::Background, Qt::darkCyan);
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

    sq_inport_set_type(m_inport, type);
    DELTA.setState(true);
    m_type = type;
    update();

}

void InportWidget::mousePressEvent(QMouseEvent *e) {

    if (e->buttons() == Qt::RightButton) {

        std::vector<QAction*> seqActions;

        sq_sequence_t seq;
        seqMenu->clear();
        size_t i;
        for (i=0; i<sq_session_get_nseqs(SESSION); i++) {
            seq = sq_session_get_seq(SESSION, i);
            seqActions.push_back(seqMenu->addAction(sq_sequence_get_name(seq)));
        }

        QAction *selectedAction;

        selectedAction = rcmenu->exec(QCursor::pos());
        if (selectedAction == nameAction) {
            launchNameDialog();
        } else {
            for (i=0; i<sq_session_get_nseqs(SESSION); i++) {
                if (seqActions[i] == selectedAction) {
                    sq_inport_add_sequence(m_inport, sq_session_get_seq(SESSION, i));
                    DELTA.setState(true);
                    break;
                }
            }
        }

    }

}

void InportWidget::launchNameDialog(void) {

    QString name;
    bool ok;

    ok = false;
    name = QInputDialog::getText(this, "inport", "Set Name:", QLineEdit::Normal,
                                            m_name, &ok);
    if (ok) {
        sq_inport_set_name(m_inport, name.toStdString().c_str());
        DELTA.setState(true);
        nameLabel->setText(name);
        m_name = name;
    }

}

void InportWidget::checkIt(bool checked) {

    QAction *action;

    if (checked) {

        action = qobject_cast<QAction*>(sender());

        if (action->text() == "None") {
            setType(INPORT_NONE);
        } else if (action->text() == "Mute") {
            setType(INPORT_MUTE);
        } else if (action->text() == "Transpose") {
            setType(INPORT_TRANSPOSE);
        } else if (action->text() == "Playhead") {
            setType(INPORT_PLAYHEAD);
        } else if (action->text() == "Clock Divide") {
            setType(INPORT_CLOCKDIVIDE);
        } else if (action->text() == "First") {
            setType(INPORT_FIRST);
        } else if (action->text() == "Last") {
            setType(INPORT_LAST);
        }

    }

}
