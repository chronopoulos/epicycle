#ifndef NOTIFICATIONTHREAD_H
#define NOTIFICATIONTHREAD_H

#include <QThread>
#include <QMutex>

#include "sequoia.h"

class NotificationThread : public QThread
{
    Q_OBJECT

    public:
        NotificationThread(QObject*, sq_sequence_t*);
        void requestStop(void);

    private:
        sq_sequence_t *m_seq;
        bool running;
        QMutex mtx_running;

    protected:
        void run(void);

    signals:
        void playheadUpdated(int);

};

#endif
