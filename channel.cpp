#include "channel.h"

#include "globals.h"
#include "user.h"

Channel::Channel(int id, QString const & name, ChannelListItem * parent)
    : ChannelListItem(id, name, parent)
{
    m_flags = Qt::ItemIsDropEnabled;
}

Channel::~Channel()
{
}

void Channel::addUser(User * user)
{
    user->setIcon(QIcon(":/icons/player_commander_off.png")); // a bouger

    if (user->id() == ClientID)
        m_users.push_front(user);
    else
        m_users.append(user);

    emit userAdded(user);
}

void Channel::removeUser(int id)
{
    auto it = std::find_if(m_users.begin(), m_users.end(),
        [id](User * user) { return user->id() == id; });

    if (it != m_users.end())
    {
        m_users.erase(it);
        emit userRemoved(id);
    }
}

User * Channel::getUserFromId(int id)
{
    auto it = std::find_if(m_users.begin(), m_users.end(),
        [id](User * user) { return user->id() == id; });

    if (it != m_users.end())
        return m_users[it - m_users.begin()];
    return nullptr;
}

ChannelListItem * Channel::itemAt(int index)
{
    if (index < 0 || index >= m_users.size())
        return nullptr;
    return m_users.at(index);
}

int Channel::itemCount() const
{
    return m_users.size();
}
