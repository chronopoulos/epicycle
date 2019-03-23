#include "Button.h"

#include <QDebug>
#include <QPalette>
#include <QApplication>

int Button::Edit_NoteValue = 0;
int Button::Edit_NoteVelocity = 1;
int Button::Edit_NoteLength = 2;
int Button::Edit_Microtime = 3;

Button::Button(int step) {

    m_step = step;

    setMinimumSize(50,50);
    setMaximumSize(50,50);

    m_isActive = false;

    m_editParameter = Button::Edit_NoteValue;

    setFrameStyle(QFrame::Box | QFrame::Plain);

    setPhocus(false);

    sq_trigger_init(&m_trig);

    setContextMenuPolicy(Qt::PreventContextMenu);

}

void Button::setPhocus(bool phocus) {

    m_phocus = phocus;
    update();

}

void Button::phocusEvent(QKeyEvent *e) {

    int increment;
    Qt::KeyboardModifiers mod = QApplication::keyboardModifiers();

    if (mod & Qt::ControlModifier) {

        if (e->key() == Qt::Key_J) {

            if (mod & Qt::ShiftModifier) {
                increment = -4;
            } else {
                increment = -1;
            }

            adjustEditParameter(increment);

        } else if (e->key() == Qt::Key_K) {

            if (mod & Qt::ShiftModifier) {
                increment = 4;
            } else {
                increment = 1;
            }

            adjustEditParameter(increment);

        }

    }
        
    if ((e->key() == Qt::Key_T) & !(e->modifiers() & Qt::ShiftModifier)) {

            toggle();

    }

}

void Button::mousePressEvent(QMouseEvent *e) {

    if (e->buttons() == Qt::LeftButton) toggle();

}

void Button::toggle(void) {

    m_isActive = !m_isActive;

    if (m_isActive) {
        sq_trigger_set_note(&m_trig, m_trig.note, m_trig.velocity, m_trig.length);
    } else {
        sq_trigger_set_null(&m_trig);
    }

    emit trigUpdated(m_step, &m_trig);
    update();

}

void Button::wheelEvent(QWheelEvent *e) {

    int wheelIncrement, wheelSign;

    if (QApplication::keyboardModifiers() == Qt::ShiftModifier) {
        wheelIncrement = 4;
    } else {
        wheelIncrement = 1;
    }

    if (e->angleDelta().y() > 0) {
        wheelSign = 1;
    } else if (e->angleDelta().y() < 0) {
        wheelSign = -1;
    } else {
        wheelSign = 0; // this should never happen
    }

    adjustEditParameter(wheelSign * wheelIncrement);

}

void Button::adjustEditParameter(int increment) {

    int noteValue, noteVelocity;
    float noteLength, microtime;

    if (m_isActive) {

        if (m_editParameter == Button::Edit_NoteValue) {

            noteValue = m_trig.note;

            noteValue += increment;
            if (noteValue > 127) noteValue = 127;
            if (noteValue < 0) noteValue = 0;

            m_trig.note = noteValue;

        } else if (m_editParameter == Button::Edit_NoteVelocity) {

            noteVelocity = m_trig.velocity;

            noteVelocity += increment;
            if (noteVelocity > 127) noteVelocity = 127;
            if (noteVelocity < 0) noteVelocity = 0;

            m_trig.velocity = noteVelocity;

        } else if (m_editParameter == Button::Edit_NoteLength) {

            noteLength = m_trig.length;

            noteLength += (float) increment / 100.;
            if (noteLength > 15.9) noteLength = 15.9;
            if (noteLength < 0.1) noteLength = 0.1;

            m_trig.length = noteLength;

        } else if (m_editParameter == Button::Edit_Microtime) {

            microtime = m_trig.microtime;

            microtime += (float) increment / 100.;
            if (microtime > 0.45) microtime = 0.45;
            if (microtime < -0.5) microtime = -0.5;

            m_trig.microtime = microtime;

        }

        emit trigUpdated(m_step, &m_trig);
        update();

    }

}

void Button::setEditParameter(int index) {

    m_editParameter = index;

    update();

}

void Button::paintEvent(QPaintEvent *e) {

    QString editText;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::black);

    qreal w,h;
    w = width();
    h = height();

    if (m_isActive) {

        painter.setBrush(Qt::red);
        painter.drawRect(0.3*w, 0.1*h, 0.4*w, 0.2*h);

        if (m_editParameter == Button::Edit_NoteValue) {
            editText = QString::number(m_trig.note);
        } else if (m_editParameter == Button::Edit_NoteVelocity) {
            editText = QString::number(m_trig.velocity);
        } else if (m_editParameter == Button::Edit_NoteLength) {
            editText = QString::number(m_trig.length, 'f', 2);
        } else if (m_editParameter == Button::Edit_Microtime) {
            editText = QString::number(m_trig.microtime, 'f', 2);
        }

        painter.drawText(QRect(0.1*w,0.5*h,0.8*w,0.2*h), Qt::AlignCenter, editText);

    } else {

        painter.setBrush(Qt::black);
        painter.drawRect(0.3*w, 0.1*h, 0.4*w, 0.2*h);

    }

    QPalette pal = palette();
    if (m_phocus) {
        setLineWidth(3);
        pal.setColor(QPalette::Background, Qt::gray);
    } else {
        setLineWidth(1);
        pal.setColor(QPalette::Background, QColor(100, 100, 100));
    }
    setAutoFillBackground(true);
    setPalette(pal);

    QFrame::paintEvent(e);

}


Indicator::Indicator(int step) {

    m_step = step;

    m_hasPlayhead = false;
    m_hasLBracket= false;
    m_hasRBracket= false;

    setMinimumHeight(30);

    setMinimumWidth(50);
    setMaximumWidth(50);

    setContextMenuPolicy(Qt::PreventContextMenu);

}

void Indicator::setPlayhead(bool hasPlayhead) {

    m_hasPlayhead = hasPlayhead;
    update();

}

void Indicator::setFirst(bool hasLBracket) {

    m_hasLBracket = hasLBracket;
    update();

}

void Indicator::setLast(bool hasRBracket) {

    m_hasRBracket = hasRBracket;
    update();

}

void Indicator::paintEvent(QPaintEvent*) {

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);

    qreal w,h;
    w = width();
    h = height();

    if (m_hasPlayhead) {
        painter.drawRect(0.1*w, 0.8*h, 0.8*w, 0.1*h);
    }

    if (m_hasLBracket) {
        QPolygonF triangle;
        triangle.append(QPointF(0, -5));
        triangle.append(QPointF(5, 0));
        triangle.append(QPointF(0, 5));
        triangle.append(QPointF(0,-5));
        triangle.translate(w/2 - 5, h/2);
        painter.drawPolygon(triangle);
    }

    if (m_hasRBracket) {
        QPolygonF triangle;
        triangle.append(QPointF(0, -5));
        triangle.append(QPointF(-5, 0));
        triangle.append(QPointF(0, 5));
        triangle.append(QPointF(0,-5));
        triangle.translate(w/2 + 5, h/2);
        painter.drawPolygon(triangle);
    }

}

void Indicator::mousePressEvent(QMouseEvent *e) {

    if (e->buttons() == Qt::LeftButton) {
        if (QApplication::keyboardModifiers() & Qt::ControlModifier) {
            emit playheadRequested(m_step);
        } else {
            emit firstRequested(m_step);
        }
    } else if (e->buttons() == Qt::RightButton) {
        emit lastRequested(m_step);
    }

}

