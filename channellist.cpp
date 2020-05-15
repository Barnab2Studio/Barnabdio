#include "ChannelList.h"

#include "Channel.h"

ChannelList::ChannelList(QObject * parent) : QObject(parent)
{
}

Channel * ChannelList::addChannel(int id, QString const & name)
{
    Channel * channel = new Channel(id, name);

    m_channels.push_back(channel);
    emit channelCreated(channel);
    return channel;
}

void ChannelList::removeChannel(int id)
{
    auto it = std::find_if(m_channels.begin(), m_channels.end(), [id](Channel * channel) { return channel->id() == id; });

    if (it != m_channels.end())
    {
        Channel * channel = m_channels[it - m_channels.begin()];

        if (channel->hasUsers())
            return;

        emit channelDeleted(channel);
        m_channels.erase(it);
        delete channel;
    }
}
