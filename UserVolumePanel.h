#ifndef USERVOLUMEPANEL_H
#define USERVOLUMEPANEL_H

#include <vector>

#include <QLayout>

class VolumeSlideBar;
class UserVolumePanel : public QHBoxLayout
{
    Q_OBJECT
public:
    UserVolumePanel(QWidget * parent = nullptr);
    virtual ~UserVolumePanel() {}

private slots:
    void addVolumeSlideBar(VolumeSlideBar * volumeSlideBar);
    void removeVolumeSlideBar(int id);

private:
    std::vector<int> m_slideBarsId;
};

#endif // USERVOLUMEPANEL_H
