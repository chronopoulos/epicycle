#include <QDebug>

#include "Manager.h"

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

    setMinimumWidth(1000);

    setAcceptDrops(true);

}

void Manager::addEditor(Editor *ed) {

    if (nullState) {

        layout->removeWidget(emptySetIcon);
        emptySetIcon->setVisible(false);
        layout->setAlignment(Qt::AlignTop);
        nullState = false;

    }

    layout->addWidget(ed);
    editors.push_back(ed);

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

        //thumb = new Thumbnail(); // test
        addEditor(new Editor());

    }

}

void Manager::clean(void) {

    // clean all editors
    std::vector<Editor*>::iterator iter;
    for (iter = editors.begin(); iter != editors.end(); iter++) {
        (*iter)->clean();
    }

}
