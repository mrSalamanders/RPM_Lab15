#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCloseEvent>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    server = new Server(this);
    this->setWindowTitle("Server");
    connect(server, SIGNAL(updateUsers(int)), this, SLOT(setUsers(int)));
    connect(server, &Server::updateState, ui->statusLine, &QLineEdit::setText);
    connect(server, &Server::showMessageOnServer, ui->textEdit, &QTextEdit::append);

    setup = new QSettings("conf.ini", QSettings::IniFormat);

    if (setup->value("serverport").toInt() > 1000 && setup->value("serverport").toInt() <= 65535) {
        qDebug() << "Port exists";
        qDebug() << setup->value("serverport").toInt();
        server->serverPort = setup->value("serverport").toInt();
        server->startup();
    } else {
        firstPortSetup();
    }

    configure = new ConfDialog(this);
    configure->hide();

    QIcon newIcon;
    newIcon.addFile(":/res/tray.png");
    tray.setIcon(newIcon);
    trayMenu = new QMenu("Server menu", this);
    trayMenu->addAction("Show server window", this, &MainWindow::showAdvanced, QKeySequence(Qt::CTRL + Qt::Key_1));
    trayMenu->addAction("Configure", this, &MainWindow::configureServer, QKeySequence(Qt::CTRL + Qt::Key_2));
    trayMenu->addAction("Shut down", this, &MainWindow::close, QKeySequence(Qt::CTRL + Qt::Key_3));
    tray.setContextMenu(trayMenu);
    tray.setToolTip("Server Icon");
    connect(&tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(handleTray(QSystemTrayIcon::ActivationReason)));
    connect(ui->toolButton, &QAbstractButton::clicked, this, &MainWindow::configureServer);
    connect(configure, SIGNAL(setPort(qint16)), this, SLOT(saveSettings(qint16)));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setUsers(int num) {
    QString number = QString::number(num);
    ui->usersLine->setText(number);
};

void MainWindow::setState(QString state) {
    ui->statusLine->setText(state);
};

void MainWindow::handleTray(QSystemTrayIcon::ActivationReason reason) {
    if (reason == 1) {
        trayMenu->exec();
    } else {

    }
};

void MainWindow::closeEvent(QCloseEvent *event) {
    if (!this->isHidden() && !tray.isVisible()) {
        QMessageBox box;
        box.addButton(tr("Shut down"), QMessageBox::ActionRole);
        box.addButton(tr("Put in menu bar"), QMessageBox::ActionRole);
        box.setText("Shut down or put in menu bar?");
        box.exec();
        if (box.clickedButton()->text() == "Put in menu bar") {
            event->ignore();
            tray.show();
            this->hide();
        } else {
            event->accept();
        }
    };
};

void MainWindow::showAdvanced() {
    this->show();
    tray.hide();
};

void MainWindow::configureServer() {
    configure->setPortToDialog(this->server);
    configure->show();
};

void MainWindow::firstPortSetup() {
    qint16 userPort;
    userPort = QInputDialog::getInt(this, "Enter server port:", "Port:", 0, 1000, 65535, 1);
    server->serverPort = userPort;
    saveSettings(userPort);
    server->startup();
};

void MainWindow::saveSettings(qint16 port) {
    qDebug() << "Port received:";
    qDebug() << port;
    setup->setValue("serverport", port);
    setup->sync();
};


