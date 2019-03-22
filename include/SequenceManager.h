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

#include "Editor.h"

class SequenceManager : public QFrame
{
    Q_OBJECT

    public:
        SequenceManager(void);
        void addEditor(Editor*);
        void clean(void);
        void advancePhocus(int);
        void phocusEvent(QKeyEvent*);

    private:
        QVBoxLayout *layout;
        QLabel *emptySetIcon;
        bool nullState;
        void setNullState(void);
        std::vector<Editor*> editors;
        int phocusIndex;

    protected:
        void contextMenuEvent(QContextMenuEvent*);

};

#endif
