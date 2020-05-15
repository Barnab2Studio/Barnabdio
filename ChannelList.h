#ifndef CHANNELLIST_H
#define CHANNELLIST_H

#include <QObject>

class Channel;
class ChannelList : public QObject
{
    Q_OBJECT

public:
    explicit ChannelList(QObject *parent = nullptr);

    Channel * addChannel(int id, QString const & name);
    void removeChannel(int id);

signals:
    void channelCreated(Channel *);
    void channelDeleted(Channel *);

private:
    std::vector<Channel *> m_channels;
};

#endif // CHANNELLIST_H
