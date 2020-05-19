#include "MainWindow.h"
#include "ui_mainwindow.h"

#include "volumelistlayout.h"

#include "User.h"
#include "channel.h"
#include "channellistitem.h"
#include "channellistmodel.h"

#include "connectiondialog.h"
#include "framelesswindow.h"

#include "tcpclient.h"

#include "globals.h"

#include <QMenu>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_channelList(new ChannelListModel(this))
    , m_volumeListLayout(new VolumeListLayout())
    , m_connectionDialog(nullptr)
    , m_treeViewMenu(new QMenu(this))
    , m_userMenu(new QMenu(this))
    , m_clientMenu(new QMenu(this))
    , m_channelMenu(new QMenu(this))
    , m_tcpclient(new TCPClient(this))
{
    ui->setupUi(this);

    initTreeView();

    ui->ToolBar->setContextMenuPolicy(Qt::PreventContextMenu);
    ui->scrollAreaWidgetContents->setLayout(m_volumeListLayout);

    connect(m_tcpclient, SIGNAL(channelCreated(int, QString const &)),
            m_channelList, SLOT(addChannel(int, QString const &)));

    connect(m_tcpclient, SIGNAL(userConnected(int, QString const &, int)),
            m_channelList, SLOT(addUser(int, QString const &, int)));

    connect(m_tcpclient, SIGNAL(userMoved(int, int)),
            m_channelList, SLOT(moveUser(int, int)));

    connect(m_tcpclient, SIGNAL(userRenamed(int, QString const &)),
            m_channelList, SLOT(renameUser(int, QString const &)));

    connect(m_channelList, SIGNAL(channelChangeRequested(int, int)),
            m_tcpclient,   SLOT(sendChannelChangeRequest(int, int)));

    connect(m_channelList, SIGNAL(userNameChangeRequested(int, QString const &)),
            m_tcpclient,   SLOT(sendUserNameChangeRequest(int, QString const &)));

    connect(m_tcpclient, SIGNAL(initEnd()),
            this,        SLOT(finishConnectionInit()));

    connect(m_tcpclient, SIGNAL(disconnected()),
            this,        SLOT(cleanup()));

    //Chat
    connect(m_tcpclient, SIGNAL(chatMessageRecieved(QString)), ui->ChatHistory1,SLOT(append(QString)));

    connect(ui->ChatInput, SIGNAL(returnPressed()),this,SLOT(chatInput_onReturnPressed()));

    m_treeViewMenu->addAction(ui->actionCreateChannel);
    m_treeViewMenu->addSeparator();
    m_treeViewMenu->addAction(ui->actionExpandAll);
    m_treeViewMenu->addAction(ui->actionCollapseAll);

    m_clientMenu->addAction(ui->actionRenameClient);

    m_userMenu->addAction(ui->actionOpenPrivateChat);
    m_userMenu->addAction(ui->actionSendPoke);

    m_channelMenu->addAction(ui->actionEditChannel);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::cleanup()
{
    m_volumeListLayout->clear();
    m_volumeListLayout->setChannel(nullptr);
    m_channelList->clear();
}

void MainWindow::chatInput_onReturnPressed()
{
    User * client = m_channelList->getUserFromId(ClientID);

    if (client == nullptr)
    {
        qDebug() << __func__ << "Error: Client is not initialized";
        return;
    }

    QByteArray message = ("0;" + client->name() + ":" + ui->ChatInput->text() + "\n").toUtf8();
    qDebug() << "Sending message : " << QString::fromUtf8(message);
    m_tcpclient->sendChatMessage(message);
    ui->ChatInput->setText(QString());
}

void MainWindow::finishConnectionInit()
{
    User * client = m_channelList->getUserFromId(ClientID);

    if (client == nullptr)
    {
        qDebug() << __func__ << "Error: Init client failed";
        return;
    }

    connect(client,             SIGNAL(channelChanged(Channel *)),
            m_volumeListLayout, SLOT(setChannel(Channel *)));

    ui->treeView->expandAll();
}

void MainWindow::initTreeView()
{
    ui->treeView->setHeaderHidden(true);
    ui->treeView->setAcceptDrops(true);
    ui->treeView->setDragEnabled(true);
    ui->treeView->setDragDropMode(QAbstractItemView::InternalMove);
    ui->treeView->setModel(m_channelList);
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->treeView,  SIGNAL(doubleClicked(const QModelIndex &)),
            this,          SLOT(treeView_onDoubleClick(const QModelIndex &)));

    connect(ui->treeView, SIGNAL(customContextMenuRequested(const QPoint &)),
            this,         SLOT(displayMenu(const QPoint &)));
}

void MainWindow::displayMenu(const QPoint & pos)
{
    QMenu * menu;
    QModelIndex index = ui->treeView->indexAt(pos);

    if (!index.isValid())
    {
        menu = m_treeViewMenu;
    }
    else
    {
        ChannelListItem * item = static_cast<ChannelListItem *>(index.internalPointer());
        User * user = qobject_cast<User *>(item);
        if (user)
            menu = ((user->id() == ClientID) ? m_clientMenu : m_userMenu);
        else
            menu = m_channelMenu;
    }

    QTimer::singleShot(0, []{}); /* fixes small display bug */
    menu->popup(ui->treeView->mapToGlobal(pos));
}

void MainWindow::initConnectionDialog()
{
    if (m_connectionDialog != nullptr)
        return;

    m_connectionDialog = new ConnectionDialog(this);

    connect(m_connectionDialog, SIGNAL(connectionRequested(QString const &, QString const &, QString const &, QString const &)),
            m_tcpclient,      SLOT(connect(QString const &, QString const &, QString const &, QString const &)));
}

void MainWindow::treeView_onDoubleClick(const QModelIndex & index)
{
    ChannelListItem * item = static_cast<ChannelListItem *>(index.internalPointer());

    Channel * channel = qobject_cast<Channel *>(item);
    if (channel != nullptr)
    {
        User * client = m_channelList->getUserFromId(ClientID);

        if (client == nullptr)
        {
            qDebug() << __func__ << "Error: Client is not initialized";
            return;
        }

        if (channel != client->parent())
            m_tcpclient->sendChannelChangeRequest(channel->id(), client->id());
        ui->treeView->setExpanded(index, false);
    }
}

void MainWindow::on_actionConnect_triggered()
{
    initConnectionDialog();
    m_connectionDialog->show();
}
