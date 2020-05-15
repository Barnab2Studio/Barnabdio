#ifndef CHANNEL_H
#define CHANNEL_H

#include <QVector>

#include "channellistitem.h"

class User;
class Channel : public ChannelListItem
{
    Q_OBJECT

public:
    explicit Channel(int id, QString const & name, ChannelListItem * parent = nullptr);
    virtual ~Channel();

    void addUser(User * user);
    void removeUser(int id);

    User * getUserFromId(int id);

    ChannelListItem * itemAt(int index) override;
    int itemCount() const override;

signals:
    void userAdded(User * user);
    void userRemoved(int id);

private:
    QVector<User *> m_users;
};

#endif // CHANNEL_H
