#include "Button.h"

#include <QDebug>
#include <QPalette>
#include <QApplication>

int Button::Edit_NoteValue = 0;
int Button::Edit_NoteVelocity = 1;

int SCOPE = 2; // editor scope

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
        }

        painter.drawText(QRect(0.3*w,0.5*h,0.4*w,0.2*h), Qt::AlignCenter, editText);

    } else {

        painter.setBrush(Qt::black);
        painter.drawRect(0.3*w, 0.1*h, 0.4*w, 0.2*h);

    }

    QPalette pal = palette();
    if (m_phocus && (SCOPE == 2)) {
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

void Indicator::setLBracket(bool hasLBracket) {

    if (hasLBracket == m_hasLBracket) return;

    m_hasLBracket = hasLBracket;
    update();

    if (m_hasLBracket) emit lBracketSet(m_step);

}

void Indicator::setRBracket(bool hasRBracket) {

    if (hasRBracket == m_hasRBracket) return;

    m_hasRBracket = hasRBracket;
    update();

    if (m_hasRBracket) emit rBracketSet(m_step);

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
            setLBracket(true);
        }
    } else if (e->buttons() == Qt::RightButton) {
        setRBracket(true);
    }

}

