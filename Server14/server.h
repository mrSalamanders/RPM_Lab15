#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QtNetwork>
#include <list>
#include <QWebSocket>
#include <QWebSocketServer>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlDatabase>

class Server : public QObject
{
    Q_OBJECT
private:
    /**
     * @brief tcpServer
     * Сервер отвечающий за работу TCP соединения
     */
    QWebSocketServer* tcpServer;
    /**
     * @brief client
     * Переменная которая будет хранить соединениес клиентом
     */
    QWebSocket* client;
    /**
     * @brief db база данных (скулайт)
     */
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
public:
    /**
     * @brief Server конструктор сервера
     * @param parent родительский объект
     */
    explicit Server(QObject *parent = nullptr);
    /// Это список подключённых пользователей
    std::list<QWebSocket*> users;
    /// Это список последних сообщений чата
    std::list<QByteArray> someList;
    ///
    /// @brief Эта функция комплектует системное сообщение
    /// @param msg этот параметр - текст системного сообщения
    /// @return возвращает QByteArray, содержащий системное сообщение
    ///
    QByteArray SystemCall(QString msg);
    /// Это директория с файлом-архивом
    QDir directory;
    /// Это абсолютный путь к файлу-архиву
    QString filepath;
    /**
     * @brief dbpath путь к бд
     */
    QString dbpath;
    ///
    /// @brief Эта функция записывает последние 10 сообщений в файл
    /// Можно было бы реализовать запись по закрытию программы
    /// Но для этого нужно было бы использовать системные сигналы
    /// Которые сильно разнятся от системы к системе
    /// Поэтому ради совместимости платформ было решено записывать историю после каждого нового сообщения
    /// Это также увеличит надёжность в случаях перебоев питания
    ///
    void archive();
    /**
     * @brief startup пуск
     */
    void startup();
    /**
     * @brief serverPort порт сервера
     */
    qint16 serverPort;
private slots:
    ///
    /// @brief Это слот, который срабатывает при подключении пользователя
    ///
    void connectUser();
    ///
    /// @brief Это слот, срабатывающий по сигналу readyRead со стороны пользователя
    ///
    void runClientAction(QByteArray data);
    ///
    /// @brief Этот слот срабатывает, когда пользователь отключается
    ///
    void userLeft();

signals:
    /**
     * @brief updateUsers обновить число юзеров
     * @param num число
     */
    void updateUsers(int num);
    /**
     * @brief updateState обновить состояние сервера
     * @param state состояние
     */
    void updateState(QString state);
    /**
     * @brief showMessageOnServer показать сообщение на сервере
     * @param message само сообщение
     */
    void showMessageOnServer(QString message);
};
#endif // SERVER_H
