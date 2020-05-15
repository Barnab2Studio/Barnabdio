#include "UserList.h"

#include "User.h"

UserList::UserList()
{
}

User * UserList::addUser(int id, QString const & name)
{
    User * user = new User(id, name);

    m_users.push_back(user);
    emit userConnected(user);
    return user;
}

void UserList::removeUser(int id)
{
    auto it = std::find_if(m_users.begin(), m_users.end(), [id](User * user) { return user->id() == id; });

    if (it != m_users.end())
    {
        User * user = m_users[it - m_users.begin()];
        user->setChannel(nullptr);
        emit userDisconnected(user);
        m_users.erase(it);
    }
}
