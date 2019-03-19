#include "Manager.h"

#include <QDebug>

Manager::Manager(void) : QFrame() {

    layout = new QVBoxLayout();
    setLayout(layout);

    setFrameStyle(QFrame::Box | QFrame::Plain);
    setLineWidth(1);

    emptySetIcon = new QLabel();
    QPixmap pm(":/img/empty_set.png");
    emptySetIcon->setPixmap(pm.scaledToWidth(100, Qt::SmoothTransformation));
    emptySetIcon->setAlignment(Qt::AlignHCenter);

    setNullState();

    setMinimumWidth(340);

    setAcceptDrops(true);

}

void Manager::setNullState(void) {

    layout->setAlignment(Qt::AlignCenter);
    layout->addWidget(emptySetIcon);
    emptySetIcon->setVisible(true);
    nullState = true;

}

void Manager::contextMenuEvent(QContextMenuEvent*) {

    QMenu menu;

    QAction *addSeqAction = menu.addAction("Add Sequence");

    QAction *action = menu.exec(QCursor::pos());
    if (action == addSeqAction) {

    }

}

