#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "server.h"
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QAbstractButton>
#include "confdialog.h"
#include <QSettings>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    /**
     * @brief server сервер
     */
    Server *server;
    /**
     * @brief tray икона для трея
     */
    QSystemTrayIcon tray;
    /**
     * @brief trayMenu трей или меню бар
     */
    QMenu* trayMenu;
    /**
     * @brief closeEvent событие закрытия
     * @param event само событие
     */
    void closeEvent(QCloseEvent *event);
    /**
     * @brief firstPortSetup ввод порта при инициализации
     */
    void firstPortSetup();
    /**
     * @brief setup настройки
     */
    QSettings *setup;

private:
    Ui::MainWindow *ui;
    /**
     * @brief configure диалог для конфигураций
     */
    ConfDialog *configure;

signals:

public slots:
    /**
     * @brief setUsers установка числа пользователей
     * @param cnt число
     */
    void setUsers(int cnt);
    /**
     * @brief setState установка состояния сервера
     * @param state состояние
     */
    void setState(QString state);
    /**
     * @brief handleTray обработчик действий с треем
     * @param reason причина активации
     */
    void handleTray(QSystemTrayIcon::ActivationReason reason);
    /**
     * @brief showAdvanced показ окна
     */
    void showAdvanced();
    /**
     * @brief configureServer настройка сервера
     */
    void configureServer();
    /**
     * @brief saveSettings сохранение настроек
     * @param port порт
     */
    void saveSettings(qint16 port);
};

#endif // MAINWINDOW_H
