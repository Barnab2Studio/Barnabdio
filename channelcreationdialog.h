#ifndef CHANNELCREATIONDIALOG_H
#define CHANNELCREATIONDIALOG_H

#include <QDialog>

#include "framelesswindow.h"

namespace Ui {
class ChannelCreationDialog;
}

class ChannelCreationDialog : public FramelessWindow
{
    Q_OBJECT

public:
    explicit ChannelCreationDialog(QWidget * parent = nullptr);
    ~ChannelCreationDialog();

public slots:
    void show();

private slots:
    void on_CreateButton_pressed();
    void on_CancelButton_pressed();

signals:
    void channelCreationRequested(QString const &);

private:
    Ui::ChannelCreationDialog * ui;
};

#endif // CHANNELCREATIONDIALOG_H
