#include <QVBoxLayout>
#include <QPalette>
#include <QDebug>
#include <QInputDialog>

#include "InportWidget.h"
#include "Helper.h"

extern sq_session_t SESSION;

InportWidget::InportWidget() : QFrame() {

    setFrameStyle(QFrame::Box | QFrame::Plain);
    setLineWidth(2);

    QVBoxLayout *layout = new QVBoxLayout();
    setLayout(layout);

    // sequoia data
    m_name = getRandomString(4);
    sq_inport_init(&m_inport,m_name.toStdString().c_str());
    sq_session_register_inport(&SESSION, &m_inport);

    nameLabel = new QLabel(m_name);

    layout->addWidget(nameLabel);

    m_type = m_inport.type;

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
    tmpAction = modMenu->addAction("Direction");
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

    seqAction = rcmenu->addAction("set sequences");

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

    std::vector<QAction*> actions;

    if (e->buttons() == Qt::LeftButton) {

        // set the inport type
        /*
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
        */

    } else if (e->buttons() == Qt::RightButton) {

        // select a sequence to add
        /*
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
        */

        QAction *selectedAction;

        selectedAction = rcmenu->exec(QCursor::pos());
        if (selectedAction == nameAction) {
            launchNameDialog();
        } else if (selectedAction == seqAction) {
            qDebug() << "seq";
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
        sq_inport_set_name(&m_inport, name.toStdString().c_str());
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
        } else if (action->text() == "Direction") {
            setType(INPORT_DIRECTION);
        } else if (action->text() == "First") {
            setType(INPORT_FIRST);
        } else if (action->text() == "Last") {
            setType(INPORT_LAST);
        }

    }

}
