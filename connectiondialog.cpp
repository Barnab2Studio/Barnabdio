#include "connectiondialog.h"
#include "ui_connectiondialog.h"

#include <QIntValidator>

ConnectionDialog::ConnectionDialog(QWidget * parent) :
    FramelessWindow(parent),
    ui(new Ui::ConnectionDialog)
{
    QDialog * connectionDialog = new QDialog(this);
    ui->setupUi(connectionDialog);

    connect(ui->ConnectButton, SIGNAL(pressed()),
            this,              SLOT(on_ConnectButton_pressed()));

    connect(ui->CancelButton, SIGNAL(pressed()),
            this,             SLOT(on_CancelButton_pressed()));

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

void ConnectionDialog::on_ConnectButton_pressed()
{
    emit connectionRequested(ui->ServerInput->text(), ui->PortInput->text(), ui->PasswordInput->text(), ui->NameInput->text());
    hide();
}

void ConnectionDialog::on_CancelButton_pressed()
{
    hide();
}

void ConnectionDialog::show()
{
    move(parentWidget()->rect().center() - this->rect().center());
    FramelessWindow::show();
}
