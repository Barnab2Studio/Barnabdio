#include "channellistitem.h"

ChannelListItem::ChannelListItem(int id, QString const & name, ChannelListItem * parent)
    : m_id(id)
    , m_name(name)
    , m_parent(parent)
{
}

ChannelListItem::~ChannelListItem()
{
}

int ChannelListItem::id() const
{
    return m_id;
}

QIcon const & ChannelListItem::icon() const
{
    return m_icon;
}

void ChannelListItem::setIcon(QIcon const & icon)
{
    m_icon = icon;
}

QString const & ChannelListItem::name() const
{
    return m_name;
}

void ChannelListItem::rename(QString const & name)
{
    m_name = name;
//    emit itemRenamed(name);
}

ChannelListItem * ChannelListItem::parent()
{
    return m_parent;
}
