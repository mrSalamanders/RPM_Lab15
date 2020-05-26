#include <QApplication>
#include <QObject>
#include <csignal>
#include "server.h"
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow app;
    app.show();
    return a.exec();
}

