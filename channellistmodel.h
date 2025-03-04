#ifndef CHANNELLISTMODEL_H
#define CHANNELLISTMODEL_H

#include <QAbstractItemModel>

class User;
class Channel;
class ChannelListItem;
class ChannelListModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit ChannelListModel(QObject * parent = nullptr);
    virtual ~ChannelListModel();

    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    Qt::DropActions supportedDropActions() const override;
    QStringList mimeTypes() const override;
    QMimeData * mimeData(const QModelIndexList & indexes) const override;
    bool dropMimeData(const QMimeData * data, Qt::DropAction action,
                      int row, int column, const QModelIndex & parent) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

public slots:
    Channel * addChannel(int id, QString const & name);
    void removeChannel(int id);
    void renameChannel(int id, QString const & name);
    Channel * getChannelFromId(int id) const;

    void addUser(int userId, QString const & userName, int channelId);
    void removeUser(int id);
    void renameUser(int id, QString const & name);
    User * getUserFromId(int id) const;
    void moveUser(int idChannel, int idUser);

    void clear();
    void displayIndexes() const;

signals:
    void channelChangeRequested(int channelId, int userId);
    void userNameChangeRequested(int id, QString const & name);
    void channelNameChangeRequested(int id, QString const & name);

private:
    Channel * m_root;
    QVector<Channel *> m_channelList;
    QMap<int, User *> m_userList;
};
#endif // CHANNELLIST_H
