#include <QPalette>
#include <QVBoxLayout>
#include <QMenu>
#include <QDebug>
#include <QInputDialog>

#include "OutportWidget.h"
#include "Helper.h"
#include "Delta.h"

extern sq_session_t SESSION;
extern Delta DELTA;

OutportWidget::OutportWidget(sq_outport_t outport) : QFrame() {

    m_outport = outport;
    m_name = QString(sq_outport_get_name(outport));

    setFrameStyle(QFrame::Box | QFrame::Plain);
    setLineWidth(2);

    QVBoxLayout *layout = new QVBoxLayout();
    setLayout(layout);

    nameLabel = new QLabel(m_name);
    nameLabel->setStyleSheet("QLabel {color : black; font : bold;}");
    nameLabel->setAlignment(Qt::AlignCenter);

    layout->addWidget(nameLabel);

    resize(80, 80);

}

void OutportWidget::paintEvent(QPaintEvent *e) {

    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(0x02, 0xf4, 0x5f));  // aqua
    setAutoFillBackground(true);
    setPalette(pal);

    QFrame::paintEvent(e);

}

void OutportWidget::mousePressEvent(QMouseEvent *e) {

    QMenu menu;
    QAction *nameAction;
    QString name;
    bool ok;

    if (e->buttons() == Qt::RightButton) {

        nameAction = menu.addAction("Set Name");
        if(menu.exec(QCursor::pos()) == nameAction) {
            ok = false;
            name = QInputDialog::getText(this, "outport", "Set Name:", QLineEdit::Normal,
                                                    m_name, &ok);
            if (ok) {
                sq_outport_set_name(m_outport, name.toStdString().c_str());
                DELTA.setState(true);
                nameLabel->setText(name);
                m_name = name;
            }
        }

    }

}
