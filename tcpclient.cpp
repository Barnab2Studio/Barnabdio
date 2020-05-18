#include "tcpclient.h"

#include <QDebug>

#include "globals.h"

TCPClient::TCPClient(QObject * parent)
    : QObject(parent)
{
    socket = new QTcpSocket(this);

    QObject::connect(socket,SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(errorSocket(QAbstractSocket::SocketError)));
    QObject::connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(dataReceived()));
    QObject::connect(socket, SIGNAL(disconnected()), this, SIGNAL(disconnected()));

    dataHandler = {
        {UserError, &TCPClient::handleUserError},
        {ChatMessage, &TCPClient::handleChatMessage},
        {ServerMessage, &TCPClient::handleServerMessage},
        {StartupInfo, &TCPClient::handleStartupInfo},
        {ChannelList, &TCPClient::handleChannelList},
        {UserList, &TCPClient::handleUserList},
        {GoodPassword, &TCPClient::handleGoodPassword},
        {WrongPassword, &TCPClient::handleWrongPassword},
        {UserJoinedChannel, &TCPClient::handleUserJoinedChannel},
        {ChannelCreated, &TCPClient::handleChannelCreated},
        {ChannelDeleted, &TCPClient::handleChannelDeleted}
    };
}

TCPClient::~TCPClient()
{
    socket->close();
}

void TCPClient::connect(QString const & server, QString const & port, QString const & password, QString const & name)
{
    if (socket->state() == QAbstractSocket::ConnectedState)
    {
        emit chatMessageRecieved("Disconnecting...");
        socket->disconnectFromHost();
    }

    socket->connectToHost(server,port.toInt());

    emit chatMessageRecieved("Connecting to server " + server);
    if (!socket->waitForConnected(3000)) {
        chatMessageRecieved("Connection failed");
        return;
    }

    //Shlaguerie as long as server do not handle auth process
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    QByteArray authMessage = (password +";" + name).toUtf8();

    out.writeRawData(authMessage,authMessage.length());

    socket->write(paquet);
}

void TCPClient::sendChatMessage(QString const & message)
{
    QByteArray packet;
    QDataStream out(&packet, QIODevice::WriteOnly);
    out.writeRawData(message.toUtf8(),message.toUtf8().length());
    socket->write(packet);
}

void TCPClient::dataReceived()
{
    while (socket->canReadLine())
    {
        QStringList incomingData = QString(socket->readLine()).trimmed().split(';');

        if (incomingData.size() == 0)
        {
            qDebug() << "Empty message received";
            continue;
        }

        MessageType type = static_cast<MessageType>(incomingData[0].toInt());
        if (dataHandler.count(type) == 0)
        {
            qDebug() << "Message type not supported : " << type;
            continue;
        }

        (this->*dataHandler[type])(incomingData);
    }
}

void TCPClient::notifyClientNameChanged(QString const & name)
{

}

void TCPClient::notifyClientChannelChanged(int idChannel, int idUser)
{
    QByteArray message = (QString::number(UserJoinedChannel) + ";" + QString::number(idChannel) + ";" +  QString::number(idUser) + "\n").toUtf8();
    qDebug() << "notifyClientChannelChanged" << idChannel << idUser;
    QByteArray packet;
    QDataStream out(&packet, QIODevice::WriteOnly);
    out.writeRawData(message, message.length());
    socket->write(packet);
}


void TCPClient::notifyChannelCreated(int id, QString const & name)
{

}

void TCPClient::notifyChannelDeleted(int id)
{

}

void TCPClient::notifyChannelRenamed(int id, QString const & name)
{

}

void TCPClient::handleUserError(QStringList const & data)
{
    qDebug() << __func__;
}

void TCPClient::handleChatMessage(QStringList const & data)
{
    if (data.size() < 2)
    {
        qDebug() << "Bad chat message format";
        return;
    }

    QString message = data[1];
    for (auto it = data.begin() + 2 ; it != data.end() ; ++it)
        message += ';' + *it;

    emit chatMessageRecieved(message);
}

void TCPClient::handleServerMessage(QStringList const & data)
{
    qDebug() << __func__;
}

void TCPClient::handleStartupInfo(QStringList const & data)
{
    if (data.size() != 2)
    {
        qDebug() << "Bad startup info format";
        return;
    }

    ClientID = data[1].toInt();
}

void TCPClient::handleChannelList(QStringList const & data)
{
    for (auto it = data.begin() + 1 ; it != data.end() ; ++it)
    {
        QStringList channelData = it->split('|');
        if (channelData.size() != 2)
        {
            qDebug() << "Bad format for channel" << *it;
            continue;
        }
        emit channelCreated(channelData[0].toInt(), channelData[1]);
    }
}

void TCPClient::handleUserList(QStringList const & data)
{
    for (auto it = data.begin() + 1 ; it != data.end() ; ++it)
    {
        QStringList userData = it->split('|');
        if (userData.size() != 3)
        {
            qDebug() << "Bad format for user" << *it;
            continue;
        }
        emit userConnected(userData[0].toInt(), userData[1], userData[2].toInt());
    }
    emit initEnd();
}

void TCPClient::handleGoodPassword(QStringList const & data)
{
    emit chatMessageRecieved("Connected to server");
}

void TCPClient::handleWrongPassword(QStringList const & data)
{
    emit chatMessageRecieved("Invalid password");
    emit chatMessageRecieved("Connection failed");
}

void TCPClient::handleUserJoinedChannel(QStringList const & data)
{
    if (data.size() != 3)
    {
        qDebug() << "Bad format for user joined channel";
        return;
    }

    qDebug() << "handleUserJoinedChannel" << data[1].toInt() << data[2].toInt();
    emit userMoved(data[1].toInt(), data[2].toInt());
}

void TCPClient::handleChannelCreated(QStringList const & data)
{
    qDebug() << __func__;
}

void TCPClient::handleChannelDeleted(QStringList const & data)
{
    qDebug() << __func__;
}

