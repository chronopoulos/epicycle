#include <QLabel>
#include <QDebug>

#include "InportManager.h"

InportManager::InportManager(void) : QFrame() {

    layout = new QVBoxLayout();
    layout->setAlignment(Qt::AlignTop);

    setLayout(layout);

    setFrameStyle(QFrame::Box | QFrame::Plain);
    setLineWidth(1);

    setMinimumWidth(100);

}

void InportManager::addInport(InportWidget *inport) {

    layout->addWidget(inport);
    inports.push_back(inport);

}

void InportManager::removeAllInports(void) {

    std::vector<InportWidget*>::iterator iter;
    for (iter = inports.begin(); iter != inports.end(); iter++) {
        layout->removeWidget(*iter);
        (*iter)->setVisible(false);
        (*iter)->deleteLater();
    }

    inports.clear();

}

