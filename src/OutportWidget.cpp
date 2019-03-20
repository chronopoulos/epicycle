#include <QVBoxLayout>
#include <QLabel>
#include <QPalette>

#include "OutportWidget.h"
#include "Helper.h"

extern sq_session_t SESSION;

OutportWidget::OutportWidget() : QFrame() {

    setFrameStyle(QFrame::Box | QFrame::Plain);
    setLineWidth(2);

    QVBoxLayout *layout = new QVBoxLayout();
    setLayout(layout);

    // sequoia data

    QString name = getRandomString(4);
    sq_outport_init(&m_outport, name.toStdString().c_str());
    sq_session_register_outport(&SESSION, &m_outport);

    layout->addWidget(new QLabel(name));

    resize(80, 80);

}

void OutportWidget::paintEvent(QPaintEvent *e) {

    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(200, 0, 0));
    setAutoFillBackground(true);
    setPalette(pal);

    QFrame::paintEvent(e);

}
