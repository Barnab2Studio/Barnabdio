#ifndef CONNECTIONDIALOG_H
#define CONNECTIONDIALOG_H

#include <QDialog>

#include "framelesswindow.h"

namespace Ui {
class ConnectionDialog;
}

class ConnectionDialog : public FramelessWindow
{
    Q_OBJECT

public:
    explicit ConnectionDialog(QWidget * parent = nullptr);
    ~ConnectionDialog();

public slots:
    void show();

private slots:
    void on_ConnectButton_pressed();
    void on_CancelButton_pressed();

signals:
    void connectionRequested(QString const &, QString const &, QString const &, QString const &);

private:
    Ui::ConnectionDialog * ui;
};

#endif // CONNECTIONDIALOG_H
