#include "channellistmodel.h"

#include "channellistitem.h"
#include "channel.h"
#include "user.h"
#include "globals.h"

#include <QFont>
#include <QtDebug>
#include <QMimeData>

ChannelListModel::ChannelListModel(QObject * parent)
    : QAbstractItemModel(parent)
{
    m_root = new Channel(0, "Header");

}

ChannelListModel::~ChannelListModel()
{
    delete m_root;
    qDeleteAll(m_channelList);
    qDeleteAll(m_userList);
}

Channel * ChannelListModel::addChannel(int id, QString const & name)
{
    Channel * channel = new Channel(id, name, m_root);
    channel->setIcon(QIcon(":/icons/channel.png"));
    m_channelList.append(channel);
    emit layoutChanged();

    return channel;
}

void ChannelListModel::removeChannel(int id)
{
    auto it = std::find_if(m_channelList.begin(), m_channelList.end(),
        [id](Channel * channel) { return channel->id() == id; });

    if (it != m_channelList.end())
    {
        Channel * channel = m_channelList[it - m_channelList.begin()];

        if (channel->itemCount() != 0)
            return;

        m_channelList.erase(it);
        delete channel;
        emit layoutChanged();
    }
}

Channel * ChannelListModel::getChannelFromId(int id) const
{
    auto it = std::find_if(m_channelList.begin(), m_channelList.end(),
        [id](Channel * channel) { return channel->id() == id; });

    if (it != m_channelList.end())
        return m_channelList[it - m_channelList.begin()];
    return nullptr;
}

void ChannelListModel::addUser(int userId, QString const & userName, int channelId)
{
    Channel * channel = getChannelFromId(channelId);

    m_userList[userId] = new User(userId, userName);
    m_userList[userId]->setChannel(channel);
    emit layoutChanged();
}

void ChannelListModel::removeUser(int id)
{
    if (m_userList.count(id) == 0)
        return;

    m_userList[id]->setChannel(nullptr);
    delete m_userList[id];
    emit layoutChanged();
}

void ChannelListModel::renameUser(int id, QString const & name)
{
    if (m_userList.count(id) == 0)
        return;

    m_userList[id]->rename(name);
    emit layoutChanged();
}

User * ChannelListModel::getUserFromId(int id) const
{
    if (m_userList.count(id) == 0)
        return nullptr;
    return m_userList[id];
}

void ChannelListModel::moveUser(int idChannel, int idUser)
{
    User * user = getUserFromId(idUser);
    Channel * channel = getChannelFromId(idChannel);

    if (!user || !channel)
        return;

    user->setChannel(channel);
    emit layoutChanged();
}

void ChannelListModel::clear()
{
    beginResetModel();
    auto itChannel = m_channelList.begin();
    while (itChannel != m_channelList.end())
    {
        delete *itChannel;
        itChannel = m_channelList.erase(itChannel++);
    }

    auto itUser = m_channelList.begin();
    while (itUser != m_channelList.end())
    {
        delete *itUser;
        itUser = m_channelList.erase(itUser++);
    }
    endResetModel();
}

QVariant ChannelListModel::data(QModelIndex const & index, int role) const
{
    if (!index.isValid())
        return QVariant();

    ChannelListItem * item = static_cast<ChannelListItem *>(index.internalPointer());

    if (role == Qt::UserRole)
    {
        if (item->parent() == m_root)
            return item->id();
        return -1;
    }

    if (role == Qt::FontRole)
    {
        QFont font;

        if (item->parent() != m_root && item->id() == ClientID)
            font.setBold(true);

        return font;
    }

    if (role == Qt::EditRole)
        return item->name();

    if (role == Qt::DecorationRole)
        return item->icon();

    if (role == Qt::DisplayRole)
        return item->name();

    return QVariant();
}

bool ChannelListModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if (!index.isValid())
        return false;

    ChannelListItem * item = static_cast<ChannelListItem *>(index.internalPointer());
    if (item->parent() != m_root && value != item->name())
        emit userNameChangeRequested(item->id(), value.toString());
    return true;
}

