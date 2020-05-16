#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "volumelistlayout.h"

#include "user.h"
#include "channel.h"
#include "channellistitem.h"
#include "channellistmodel.h"

#include "textchathandler.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , textChatHandler(new TextChatHandler())
    , m_client(new User(0, "Fragi"))

{
    ui->setupUi(this);
    setWindowTitle("Merguez");

    ui->ToolBar->setContextMenuPolicy(Qt::PreventContextMenu);

    model = new ChannelListModel(this);
    connect(ui->treeView,  SIGNAL(doubleClicked(const QModelIndex &)),
            this,          SLOT(treeView_onDoubleClick(const QModelIndex &)));

    m_volumeListLayout = new VolumeListLayout();
    ui->scrollAreaWidgetContents->setLayout(m_volumeListLayout);

    connect(m_client,           SIGNAL(channelChanged(Channel *)),
            m_volumeListLayout, SLOT(setChannel(Channel *)));

    ui->treeView->setHeaderHidden(true);
    ui->treeView->setAcceptDrops(true);
    ui->treeView->setDragEnabled(true);
    ui->treeView->setDragDropMode(QAbstractItemView::InternalMove);

    ui->treeView->setModel(model);

    Channel * defaultChannel = model->addChannel(1, "Default channel");
    Channel * defaultChannel2 = model->addChannel(2, "Default channel2");
    Channel * defaultChannel3 = model->addChannel(3, "Default channel3");
    m_client->setChannel(defaultChannel);

    User * u1 = new User(2, "Clemi");
    u1->setChannel(defaultChannel);

    User * u2 = new User(3, "Brekzo");
    u2->setChannel(defaultChannel2);

    User * u3 = new User(4, "Valou");
    u3->setChannel(defaultChannel2);

    User * u4 = new User(5, "Killearth");
    u4->setChannel(defaultChannel3);

    User * u5 = new User(6, "Pascal");
    u5->setChannel(defaultChannel3);

    User * u6 = new User(7, "Linael");
    u6->setChannel(defaultChannel3);

    User * u7 = new User(8, "Namastey");
    u7->setChannel(defaultChannel2);

    ui->treeView->expandAll();

    //Chat
    connect(textChatHandler,SIGNAL(messageReceived(QString)),ui->ChatHistory1,SLOT(append(QString)));
    connect(ui->ChatInput, SIGNAL(returnPressed()),this,SLOT(chatInput_onReturnPressed()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::chatInput_onReturnPressed()
{
    QByteArray message = (m_client->name() +":" + ui->ChatInput->text() +"\n").toUtf8();
//    QString messageAEnvoyer = m_client->name() +":" + ui->ChatInput->text() +"\n";
    qDebug() << "Sending message : " << QString::fromUtf8(message);
    textChatHandler->sendMessage(message);

}

void MainWindow::treeView_onDoubleClick(const QModelIndex & index)
{
    ChannelListItem * item = static_cast<ChannelListItem *>(index.internalPointer());

    Channel * channel = qobject_cast<Channel *>(item);
    if (channel != nullptr && channel->parent() != nullptr)
    {
        m_client->setChannel(channel);
        ui->treeView->setExpanded(index, false);
    }
}

