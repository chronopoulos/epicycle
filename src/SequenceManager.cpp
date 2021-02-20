#include <QDebug>
#include <QApplication>

#include "SequenceManager.h"
#include "Helper.h"

SequenceManager::SequenceManager(void) : QFrame() {

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

    phocusIndex = -1;

    buttonPhocus = 0;

}

void SequenceManager::addSequenceEditor(SequenceEditor *ed) {

    if (nullState) {

        layout->removeWidget(emptySetIcon);
        emptySetIcon->setVisible(false);
        layout->setAlignment(Qt::AlignTop);
        nullState = false;

    }

    layout->addWidget(ed);
    sequenceEditors.push_back(ed);

}

void SequenceManager::setNullState(void) {

    layout->setAlignment(Qt::AlignCenter);
    layout->addWidget(emptySetIcon);
    emptySetIcon->setVisible(true);
    nullState = true;

}

void SequenceManager::contextMenuEvent(QContextMenuEvent*) {

    QMenu menu;

    QAction *addSeqAction = menu.addAction("Add Sequence");

    QAction *action = menu.exec(QCursor::pos());
    if (action == addSeqAction) {

        addSequenceEditor(new SequenceEditor(newDefaultSequence()));

    }

}

void SequenceManager::clean(void) {

    // clean all SequenceEditors
    std::vector<SequenceEditor*>::iterator iter;
    for (iter = sequenceEditors.begin(); iter != sequenceEditors.end(); iter++) {
        (*iter)->clean();
    }

}

void SequenceManager::phocusEvent(QKeyEvent *e) {

    Qt::KeyboardModifiers mod = QApplication::keyboardModifiers();

    if (!(mod & Qt::ControlModifier)) {
        if (e->key() == Qt::Key_J) {
            advancePhocus(1);
        } else if (e->key() == Qt::Key_K) {
            advancePhocus(-1);
        }
    }
    
    if (phocusIndex >= 0) {
        sequenceEditors[phocusIndex]->phocusEvent(e);
    }

}

void SequenceManager::advancePhocus(int increment) {

    int nseqs = (int) sequenceEditors.size();

    if (nseqs > 0) {

        if (phocusIndex >= 0) {
            buttonPhocus = sequenceEditors[phocusIndex]->phocusIndex;
            sequenceEditors[phocusIndex]->setPhocus(false);
            phocusIndex = phocusIndex + increment;
            if (phocusIndex >= nseqs) phocusIndex = nseqs - 1;
            if (phocusIndex < 0) {
                phocusIndex = 0;
            }
        } else {
            phocusIndex = 0;
        }

        sequenceEditors[phocusIndex]->phocusIndex = buttonPhocus; // bad variable names..
        sequenceEditors[phocusIndex]->setPhocus(true);

    }

}

void SequenceManager::removeAllSequenceEditors(void) {

    std::vector<SequenceEditor*>::iterator iter;
    for (iter = sequenceEditors.begin(); iter != sequenceEditors.end(); iter++) {
        layout->removeWidget(*iter);
        (*iter)->setVisible(false);
        (*iter)->deleteLater();
    }

    sequenceEditors.clear();

    setNullState();
    phocusIndex = -1;
    buttonPhocus = 0;

}

