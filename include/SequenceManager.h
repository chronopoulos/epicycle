#ifndef Manager_H
#define Manager_H

#include <QFrame>
#include <QVBoxLayout>
#include <QMenu>
#include <QAction>
#include <QCursor>
#include <QLabel>
#include <QDragEnterEvent>
#include <QMimeData>

#include "SequenceEditor.h"

class SequenceManager : public QFrame
{
    Q_OBJECT

    public:
        SequenceManager(void);
        void addSequenceEditor(SequenceEditor*);
        void clean(void);
        void advancePhocus(int);
        void phocusEvent(QKeyEvent*);
        void removeAllSequenceEditors(void);

    private:
        QVBoxLayout *layout;
        QLabel *emptySetIcon;
        bool nullState;
        void setNullState(void);
        std::vector<SequenceEditor*> sequenceEditors;
        int phocusIndex, buttonPhocus;

    protected:
        void contextMenuEvent(QContextMenuEvent*);

};

#endif
