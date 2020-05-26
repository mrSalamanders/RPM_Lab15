#include "confdialog.h"
#include "ui_dialog.h"
#include <QMainWindow>
#include <QCloseEvent>

ConfDialog::ConfDialog(QWidget *parent) : QMainWindow(parent), ui(new Ui::ConfDialog) {
    ui->setupUi(this);
    this->setWindowFlag(Qt::Tool);
    connect(ui->pushButton, &QPushButton::clicked, [=] {emit setPort(ui->lineEdit->text().toInt());});
}

ConfDialog::~ConfDialog() {
    delete ui;
}

void ConfDialog::closeEvent(QCloseEvent *event) {
    this->hide();
    event->ignore();
}

void ConfDialog::setPortToDialog(Server *instance) {
    ui->lineEdit->setText(QString::number(instance->serverPort));
}
