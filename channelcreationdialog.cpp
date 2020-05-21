#include "channelcreationdialog.h"
#include "ui_channelcreationdialog.h"

#include "globals.h"

ChannelCreationDialog::ChannelCreationDialog(QWidget * parent) :
    FramelessWindow(parent),
    ui(new Ui::ChannelCreationDialog)
{
    QDialog * connectionDialog = new QDialog();
    ui->setupUi(connectionDialog);

    connect(ui->CreateButton,  SIGNAL(pressed()),
            this,              SLOT(on_CreateButton_pressed()));

    connect(ui->CancelButton,  SIGNAL(pressed()),
            this,              SLOT(on_CancelButton_pressed()));

    connect(ui->NameInput,     SIGNAL(returnPressed()),
            this,              SLOT(on_CreateButton_pressed()));

    connect(ui->PasswordInput, SIGNAL(returnPressed()),
            this,              SLOT(on_CreateButton_pressed()));

    this->setWindowTitle("Create channel");
    this->setContent(connectionDialog);
    this->setFixedSize(300, 150);
    this->setResizable(false);
}

ChannelCreationDialog::~ChannelCreationDialog()
{
    delete ui;
}

void ChannelCreationDialog::on_CreateButton_pressed()
{
    if (isHidden()) // dirty fix for a bug where this slot would be called twice
        return;

    if (ui->NameInput->text() == "")
        return;

    emit channelCreationRequested(ui->NameInput->text());
    ui->NameInput->setText("");
    hide();
}

void ChannelCreationDialog::on_CancelButton_pressed()
{
    hide();
}

void ChannelCreationDialog::show()
{
    if (ClientID < 0)
        return;

    ui->NameInput->setFocus();
    move(parentWidget()->rect().center() - this->rect().center());
    FramelessWindow::show();
}
