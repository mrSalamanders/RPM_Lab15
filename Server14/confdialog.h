#ifndef CONFDIALOG_H
#define CONFDIALOG_H

#include <QDialog>
#include <QtWidgets>
#include <QLineEdit>
#include "server.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ConfDialog; }
QT_END_NAMESPACE

class ConfDialog : public QMainWindow {
        Q_OBJECT
public:
    ConfDialog(QWidget *parent = nullptr);
    ~ConfDialog();
    /**
    * @brief closeEvent функция события закрытия
    * @param event событие
    */
   void closeEvent(QCloseEvent *event);
   /**
    * @brief setPortToDialog данные о порте в диалог
    * @param instance я не знаю как по-русски инцтанц
    */
   void setPortToDialog(Server *instance);
private:
    Ui::ConfDialog *ui;
signals:
    /**
     * @brief setPort сигнал установки порта
     * @param port сам порт
     */
    void setPort(qint16 port);
};

#endif // CONFDIALOG_H
