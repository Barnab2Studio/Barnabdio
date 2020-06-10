#include "MainWindow.h"
#include "ui_mainwindow.h"

#include "volumelistlayout.h"

#include "User.h"
#include "channel.h"
#include "channellistitem.h"
#include "channellistmodel.h"

#include "channelcreationdialog.h"
#include "connectiondialog.h"
#include "framelesswindow.h"

#include "tcpclient.h"

#include "globals.h"

#include <QMenu>
#include <QDebug>

#include <QtMultimedia/QAudioDeviceInfo>
#include <QtMultimedia/QAudioInput>
#include <QtMultimedia/QAudioOutput>
#include "recordingthread.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_channelList(new ChannelListModel(this))
    , m_volumeListLayout(new VolumeListLayout())
    , m_connectionDialog(nullptr)
    , m_channelCreationDialog(nullptr)
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

    connect(m_tcpclient, SIGNAL(userMoved(int, int)),
            this,        SLOT(expandClientChannel(int, int)));

    connect(m_tcpclient,   SIGNAL(userRenamed(int, QString const &)),
            m_channelList, SLOT(renameUser(int, QString const &)));

    connect(m_tcpclient,   SIGNAL(userDisconnected(int)),
            m_channelList, SLOT(removeUser(int)));

    connect(m_tcpclient,   SIGNAL(channelRenamed(int, QString const &)),
            m_channelList, SLOT(renameChannel(int, QString const &)));

    connect(m_tcpclient,   SIGNAL(channelDeleted(int)),
            m_channelList, SLOT(removeChannel(int)));

    connect(m_channelList, SIGNAL(channelChangeRequested(int, int)),
            m_tcpclient,   SLOT(sendChannelChangeRequest(int, int)));

    connect(m_channelList, SIGNAL(userNameChangeRequested(int, QString const &)),
            m_tcpclient,   SLOT(sendUserNameChangeRequest(int, QString const &)));

    connect(m_channelList, SIGNAL(channelNameChangeRequested(int, QString const &)),
            m_tcpclient,   SLOT(sendChannelNameChangeRequest(int, QString const &)));

    connect(m_tcpclient, SIGNAL(initEnd()),
            this,        SLOT(finishConnectionInit()));

    connect(m_tcpclient, SIGNAL(disconnected()),
            this,        SLOT(cleanup()));

    //Chat
    connect(m_tcpclient, SIGNAL(chatMessageRecieved(QString)), ui->ChatHistory1, SLOT(append(QString)));

    connect(ui->ChatInput, SIGNAL(returnPressed()),this,SLOT(chatInput_onReturnPressed()));



//    const QAudioDeviceInfo inputDeviceInfo = QAudioDeviceInfo::defaultInputDevice();
    const QAudioDeviceInfo outputDeviceInfo = QAudioDeviceInfo::defaultOutputDevice();

//    QAudioFormat formatAudio;
//    formatAudio.setSampleRate(8000);
//    formatAudio.setChannelCount(1);
//    formatAudio.setSampleSize(16);
//    formatAudio.setCodec("audio/pcm");
//    formatAudio.setByteOrder(QAudioFormat::LittleEndian);
//    formatAudio.setSampleType(QAudioFormat::SignedInt);

//    m_audioInput = new QAudioInput(inputDeviceInfo, formatAudio, this);

//    QAudioOutput* audioOutput = new QAudioOutput(outputDeviceInfo,formatAudio,this);
//    m_audioInput->start(audioOutput->start());


    const int BufferSize = 32000;
    char buffer[BufferSize];

    QWaitCondition bufferNotEmpty;
    QWaitCondition bufferNotFull;
    QMutex mutex;
    int numUsedBytes = 0;
    new RecordingThread(this,buffer,&bufferNotEmpty,&bufferNotFull,&mutex,&numUsedBytes);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::cleanup()
{
    ClientID = -1;
    m_volumeListLayout->clear();
    m_volumeListLayout->setChannel(nullptr);
    m_channelList->clear();

    if (m_channelCreationDialog)
    m_channelCreationDialog->hide();
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

    m_volumeListLayout->setChannel(static_cast<Channel *>(client->parent()));
    ui->treeView->expandAll();
}

void MainWindow::initTreeView()
{
    ui->treeView->setModel(m_channelList);
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->treeView,  SIGNAL(doubleClicked(const QModelIndex &)),
            this,          SLOT(treeView_onDoubleClick(const QModelIndex &)));

    connect(ui->treeView, SIGNAL(customContextMenuRequested(const QPoint &)),
            this,         SLOT(displayMenu(const QPoint &)));

    m_treeViewMenu->addAction(ui->actionCreateChannel);

    m_userMenu->addAction(ui->actionOpenPrivateChat);
    m_userMenu->addAction(ui->actionSendPoke);

    m_channelMenu->addAction(ui->actionRenameChannel);
    m_channelMenu->addAction(ui->actionDeleteChannel);
}

void MainWindow::expandClientChannel(int idChannel, int idUser)
{
    if (idUser != ClientID)
        return;

    QModelIndex index = m_channelList->match(m_channelList->index(0,0), Qt::UserRole, idChannel)[0];
    ui->treeView->expand(index);
}

void MainWindow::displayMenu(const QPoint & pos)
{
    if (ClientID < 0)
        return;

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
        {
            menu = m_channelMenu;
        }
    }

    if (menu->actions().size() == 0)
        return;

    QTimer::singleShot(0, []{}); /* fixes small display bug */
    menu->popup(ui->treeView->mapToGlobal(pos));
    m_previousActionIndex = index;
}

void MainWindow::initConnectionDialog()
{
    if (m_connectionDialog != nullptr)
        return;

    m_connectionDialog = new ConnectionDialog(this);

    connect(m_connectionDialog, SIGNAL(connectionRequested(QString const &, QString const &, QString const &, QString const &)),
            m_tcpclient,      SLOT(connect(QString const &, QString const &, QString const &, QString const &)));
}

void MainWindow::initChannelCreationDialog()
{
    if (m_channelCreationDialog != nullptr)
        return;

    m_channelCreationDialog = new ChannelCreationDialog(this);

    connect(m_channelCreationDialog, SIGNAL(channelCreationRequested(QString const)),
            m_tcpclient,             SLOT(sendChannelCreationRequest(QString const &)));
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

        if (channel == client->parent())
            return;

        m_tcpclient->sendChannelChangeRequest(channel->id(), client->id());
    }
}

void MainWindow::on_actionConnect_triggered()
{
    initConnectionDialog();
    m_connectionDialog->show();
}

void MainWindow::on_actionCreateChannel_triggered()
{
    initChannelCreationDialog();
    m_channelCreationDialog->show();
}

void MainWindow::on_actionRenameChannel_triggered()
{
    ChannelListItem * item = static_cast<ChannelListItem *>(m_previousActionIndex.internalPointer());
    item->setFlags(item->flags() | Qt::ItemIsEditable);

    ui->treeView->edit(m_previousActionIndex);
    m_previousActionIndex = QModelIndex();
}

void MainWindow::on_actionDeleteChannel_triggered()
{
    ChannelListItem * item = static_cast<ChannelListItem *>(m_previousActionIndex.internalPointer());

    m_tcpclient->sendChannelDeletionRequest(item->id());
    m_previousActionIndex = QModelIndex();
}
