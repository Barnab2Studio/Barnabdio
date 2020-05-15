#ifndef CHANNELLISTMODEL_H
#define CHANNELLISTMODEL_H

#include <QAbstractItemModel>

class Channel;
class ChannelListItem;
class ChannelListModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit ChannelListModel(QObject * parent = nullptr);
    virtual ~ChannelListModel();

    Channel * addChannel(int id, QString const & name);
    void removeChannel(int id);

    void displayIndexes();

    Channel * getChannelFromId(int id) const;

    QVariant data(const QModelIndex &index, int role) const override;
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

private:
    Channel * m_root;
    QVector<Channel *> m_channelList;
};
#endif // CHANNELLIST_H
