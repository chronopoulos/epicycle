#include <QLabel>
#include <QDebug>

#include "OutportManager.h"

OutportManager::OutportManager(void) : QFrame() {

    layout = new QVBoxLayout();
    layout->setAlignment(Qt::AlignTop);

    setLayout(layout);

    setFrameStyle(QFrame::Box | QFrame::Plain);
    setLineWidth(1);

    setMinimumWidth(100);

}

void OutportManager::addOutport(OutportWidget *outport) {

    layout->addWidget(outport);
    outports.push_back(outport);

}

void OutportManager::removeAllOutports(void) {

    std::vector<OutportWidget*>::iterator iter;
    for (iter = outports.begin(); iter != outports.end(); iter++) {
        layout->removeWidget(*iter);
        (*iter)->setVisible(false);
        (*iter)->deleteLater();
    }

    outports.clear();

}
