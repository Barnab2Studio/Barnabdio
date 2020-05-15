#ifndef USERLIST_H
#define USERLIST_H

#include <vector>

#include <QObject>

class User;
class UserList : public QObject
{
    Q_OBJECT

public:
    UserList();
    virtual ~UserList() {}

public slots: // public for testing, should be private later on
    User * addUser(int id, QString const & name);
    void removeUser(int id);

signals:
    void userConnected(User *);
    void userDisconnected(User *);

private:
    std::vector<User *> m_users;
};

#endif // UserList_H
