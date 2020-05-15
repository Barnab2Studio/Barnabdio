#include "user.h"

#include "channel.h"

#include <QDebug>

User::User(int id, QString const & name, ChannelListItem * parent)
    : ChannelListItem(id, name, parent)
    , m_volume(100)
{
    if (id == 3)
        m_volume = 0;
}

User::~User()
{
}

void User::setChannel(Channel * channel)
{
    if (channel == m_parent)
        return;

    Channel * currentChannel = qobject_cast<Channel *>(m_parent);

    if (currentChannel != nullptr)
        currentChannel->removeUser(m_id);

    m_parent = channel;

    if (channel != nullptr)
        channel->addUser(this);

    emit channelChanged(channel);
}

int User::volume() const
{
    return m_volume;
}

void User::setVolume(int volume)
{
    m_volume = volume;
}

ChannelListItem * User::itemAt(int index)
{
    Q_UNUSED(index);
    return nullptr;
}

int User::itemCount() const
{
    return 0;
}
