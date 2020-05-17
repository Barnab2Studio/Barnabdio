#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QtNetwork>
#include <QtWidgets>

class TCPClient : public QObject
{
    Q_OBJECT
public:
    explicit TCPClient(QObject * parent = nullptr);
    virtual ~TCPClient();

public slots:
    void connect(QString const & server, QString const & port, QString const & password, QString const & name);


    void dataReceived(); //Handle packet reception and then emit signal accordingly to packet content

    void sendChatMessage(QString const & message); // in the future, we will specify for which channel we must send the message

    void notifyClientNameChanged(QString const & name);
    void notifyClientChannelChanged(int id);

    void notifyChannelCreated(int id, QString const & name);
    void notifyChannelDeleted(int id);
    void notifyChannelRenamed(int id, QString const & name);

//    void errorSocket(QAbstractSocket::SocketError erreur);

signals:
    void chatMessageRecieved(QString const & message);

    void userConnected(int id, QString const & name);
    void userDisconnected(int id);
    void userMoved(int id, QString const & name);
    void userRenamed(int id, QString const & name);

    void channelCreated(int id, QString const & name);
    void channelDeleted(int id);
    void channelRenamed(int id, QString const & name);

private:
    QTcpSocket *socket;
};

#endif // TCPCLIENT_H
