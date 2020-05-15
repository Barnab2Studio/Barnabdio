#ifndef CHANNELLISTCONTROLLER_H
#define CHANNELLISTCONTROLLER_H

#include <QObject>

class User;
class Channel;
class ChannelList;
class ChannelListPanel;
class ChannelListController : public QObject
{
    Q_OBJECT
public:
    explicit ChannelListController(ChannelList * channelList, ChannelListPanel * channelListPanel, QObject * parent = nullptr);

public slots:
    void onChannelCreated(Channel * channel);
    void onChannelDeleted(Channel * channel);

    void onUserAdded(Channel * channel, User * user);
    void onUserRemoved(Channel * channel, User * user);

signals:
    void channelCreated(int, QString const &);
    void channelDeleted(int);

    void userAdded(int, int, QString const &);
    void userRemoved(int, int);

private:
    ChannelList * m_channelList;
    ChannelListPanel * m_channelListPanel;
};

#endif // CHANNELLISTCONTROLLER_H
