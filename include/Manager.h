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

class Manager : public QFrame
{
    Q_OBJECT

    public:
        Manager(void);

    private:
        QVBoxLayout *layout;
        QLabel *emptySetIcon;
        bool nullState;
        void setNullState(void);

    protected:
        void contextMenuEvent(QContextMenuEvent*);

};

#endif
