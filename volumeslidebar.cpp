#include "VolumeSlideBar.h"

#include <QLabel>
#include <QSlider>
#include <QScrollArea>

VolumeSlideBar::VolumeSlideBar(int id, QString const & name, QWidget * parent)
    : QVBoxLayout(parent)
    , m_userVolumeLabel(new QLabel())
    , m_volumeSlider(new QSlider())
    , m_volumeValueLabel(new QLabel())
    , m_scrollArea(new QScrollArea())
    , m_id(id)
{
    QObject::connect(m_volumeSlider,        SIGNAL(valueChanged(int)),
                     m_volumeValueLabel,    SLOT(setNum(int)));
    QObject::connect(m_volumeSlider,        SIGNAL(valueChanged(int)),
                     this,                  SLOT(onVolumeChanged(int)));

    initLayout();
    initLabel(name);
    initScrollArea();
    initSlider();
}

VolumeSlideBar::~VolumeSlideBar()
{
    delete m_volumeValueLabel;
    delete m_volumeSlider;
    delete m_userVolumeLabel;
    delete m_scrollArea;
}

void VolumeSlideBar::initLabel(QString const & name)
{
    m_userVolumeLabel->setText(name);
    m_userVolumeLabel->setAlignment(Qt::AlignCenter);
}

void VolumeSlideBar::initScrollArea()
{
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFixedSize(QSize(50,25));

    m_scrollArea->setWidget(m_userVolumeLabel);
    m_scrollArea->setAlignment(Qt::AlignCenter);

    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}
#include <QPalette>
void VolumeSlideBar::initSlider()
{
    m_volumeSlider->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    m_volumeSlider->setMaximum(100);
    m_volumeSlider->setMinimum(0);
    m_volumeSlider->setValue(100);
}

void VolumeSlideBar::initLayout()
{
    this->addWidget(m_scrollArea);
    this->addWidget(m_volumeSlider);
    this->addWidget(m_volumeValueLabel);

    this->setAlignment(m_userVolumeLabel, Qt::AlignHCenter);
    this->setAlignment(m_volumeValueLabel, Qt::AlignHCenter);

    this->setContentsMargins(0,0,10,0);

    this->setStretch(0,1);
    this->setStretch(1,9);
    this->setStretch(2,1);

    this->setSpacing(0);
}

void VolumeSlideBar::onNameChanged(QString const & name)
{
    m_userVolumeLabel->setText(name);
}

void VolumeSlideBar::onVolumeChanged(int volume)
{
    emit volumeChanged(volume);
}
