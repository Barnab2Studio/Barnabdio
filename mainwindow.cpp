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

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_channelList(new ChannelListModel(this))
    , m_volumeListLayout(new VolumeListLayout())
    , m_connectionDialog(nullptr)
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

    connect(m_channelList, SIGNAL(userChannelChanged(int, int)),
            m_tcpclient,   SLOT(notifyClientChannelChanged(int, int)));

    connect(m_tcpclient, SIGNAL(initEnd()),
            this,        SLOT(finishConnectionInit()));

    connect(m_tcpclient, SIGNAL(disconnected()),
            this,        SLOT(cleanup()));

    //Chat
    connect(m_tcpclient, SIGNAL(chatMessageRecieved(QString)), ui->ChatHistory1,SLOT(append(QString)));

    connect(ui->ChatInput, SIGNAL(returnPressed()),this,SLOT(chatInput_onReturnPressed()));
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

    connect(ui->treeView,  SIGNAL(doubleClicked(const QModelIndex &)),
            this,          SLOT(treeView_onDoubleClick(const QModelIndex &)));
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

        client->setChannel(channel);
        ui->treeView->setExpanded(index, false);
    }
}

void MainWindow::on_actionConnect_triggered()
{
    initConnectionDialog();
    m_connectionDialog->show();
}
