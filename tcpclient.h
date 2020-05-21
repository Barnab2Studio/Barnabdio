#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QMap>
#include <QObject>
#include <QtNetwork>

class TCPClient : public QObject
{
    Q_OBJECT
public:
    explicit TCPClient(QObject * parent = nullptr);
    virtual ~TCPClient();

public slots:
    void connect(QString const & server, QString const & port, QString const & password, QString const & name);

    void dataReceived();

    void sendChatMessage(QString const & message);

    void sendUserNameChangeRequest(int id, QString const & name);
    void sendChannelChangeRequest(int idChannel, int idUser);

    void sendChannelCreationRequest(QString const & name);
    void sendChannelDeletionRequest(int id);
    void sendChannelNameChangeRequest(int id, QString const & name);
    void sendRequest(QString const & request);

signals:
    void chatMessageRecieved(QString const & message);

    void userConnected(int id, QString const & name, int channel);
    void userDisconnected(int id);
    void userMoved(int idChannel, int idUser);
    void userRenamed(int id, QString const & name);

    void channelCreated(int id, QString const & name);
    void channelDeleted(int id);
    void channelRenamed(int id, QString const & name);

    void initEnd();
    void disconnected();

private:
    QTcpSocket * socket;
    QString name;

    enum MessageType
    {
        UserError = -1,
        ChatMessage = 0,
        ServerMessage = 1,
        StartupInfo = 10,
        ChannelList = 11,
        UserList = 12,
        GoodPassword = 20,
        WrongPassword = -20,
        UserJoinedChannel = 31,
        UserRenamed = 32,
        UserDisconnected = 33,
        ChannelCreated = 42,
        ChannelRenamed = 43,
        ChannelDeleted = 44,
        RequestDenied = 98,
        EndOfTransmission = 99
    };
    typedef void (TCPClient::*parseDataFp)(QStringList const &);
    QMap<MessageType, parseDataFp> dataHandler;

    void handleUserError(QStringList const & data);
    void handleChatMessage(QStringList const & data);
    void handleServerMessage(QStringList const & data);
    void handleStartupInfo(QStringList const & data);
    void handleChannelList(QStringList const & data);
    void handleUserList(QStringList const & data);
    void handleGoodPassword(QStringList const & data);
    void handleUserJoinedChannel(QStringList const & data);
    void handleUserRenamed(QStringList const & data);
    void handleUserDisconnected(QStringList const & data);
    void handleWrongPassword(QStringList const & data);
    void handleChannelCreated(QStringList const & data);
    void handleChannelRenamed(QStringList const & data);
    void handleChannelDeleted(QStringList const & data);
    void handleRequestDenied(QStringList const & data);
    void handleEndOfTransmission(QStringList const & data);
};

#endif // TCPCLIENT_H
