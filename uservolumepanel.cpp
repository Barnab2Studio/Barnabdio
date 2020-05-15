#include "UserVolumePanel.h"

#include "VolumeSlideBar.h"

UserVolumePanel::UserVolumePanel(QWidget * parent)
    : QHBoxLayout(parent)
{
}

void UserVolumePanel::addVolumeSlideBar(VolumeSlideBar * volumeSlideBar)
{
    this->addLayout(volumeSlideBar);
    m_slideBarsId.push_back(volumeSlideBar->id());
}

void UserVolumePanel::removeVolumeSlideBar(int id)
{
    auto it = std::find(m_slideBarsId.begin(), m_slideBarsId.end(), id);
    if (it != m_slideBarsId.end())
    {
        delete this->takeAt(it - m_slideBarsId.begin());

        m_slideBarsId.erase(it);
    }
}
