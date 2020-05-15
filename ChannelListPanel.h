#ifndef CHANNELLISTPANEL_H
#define CHANNELLISTPANEL_H

#include <QObject>
#include <qtreewidget.h>

#include <map>

class QTreeWidgetItem;
class ChannelListPanel : public QTreeWidget
{
    Q_OBJECT

    struct User
    {
        int id;
        QTreeWidgetItem * user;
    };

    struct Channel
    {
        QTreeWidgetItem * channel;
        std::vector<User> users;
    };

public:
    explicit ChannelListPanel(QWidget * parent = nullptr);

private slots:
    void addChannel(int id, QString const & name);
    void removeChannel(int id);

    void addUser(int idChannel, int idUser, QString const & name);
    void removeUser(int idChannel, int idUser);

signals:

private:
    std::map<int, Channel> m_nodes;
};

#endif // CHANNELLISTPANEL_H
