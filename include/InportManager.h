#ifndef INPORTMANAGER_H
#define INPORTMANAGER_H

#include <QFrame>
#include <QVBoxLayout>

#include "InportWidget.h"

class InportManager : public QFrame
{
    Q_OBJECT

    public:
        InportManager(void);
        void addInport(InportWidget*);
        void removeAllInports(void);

    private:
        QVBoxLayout *layout;
        std::vector<InportWidget*> inports;

};

#endif
