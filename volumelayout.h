#ifndef VOLUMELAYOUT_H
#define VOLUMELAYOUT_H

#include <QLayout>

class QLabel;
class QSlider;
class QScrollArea;
class VolumeLayout : public QVBoxLayout
{
    Q_OBJECT
public:
    VolumeLayout(QString const & name, int volume = 100, QWidget * parent = nullptr);
    virtual ~VolumeLayout();

    void setName(QString const & name);

private slots:
    void onVolumeChanged(int volume);

signals:
    void volumeChanged(int);

private:
    QLabel * m_userVolumeLabel;
    QSlider * m_volumeSlider;
    QLabel * m_volumeValueLabel;
    QScrollArea * m_scrollArea;

    int m_volume;

    void initLayout();
    void initScrollArea();
    void initSlider(int volume);
    void initNameLabel(QString const & name);
    void initValueLabel(int value);
};

#endif // VOLUMELAYOUT_H
