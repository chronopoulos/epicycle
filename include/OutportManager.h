#ifndef OUTPORTMANAGER_H
#define OUTPORTMANAGER_H

#include <QFrame>
#include <QVBoxLayout>

#include "OutportWidget.h"

class OutportManager : public QFrame
{
    Q_OBJECT

    public:
        OutportManager(void);
        void addOutport(OutportWidget*);
        void removeAllOutports(void);

    private:
        QVBoxLayout *layout;
        std::vector<OutportWidget*> outports;

};

#endif