Qt::ItemFlags ChannelListModel::flags(QModelIndex const & index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    ChannelListItem * item = static_cast<ChannelListItem *>(index.internalPointer());

    /* Root case */
    if (item == m_root)
        return QAbstractItemModel::flags(index);

    /* User case */
    if (item->parent() != m_root)
    {
        Qt::ItemFlags flags = Qt::ItemIsDragEnabled | QAbstractItemModel::flags(index);

        if (item->id() == ClientID)
            flags |= Qt::ItemIsEditable;

        return flags;
    }
    /* Channel case */
    return Qt::ItemIsDropEnabled | QAbstractItemModel::flags(index);
}

Qt::DropActions ChannelListModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

QStringList ChannelListModel::mimeTypes() const
{
    QStringList types;
    types << "application/vnd.userid.list";
    return types;
}

QMimeData *ChannelListModel::mimeData(const QModelIndexList &indexes) const
 {
     QMimeData *mimeData = new QMimeData();
     QByteArray encodedData;

     QDataStream stream(&encodedData, QIODevice::WriteOnly);

     foreach (QModelIndex index, indexes) {
         if (index.isValid()) {
            ChannelListItem * childItem = static_cast<ChannelListItem *>(index.internalPointer());
            ChannelListItem * parentItem = childItem->parent();

            if (parentItem == nullptr || parentItem == m_root)
                continue;

            int32_t ids = ((childItem->id() << 16) | parentItem->id());
            stream << ids;
         }
     }

     mimeData->setData("application/vnd.userid.list", encodedData);
     return mimeData;
 }

bool ChannelListModel::dropMimeData(const QMimeData * data, Qt::DropAction action,
                                    int row, int column, const QModelIndex & parent)
{
    if(!canDropMimeData(data, action, row, column, parent))
        return false;

    Channel * item = static_cast<Channel *>(parent.internalPointer());

    QByteArray encodedData = data->data("application/vnd.userid.list");

    QDataStream stream(&encodedData, QIODevice::ReadOnly);
    QStringList newItems;

    int32_t id;
    stream >> id;

    Channel * parentSource = getChannelFromId(id & 0xffff);
    User * source = parentSource->getUserFromId(id >> 16);

    if (item != source->parent())
        emit channelChangeRequested(item->id(), source->id());

    return true;
}

QVariant ChannelListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    (void)section;
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return m_root->name();

    return QVariant();
}

QModelIndex ChannelListModel::index(int row, int column, QModelIndex const & parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    ChannelListItem * item;

    if (parent.isValid())
    {
        ChannelListItem * parentItem = static_cast<ChannelListItem *>(parent.internalPointer());
        if (parentItem == m_root)
            return QModelIndex();

        item = parentItem->itemAt(row);
    }
    else
    {
        item = m_channelList[row];
    }

    if (item != nullptr)
        return createIndex(row, column, item);
    return QModelIndex();
}

QModelIndex ChannelListModel::parent(QModelIndex const & index) const
{
    if (!index.isValid())
        return QModelIndex();

    ChannelListItem * childItem = static_cast<ChannelListItem *>(index.internalPointer());
    ChannelListItem * parentItem = childItem->parent();

    if (parentItem == nullptr || parentItem == m_root)
        return QModelIndex();

    return createIndex(0, 0, parentItem);
}

int ChannelListModel::rowCount(QModelIndex const & parent) const
{
    if (parent.column() > 0)
        return 0;

    int count;
    if (parent.isValid())
    {
        ChannelListItem * item;
        item = static_cast<ChannelListItem *>(parent.internalPointer());
        if (item == nullptr)
            qDebug() << "tchoiiiin";
        count = item->itemCount();
    }
    else
        count = m_channelList.size();

    return count;
}

int ChannelListModel::columnCount(QModelIndex const & parent) const
{
    Q_UNUSED(parent);
    return 1;
}

void ChannelListModel::displayIndexes() const
{
    for (int i = 0 ; i < rowCount() ; ++i)
    {
        QModelIndex channelIndex = index(i, 0, QModelIndex());
        ChannelListItem * channel = static_cast<ChannelListItem *>(channelIndex.internalPointer());

        qDebug() << "channel index(" << i << ", " << 0 << ") name=" << channel->name();
        for (int j = 0 ; j < rowCount(channelIndex) ; ++j)
        {
            QModelIndex userIndex = index(j, 0, channelIndex);
            ChannelListItem * user = static_cast<ChannelListItem *>(userIndex.internalPointer());

            qDebug() << "    user index(" << j << ", " << 0 << ") name=" << user->name();
        }
    }
    qDebug() << "";
}
