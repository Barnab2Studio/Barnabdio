#include "ChannelListController.h"

#include "User.h"
#include "Channel.h"
#include "ChannelList.h"
#include "ChannelListPanel.h"

ChannelListController::ChannelListController(ChannelList * channelList, ChannelListPanel * channelListPanel, QObject * parent)
    : QObject(parent)
    , m_channelList(channelList)
    , m_channelListPanel(channelListPanel)
{
    QObject::connect(m_channelList,         SIGNAL(channelCreated(Channel *)),
                     this,                  SLOT(onChannelCreated(Channel *)));

    QObject::connect(m_channelList,         SIGNAL(channelDeleted(Channel *)),
                     this,                  SLOT(onChannelDeleted(Channel *)));

    QObject::connect(this,                  SIGNAL(channelCreated(int, QString const &)),
                     m_channelListPanel,    SLOT(addChannel(int, QString const &)));

    QObject::connect(this,                  SIGNAL(channelDeleted(int)),
                     m_channelListPanel,    SLOT(removeChannel(int)));

    QObject::connect(this,                  SIGNAL(userAdded(int, int, QString const &)),
                     m_channelListPanel,    SLOT(addUser(int, int, QString const &)));

    QObject::connect(this,                  SIGNAL(userRemoved(int, int)),
                     m_channelListPanel,    SLOT(removeUser(int, int)));
}

void ChannelListController::onChannelCreated(Channel * channel)
{
    QObject::connect(channel, SIGNAL(userAdded(Channel *, User *)),
                     this,    SLOT(onUserAdded(Channel *, User *)));

    QObject::connect(channel, SIGNAL(userRemoved(Channel *, User *)),
                     this,    SLOT(onUserRemoved(Channel *, User *)));

    emit channelCreated(channel->id(), channel->name());
}

void ChannelListController::onChannelDeleted(Channel * channel)
{
    QObject::disconnect(channel, nullptr, this, nullptr);

    emit channelDeleted(channel->id());
}

void ChannelListController::onUserAdded(Channel * channel, User * user)
{
    emit userAdded(channel->id(), user->id(), user->name());
}

void ChannelListController::onUserRemoved(Channel * channel, User * user)
{
    emit userRemoved(channel->id(), user->id());
}
