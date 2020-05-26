#include "server.h"
#include <QDebug>
#include <QJsonObject>
#include <QFile>
#include <QDir>
#include <list>

Server::Server(QObject *parent) : QObject(parent) {
    directory.setPath(directory.currentPath());
    filepath = directory.absoluteFilePath("archive.txt");
    dbpath = ":/res/db.db";
    tcpServer = new QWebSocketServer("WebServer", QWebSocketServer::NonSecureMode, this);
    connect(tcpServer, &QWebSocketServer::newConnection, this, &Server::connectUser);

}

void Server::startup() {
    if (tcpServer->listen(QHostAddress::Any, serverPort)) {
        qInfo() << "Server started";
        emit updateState("Working");
    } else {
        qWarning() << "Unable to start server";
        emit updateState("Failed");
    }

    QFile database(dbpath);
    if (database.exists()) {
        this->db.setDatabaseName("db.db");
        qDebug() << dbpath;
        if (db.open()) {
            qDebug() << "Database open!";
        } else {
            qDebug() << "Error occured.";
        }

        QSqlQuery getData;
        getData.exec("SELECT rowid, Data FROM Messages ORDER BY rowid DESC LIMIT 10");
        while (getData.next()) {
            someList.push_front(getData.value(1).toByteArray());
        }
        db.close();
    } else {
        qDebug() << "Database not found, creating new one.";
        this->db.setDatabaseName(dbpath);
        db.open();
        QSqlQuery createQuery;
        createQuery.exec("CREATE TABLE 'Messages' ('Data' TEXT NOT NULL)");
        qDebug() << "Table created";
        db.close();
    }
    emit updateUsers(users.size());
};

void Server::connectUser() {
    client = tcpServer->nextPendingConnection();
    for (auto &i: users) {
        QByteArray bA = SystemCall("We have a new user!");
        i->sendBinaryMessage(bA);
    }
    if (someList.size() > 0) {
        QByteArray bA = SystemCall("__________Last messages___________");
        client->sendBinaryMessage(bA);
        for (auto &i : someList) {
            client->sendBinaryMessage(i);
        }
        bA = SystemCall("_____________________________________");
        client->sendBinaryMessage(bA);
    }
    users.push_back(client);
    connect(client, SIGNAL(binaryMessageReceived(QByteArray)), this, SLOT(runClientAction(QByteArray)));
    connect(client, &QWebSocket::disconnected, this, &Server::userLeft);
    emit updateUsers(users.size());
}

void Server::runClientAction(QByteArray data) {

    QByteArray byteArr;
    QJsonDocument jsonDoc;

    byteArr = data;
    jsonDoc = jsonDoc.fromBinaryData(byteArr);

    qDebug() << "<" + jsonDoc.object()["username"].toString() + "> : " + jsonDoc.object()["message"].toString();

    if (someList.size() < 10) {
        someList.push_back(byteArr);
    } else if  (someList.size() > 10) {
        while (someList.size() != 9) {
            someList.pop_front();
        }
        someList.push_back(byteArr);
    } else if (someList.size() == 10) {
        someList.pop_front();
        someList.push_back(byteArr);
    }

    for (auto &i : users) {
        i->sendBinaryMessage(byteArr);
    }
    archive();
    emit showMessageOnServer("<" + jsonDoc.object()["username"].toString() + "> : " + jsonDoc.object()["message"].toString());
}

void Server::userLeft() {
    for (auto i = users.begin(); i != users.end(); i++) {
        if (*i == sender()) {
            QWebSocket *kek = (*i);
            users.erase(i);
            (kek)->deleteLater();
            break;
        }
    }

    for (auto i = users.begin(); i != users.end(); i++) {
        QByteArray bA = SystemCall("User left our chat =(");
        QWebSocket *sok = (*i);
        sok->sendBinaryMessage(bA);
    }
    emit updateUsers(users.size());
}

void Server::archive() {
    db.open();
    QSqlQuery writeData;
    writeData.prepare("INSERT INTO Messages VALUES (:data)");
    writeData.bindValue(":data", someList.back());
    writeData.exec();
    qDebug() << "Archivation complete!";
    db.close();


};

QByteArray Server::SystemCall(QString msg) {
    QJsonObject nuser;
    nuser.insert("username", "SYSTEM");
    nuser.insert("message", msg);
    QJsonDocument doc(nuser);
    QByteArray bA = doc.toBinaryData();
    return bA;
}
