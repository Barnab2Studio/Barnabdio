#include "ChannelListPanel.h"

ChannelListPanel::ChannelListPanel(QWidget * parent) :
    QTreeWidget(parent)
{
    this->setExpandsOnDoubleClick(false);
    this->setUniformRowHeights(true);
    this->setHeaderHidden(true);
    this->setAnimated(true);
    this->setMinimumWidth(250);
}

void ChannelListPanel::addChannel(int id, QString const & name)
{
    QTreeWidgetItem * channel = new QTreeWidgetItem(this);

    channel->setText(0, name);
//    channel->setIcon(0,QIcon(":/icons/channel.png"));
    this->addTopLevelItem(channel);

    m_nodes[id] = ((ChannelListPanel::Channel){channel, {}});
}

void ChannelListPanel::removeChannel(int id)
{
    if (m_nodes.count(id) == 0)
        return;

    for (int i = 0 ; i < this->topLevelItemCount() ; ++i)
    {
        if (this->topLevelItem(i) == m_nodes[id].channel)
        {
            delete this->takeTopLevelItem(i);
            m_nodes.erase(id);
        }
    }
}

void ChannelListPanel::addUser(int idChannel, int idUser, QString const & name)
{
    if (m_nodes.count(idChannel) == 0)
        return;

    QTreeWidgetItem * user = new QTreeWidgetItem(m_nodes[idChannel].channel);

    user->setText(0, name);
    user->setIcon(0, QIcon(":/icons/player_commander_off.png"));

    m_nodes[idChannel].users.push_back(((ChannelListPanel::User){idUser, user}));
    m_nodes[idChannel].channel->addChild(user);
}

void ChannelListPanel::removeUser(int idChannel, int idUser)
{
    if (m_nodes.count(idChannel) == 0)
        return;

    const auto & users = m_nodes[idChannel].users;

    auto it = std::find_if(users.begin(), users.end(), [idUser](ChannelListPanel::User const & user)
        { return user.id == idUser; });

    if (it != users.end())
    {
        m_nodes[idChannel].channel->removeChild(users[it - users.begin()].user);
        m_nodes[idChannel].users.erase(it);
        delete users[it - users.begin()].user;
    }

//    this->itemActivated()
}
