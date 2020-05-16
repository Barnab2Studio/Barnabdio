#include "tcpclient.h"

#include <QDebug>

TCPClient::TCPClient(QObject * parent)
    : QObject(parent)
{
}

TCPClient::~TCPClient()
{
}

void TCPClient::connect(QString const & server, QString const & port, QString const & password, QString const & name)
{
    qDebug() << server << port << password << name;
}

void TCPClient::sendChatMessage(QString const & message)
{

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
