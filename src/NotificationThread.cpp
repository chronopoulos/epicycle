#include <QDebug>

#include <unistd.h>

#include "NotificationThread.h"

NotificationThread::NotificationThread(QObject *parent, sq_sequence_t *seq) :
QThread(parent) {

    m_seq = seq;

    running = true; // TODO move this into a "start" method

}

void NotificationThread::run(void) {

    int vi;
    bool vb;

    while (1) {

        mtx_running.lock();
        if (!running) {
            mtx_running.unlock();
            break;
        }
        mtx_running.unlock();

        if (sq_sequence_read_new_playhead(m_seq, &vi)) {
            emit playheadUpdated(vi);
        }

        if (sq_sequence_read_new_first(m_seq, &vi)) {
            emit firstUpdated(vi);
        }

        if (sq_sequence_read_new_last(m_seq, &vi)) {
            emit lastUpdated(vi);
        }

        if (sq_sequence_read_new_transpose(m_seq, &vi)) {
            emit transposeUpdated(vi);
        }

        if (sq_sequence_read_new_clockdivide(m_seq, &vi)) {
            emit clockDivideUpdated(vi);
        }

        if (sq_sequence_read_new_mute(m_seq, &vb)) {
            vb ? emit muteUpdated("True") : emit muteUpdated("False");
        }

        usleep(10000);

    }

}

void NotificationThread::requestStop(void) {

    mtx_running.lock();
    running = false;
    mtx_running.unlock();

}
