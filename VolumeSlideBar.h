#ifndef VolumeSlideBar_H
#define VolumeSlideBar_H

#include <QLayout>

class QLabel;
class QSlider;
class QScrollArea;
class VolumeSlideBar : public QVBoxLayout
{
    Q_OBJECT
public:
    VolumeSlideBar(int id, QString const & name, QWidget * parent = nullptr);
    virtual ~VolumeSlideBar();

    int id() const { return m_id; }

private slots:
    void onNameChanged(QString const & name);
    void onVolumeChanged(int volume);

signals:
    void volumeChanged(int);

private:
    QLabel * m_userVolumeLabel;
    QSlider * m_volumeSlider;
    QLabel * m_volumeValueLabel;
    QScrollArea * m_scrollArea;

    int m_id;

    void initLayout();
    void initSlider();
    void initScrollArea();
    void initLabel(QString const & name);
};

#endif // VolumeSlideBar_H
