#include "Dialogs.h"

NewSequenceDialog::NewSequenceDialog(void) {

    layout = new QGridLayout();

    nameLabel = new QLabel("Name");
    lengthLabel = new QLabel("Length");

    nameLineEdit = new QLineEdit();
    nameLineEdit->setText("My Sequence");
    nameLineEdit->selectAll();

    lengthSpin = new QSpinBox();
    lengthSpin->setMinimum(1);
    lengthSpin->setValue(16);

    buttons = new QDialogButtonBox();
    buttons->addButton(QDialogButtonBox::Ok);
    buttons->addButton(QDialogButtonBox::Cancel);
    connect(buttons, SIGNAL(accepted(void)), this, SLOT(accept(void)));
    connect(buttons, SIGNAL(rejected(void)), this, SLOT(reject(void)));

    layout->addWidget(nameLabel, 0,0, 1,1);
    layout->addWidget(nameLineEdit, 0,1, 1,1);
    layout->addWidget(lengthLabel, 1,0, 1,1);
    layout->addWidget(lengthSpin, 1,1, 1,1);
    layout->addWidget(buttons, 2,0, 1,2);
    setLayout(layout);

    setModal(true);
    setWindowTitle("Add Sequence");
    setWindowFlags(Qt::WindowStaysOnTopHint);

}

QString NewSequenceDialog::name(void) {

    return nameLineEdit->text();

}

int NewSequenceDialog::length(void) {

    return lengthSpin->value();

}


MaybeSaveDialog::MaybeSaveDialog(void) {

    layout = new QVBoxLayout();

    description = new QLabel("\nSave changes before closing?\n");
    description->setAlignment(Qt::AlignCenter);

    buttons = new QDialogButtonBox();
    buttons->addButton(QDialogButtonBox::Save);
    buttons->addButton(QDialogButtonBox::Cancel);
    connect(buttons, SIGNAL(accepted(void)), this, SLOT(accept(void)));
    connect(buttons, SIGNAL(rejected(void)), this, SLOT(reject(void)));

    // why isn't this built into QDialogButtonBox?
    discardButton = new QPushButton("Discard Changes");
    buttons->addButton(discardButton, QDialogButtonBox::DestructiveRole);
    connect(discardButton, SIGNAL(released(void)), this, SLOT(discard(void)));

    layout->addWidget(description);
    layout->addWidget(buttons);

    setLayout(layout);

    setModal(true);
    setWindowTitle("Save Session?");
    setWindowFlags(Qt::WindowStaysOnTopHint);

}

void MaybeSaveDialog::discard(void) {

    done(-1);

}

RandomizeDialog::RandomizeDialog(void) {

    layout = new QGridLayout();

    layout->addWidget(new QLabel("start"), 0,0);
    startSpin = new QSpinBox();
    startSpin->setMinimum(1);
    startSpin->setValue(60);
    layout->addWidget(startSpin, 0,1);

    layout->addWidget(new QLabel("range"), 1,0);
    rangeSpin = new QSpinBox();
    rangeSpin->setMinimum(1);
    rangeSpin->setValue(16);
    layout->addWidget(rangeSpin, 1,1);

    okButton = new QPushButton("OK");
    layout->addWidget(okButton, 2,0);
    cancelButton = new QPushButton("Cancel");
    layout->addWidget(cancelButton, 2,1);

    setLayout(layout);

    setModal(true);
    setWindowTitle("Randomize Parameters");
    setWindowFlags(Qt::WindowStaysOnTopHint);

    connect(okButton, SIGNAL(released(void)), this, SLOT(accept(void)));
    connect(cancelButton, SIGNAL(released(void)), this, SLOT(reject(void)));

}

int RandomizeDialog::getStart(void) {

    return startSpin->value();

}

int RandomizeDialog::getRange(void) {

    return rangeSpin->value();

}

