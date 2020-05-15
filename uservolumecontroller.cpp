#include "UserVolumeController.h"

#include "User.h"
#include "UserList.h"
#include "UserVolumePanel.h"
#include "VolumeSlideBar.h"

UserVolumeController::UserVolumeController(UserList * UserList, UserVolumePanel * userVolumePanel)
    : m_UserList(UserList)
    , m_userVolumePanel(userVolumePanel)
{
    QObject::connect(m_UserList,     SIGNAL(userConnected(User *)),
                     this,              SLOT(onUserConnected(User *)));
    QObject::connect(m_UserList,     SIGNAL(userDisconnected(User *)),
                     this,              SLOT(onUserDisconnected(User *)));

    QObject::connect(this,              SIGNAL(slideBarCreated(VolumeSlideBar *)),
                     userVolumePanel,   SLOT(addVolumeSlideBar(VolumeSlideBar *)));
    QObject::connect(this,              SIGNAL(slideBarInvalid(int)),
                     userVolumePanel,   SLOT(removeVolumeSlideBar(int)));
}

void UserVolumeController::onUserConnected(User * user)
{
    VolumeSlideBar * volumeSlideBar = new VolumeSlideBar(user->id(), user->name());

    QObject::connect(volumeSlideBar,    SIGNAL(volumeChanged(int)),
                     user,              SLOT(onVolumeChanged(int)));

    QObject::connect(user,              SIGNAL(nameChanged(QString const &)),
                     volumeSlideBar,    SLOT(onNameChanged(QString const &)));

    emit slideBarCreated(volumeSlideBar);
}

void UserVolumeController::onUserDisconnected(User * user)
{
    emit slideBarInvalid(user->id());
}
