#ifndef VOLUMELISTLAYOUT_H
#define VOLUMELISTLAYOUT_H

#include <QVBoxLayout>

class User;
class Channel;
class VolumeLayout;
class VolumeListLayout : public QHBoxLayout
{
    Q_OBJECT

public:
    explicit VolumeListLayout(QWidget * parent = nullptr);
    virtual ~VolumeListLayout();

public slots:
    void setChannel(Channel * channel);

private slots:
    void rename(User * user);
    void add(User * user);
    void remove(int id);

signals:
    void userVolumeChanged(int, int);

private:
    Channel * m_channel;
    std::map<int, VolumeLayout *> m_volumeLayouts;

    void update();
    void clear();
};

#endif // VOLUMELISTLAYOUT_H
