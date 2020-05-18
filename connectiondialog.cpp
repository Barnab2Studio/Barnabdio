#include "connectiondialog.h"
#include "ui_connectiondialog.h"

#include <QIntValidator>

#include <QDebug>

ConnectionDialog::ConnectionDialog(QWidget * parent) :
    FramelessWindow(parent),
    ui(new Ui::ConnectionDialog)
{
    QDialog * connectionDialog = new QDialog();
    ui->setupUi(connectionDialog);

    connect(ui->ConnectButton, SIGNAL(pressed()),
            this,              SLOT(onConnectButton_pressed()));

    connect(ui->CancelButton,  SIGNAL(pressed()),
            this,              SLOT(onCancelButton_pressed()));

    connect(ui->ServerInput,   SIGNAL(returnPressed()),
            this,              SLOT(onConnectButton_pressed()));

    connect(ui->PortInput,     SIGNAL(returnPressed()),
            this,              SLOT(onConnectButton_pressed()));

    connect(ui->PasswordInput, SIGNAL(returnPressed()),
            this,              SLOT(onConnectButton_pressed()));

    connect(ui->NameInput,     SIGNAL(returnPressed()),
            this,              SLOT(onConnectButton_pressed()));

    this->setWindowTitle("Connection");
    this->setContent(connectionDialog);
    this->setFixedSize(400, 250);
    this->setResizable(false);

    ui->PortInput->setValidator(new QIntValidator(0, 65535, this));
}

ConnectionDialog::~ConnectionDialog()
{
    delete ui;
}

void ConnectionDialog::onConnectButton_pressed()
{
    if (isHidden()) // dirty fix for a bug where this slot would be called twice
        return;

    emit connectionRequested(ui->ServerInput->text(), ui->PortInput->text(), ui->PasswordInput->text(), ui->NameInput->text());
    hide();
}

void ConnectionDialog::onCancelButton_pressed()
{
    hide();
}

void ConnectionDialog::show()
{
    ui->ServerInput->setFocus();
    move(parentWidget()->rect().center() - this->rect().center());
    FramelessWindow::show();
}
