#ifndef USERVOLUMECONTROLLER_H
#define USERVOLUMECONTROLLER_H

#include <QObject>

class User;
class UserList;
class UserVolumePanel;
class VolumeSlideBar;
class UserVolumeController : public QObject
{
    Q_OBJECT

public:
    UserVolumeController(UserList * UserList, UserVolumePanel * userVolumePanel);
    virtual ~UserVolumeController() {}

private slots:
    void onUserConnected(User * user);
    void onUserDisconnected(User * user);

signals:
    void slideBarCreated(VolumeSlideBar *);
    void slideBarInvalid(int);

private:
    UserList * m_UserList;
    UserVolumePanel * m_userVolumePanel;
};

#endif // USERVOLUMECONTROLLER_H
