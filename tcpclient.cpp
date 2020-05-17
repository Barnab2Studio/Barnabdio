#include "tcpclient.h"

#include <QDebug>

TCPClient::TCPClient(QObject * parent)
    : QObject(parent)
{
    socket = new QTcpSocket(this);

    QObject::connect(socket,SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(errorSocket(QAbstractSocket::SocketError)));
    QObject::connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(dataReceived()));
}

TCPClient::~TCPClient()
{
    socket->close();
}

void TCPClient::connect(QString const & server, QString const & port, QString const & password, QString const & name)
{
    qDebug() << server << port << password << name;
    socket->connectToHost(server,port.toInt());

    //Shlaguerie as long as server do not handle auth process
    qDebug() <<"Connected to server";
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);
    qDebug() << "peusdo : " << name.toUtf8();

    QByteArray pseudo = (name + "\n").toUtf8();

    out.writeRawData(pseudo,pseudo.length());

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
    // In the future, should dispatch message handling accordingly to message type (server info, chat, ect...)
    QByteArray incomingData = socket->readAll();
    qDebug() <<"Data received: " << QString::fromUtf8(incomingData);
    emit chatMessageRecieved(QString::fromUtf8(incomingData));
}



void TCPClient::notifyClientNameChanged(QString const & name)
{

}

void TCPClient::notifyClientChannelChanged(int id)
{

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
