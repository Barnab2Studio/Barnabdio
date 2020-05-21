#ifndef CHANNELLISTITEM_H
#define CHANNELLISTITEM_H

#include <QObject>
#include <QVariant>
#include <QVector>
#include <QIcon>

class ChannelListItem : public QObject
{
    Q_OBJECT

public:
    explicit ChannelListItem(int id, QString const & name, ChannelListItem * parent = nullptr);
    virtual ~ChannelListItem();

    int id() const;

    QString const & name() const;
    void rename(QString const & name);

    QIcon const & icon() const;
    void setIcon(QIcon const & icon);

    virtual ChannelListItem * itemAt(int index) = 0;
    virtual int itemCount() const = 0;

    ChannelListItem * parent();

    Qt::ItemFlags flags();
    void setFlags(Qt::ItemFlags flags);

signals:
    void renamed();

protected:
    const int m_id;
    QString m_name;
    QIcon m_icon;
    ChannelListItem * m_parent;
    Qt::ItemFlags m_flags;
};

#endif // CHANNEL_H
