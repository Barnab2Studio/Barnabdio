#include "textchathandler.h"

#include <chrono>

using namespace std;

TextChatHandler::TextChatHandler()
{

    qDebug() << "Creation du TextChatHandler";
    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(errorSocket(QAbstractSocket::SocketError)));

    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(dataReceived()));

    socket->connectToHost("127.0.0.1",5555);


}

void TextChatHandler::sendMessage(QByteArray message){
    QByteArray packet;
    QDataStream out(&packet, QIODevice::WriteOnly);
    out.writeRawData(message,message.length());
    socket->write(packet);
}

void TextChatHandler::connected()
{
    qDebug() <<"Connected to server";
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    QString messageAEnvoyer = "Johny Client\n";

    qDebug() << "message à envoyer : " << messageAEnvoyer.toUtf8();
    out.writeRawData(messageAEnvoyer.toUtf8(),messageAEnvoyer.toUtf8().length());


    socket->write(paquet); // On envoie le paquet
}

void TextChatHandler::dataReceived()
{
    QString message = QString::fromUtf8(socket->readAll());
    qDebug() <<"Data: " << message;
    emit messageReceived(message);
}

void TextChatHandler::errorSocket(QAbstractSocket::SocketError erreur)
{
    switch(erreur)
    {
    case QAbstractSocket::HostNotFoundError:
        qDebug() << "Host not found";
        break;
    case QAbstractSocket::ConnectionRefusedError:
        qDebug() << "Server refused connection";
        break;
    case QAbstractSocket::RemoteHostClosedError:
        qDebug() << "Server closed connection";
        break;
    default:
        qDebug() << "ERROR: " << socket->errorString();
    }
}
