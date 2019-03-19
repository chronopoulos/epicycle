#ifndef EDITOR_H
#define EDITOR_H

#include <QFrame>
#include <QHBoxLayout>

#include "Button.h"

class Editor : public QFrame
{
    Q_OBJECT

    public:
        Editor(void);

    private:
        std::vector<Button*> buttons;
        std::vector<Indicator*> indicators;
        int m_nsteps;

        Indicator *playheadIndicator;
        Indicator *lBracketIndicator;
        Indicator *rBracketIndicator;

    public slots:
        void updatePlayhead(int);
        void updateLBracket(int);
        void updateRBracket(int);


};

#endif
