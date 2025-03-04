#include "volumelistlayout.h"

#include "globals.h"

#include "volumelayout.h"
#include "channel.h"
#include "user.h"

#include <QLayoutItem>
#include <QDebug>

VolumeListLayout::VolumeListLayout(QWidget * parent)
    : QHBoxLayout(parent)
    , m_channel(nullptr)
{
    setAlignment(Qt::AlignLeft);
}

VolumeListLayout::~VolumeListLayout()
{
}

void VolumeListLayout::setChannel(Channel * channel)
{
    if (m_channel != nullptr)
        disconnect(m_channel, nullptr, this, nullptr);

    m_channel = channel;

    if (channel == nullptr)
        return;

    connect(channel, SIGNAL(userAdded(User *)),
            this,    SLOT(add(User *)));

    connect(channel, SIGNAL(userRemoved(int)),
            this,    SLOT(remove(int)));

    update();
}

void VolumeListLayout::rename()
{
    User * user = static_cast<User *>(QObject::sender());

    if (m_volumeLayouts.count(user->id()) == 0)
    {
        disconnect(user, nullptr, this, nullptr);
        return;
    }

    m_volumeLayouts[user->id()]->setName(user->name());
}

void VolumeListLayout::add(User * user)
{
    if (m_volumeLayouts.count(user->id()) != 0)
        return;

    if (user->id() == ClientID)
        return;

    VolumeLayout * volumeLayout = new VolumeLayout(user->name(), user->volume());
    this->addLayout(volumeLayout);
    m_volumeLayouts[user->id()] = volumeLayout;

    connect(volumeLayout, SIGNAL(volumeChanged(int)), user, SLOT(setVolume(int)));
    connect(user, SIGNAL(renamed()), this, SLOT(rename()));
}

void VolumeListLayout::remove(int id)
{
    if (m_volumeLayouts.count(id) == 0)
        return;

    delete m_volumeLayouts[id];
    m_volumeLayouts.erase(id);
}

void VolumeListLayout::update()
{
    if (m_channel == nullptr)
        return;

    clear();
    for (int i = 0 ; i < m_channel->itemCount() ; ++i)
    {
        User * user = qobject_cast<User *>(m_channel->itemAt(i));

        connect(user, SIGNAL(renamed()), this, SLOT(rename()));

        add(user);
    }
}

void VolumeListLayout::clear()
{
    QLayoutItem * child;
    while ((child = takeAt(0)) != 0)
        delete child;
    m_volumeLayouts.clear();
}
