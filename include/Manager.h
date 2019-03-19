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

class Manager : public QFrame
{
    Q_OBJECT

    public:
        Manager(void);
        void addEditor(Editor*);
        void clean(void);

    private:
        QVBoxLayout *layout;
        QLabel *emptySetIcon;
        bool nullState;
        void setNullState(void);
        std::vector<Editor*> editors;

    protected:
        void contextMenuEvent(QContextMenuEvent*);

};

#endif
