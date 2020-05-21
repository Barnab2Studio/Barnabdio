#include "tcpclient.h"

#include <QDebug>

#include "globals.h"

TCPClient::TCPClient(QObject * parent)
    : QObject(parent)
{
    socket = new QTcpSocket(this);

    QObject::connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(errorSocket(QAbstractSocket::SocketError)));
    QObject::connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(dataReceived()));
    QObject::connect(socket, SIGNAL(disconnected()), this, SIGNAL(disconnected()));

    dataHandler = {
        {UserError,         &TCPClient::handleUserError},
        {ChatMessage,       &TCPClient::handleChatMessage},
        {ServerMessage,     &TCPClient::handleServerMessage},
        {StartupInfo,       &TCPClient::handleStartupInfo},
        {ChannelList,       &TCPClient::handleChannelList},
        {UserList,          &TCPClient::handleUserList},
        {GoodPassword,      &TCPClient::handleGoodPassword},
        {WrongPassword,     &TCPClient::handleWrongPassword},
        {UserJoinedChannel, &TCPClient::handleUserJoinedChannel},
        {UserRenamed,       &TCPClient::handleUserRenamed},
        {UserDisconnected,  &TCPClient::handleUserDisconnected},
        {ChannelCreated,    &TCPClient::handleChannelCreated},
        {ChannelRenamed,    &TCPClient::handleChannelRenamed},
        {ChannelDeleted,    &TCPClient::handleChannelDeleted},
        {RequestDenied,    &TCPClient::handleRequestDenied},
        {EndOfTransmission, &TCPClient::handleEndOfTransmission}
    };
}

TCPClient::~TCPClient()
{
    socket->close();
}

void TCPClient::sendRequest(QString const & request)
{
    qDebug() << "pouet" << request;
    QByteArray packet;
    QDataStream out(&packet, QIODevice::WriteOnly);
    out.writeRawData(request.toUtf8(), request.toUtf8().length());
    socket->write(packet);
}

void TCPClient::connect(QString const & server, QString const & port, QString const & password, QString const & name)
{
    if (socket->state() == QAbstractSocket::ConnectedState)
    {
        emit chatMessageRecieved("Disconnecting...");
        socket->disconnectFromHost();
    }

    this->name = name;
    socket->connectToHost(server,port.toInt());

    emit chatMessageRecieved("Connecting to server " + server);
    if (!socket->waitForConnected(3000)) {
        chatMessageRecieved("Connection failed");
        return;
    }

    //Shlaguerie as long as server do not handle auth process
    sendRequest((password +";" + name).toUtf8());
}

void TCPClient::sendChatMessage(QString const & message)
{
    sendRequest(message);
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

void TCPClient::sendUserNameChangeRequest(int id, QString const & name)
{
    QByteArray message = (QString::number(UserRenamed) + ";" + QString::number(id) + ";" +  name + "\n").toUtf8();
    sendRequest(message);
}

void TCPClient::sendChannelChangeRequest(int idChannel, int idUser)
{
    QByteArray message = (QString::number(UserJoinedChannel) + ";" + QString::number(idChannel) + ";" +  QString::number(idUser) + "\n").toUtf8();
    sendRequest(message);
}

void TCPClient::sendChannelCreationRequest(QString const & name)
{
    QByteArray message = (QString::number(ChannelCreated) + ";" + name + "\n").toUtf8();
    sendRequest(message);
}

void TCPClient::sendChannelDeletionRequest(int id)
{
    QByteArray message = (QString::number(ChannelDeleted) + ";" + QString::number(id) + "\n").toUtf8();
    sendRequest(message);
}

void TCPClient::sendChannelNameChangeRequest(int id, QString const & name)
{
    QByteArray message = (QString::number(ChannelRenamed) + ";" + QString::number(id) + ";" + name + "\n").toUtf8();
    sendRequest(message);
}

void TCPClient::handleUserError(QStringList const & data)
{
    qDebug() << __func__;
}

void TCPClient::handleChatMessage(QStringList const & data)
{
    if (data.size() < 2)
    {
        qDebug() << "Bad format for " << __func__;
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
        qDebug() << "Bad format for " << __func__;
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
            qDebug() << "Bad format for " << __func__ << *it;
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

            qDebug() << "Bad format for " << __func__ << *it;
            continue;
        }
        emit userConnected(userData[0].toInt(), userData[1], userData[2].toInt());
    }
}

void TCPClient::handleGoodPassword(QStringList const & data)
{
    emit chatMessageRecieved("Connected to server as " + name);
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
        qDebug() << "Bad format for " << __func__;
        return;
    }

    emit userMoved(data[1].toInt(), data[2].toInt());
}

void TCPClient::handleUserRenamed(QStringList const & data)
{
    if (data.size() != 3)
    {
        qDebug() << "Bad format for " << __func__;
        return;
    }

    emit userRenamed(data[1].toInt(), data[2]);
}

void TCPClient::handleUserDisconnected(QStringList const & data)
{
    if (data.size() != 2)
    {
        qDebug() << "Bad format for " << __func__;
        return;
    }
    emit userDisconnected(data[1].toInt());
}

void TCPClient::handleChannelCreated(QStringList const & data)
{
    if (data.size() != 3)
    {
        qDebug() << "Bad format for " << __func__;
        return;
    }
    emit channelCreated(data[1].toInt(), data[2]);
}

void TCPClient::handleChannelRenamed(QStringList const & data)
{
    if (data.size() != 3)
    {
        qDebug() << "Bad format for " << __func__;
        return;
    }

    emit channelRenamed(data[1].toInt(), data[2]);
}

void TCPClient::handleChannelDeleted(QStringList const & data)
{
    if (data.size() != 2)
    {
        qDebug() << "Bad format for " << __func__;
        return;
    }

    emit channelDeleted(data[1].toInt());
}

void TCPClient::handleRequestDenied(const QStringList & data)
{
    if (data.size() != 2)
    {
        qDebug() << "Bad format for " << __func__;
        return;
    }

    emit chatMessageRecieved("Request denied: " + data[1]);
}

void TCPClient::handleEndOfTransmission(QStringList const & data)
{
    emit initEnd();
}
