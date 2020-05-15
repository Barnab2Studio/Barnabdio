#ifndef TEXTCHATHANDLER_H
#define TEXTCHATHANDLER_H

#include <QtWidgets>
#include <QtNetwork>
#include <QObject>

class TextChatHandler : public QWidget
{
     Q_OBJECT
public:
    TextChatHandler();

private slots:
     void errorSocket(QAbstractSocket::SocketError erreur);
     void connected();
     void dataReceived();
private:
    QTcpSocket *socket; // Représente le serveur
    qint32 tailleMessage;
    QDataStream in;
};

#endif // TEXTCHATHANDLER_H
