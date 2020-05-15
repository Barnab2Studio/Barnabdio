#include "volumelayout.h"

#include <QLabel>
#include <QSlider>
#include <QScrollArea>

#include <cmath>
#include <iostream>
using namespace std;
VolumeLayout::VolumeLayout(QString const & name, int volume, QWidget * parent)
    : QVBoxLayout(parent)
    , m_name(name)
    , m_userVolumeLabel(new QLabel())
    , m_volumeSlider(new QSlider())
    , m_volumeValueLabel(new QLabel())
    , m_scrollArea(new QScrollArea())
{
    QObject::connect(m_volumeSlider,        SIGNAL(valueChanged(int)),
                     m_volumeValueLabel,    SLOT(setNum(int)));
    QObject::connect(m_volumeSlider,        SIGNAL(valueChanged(int)),
                     this,                  SLOT(onVolumeChanged(int)));

    initLayout();
    initNameLabel(name);
    initValueLabel(volume);
    initScrollArea();
    initSlider(volume);
}

VolumeLayout::~VolumeLayout()
{
    delete m_volumeValueLabel;
    delete m_volumeSlider;
    delete m_userVolumeLabel;
    delete m_scrollArea;
}

QString const & VolumeLayout::name() const
{
    return m_name;
}

void VolumeLayout::setName(QString const & name)
{
    m_name = name;
}

void VolumeLayout::initValueLabel(int volume)
{
    m_volumeValueLabel->setText(QString::number(volume));
}

void VolumeLayout::initNameLabel(QString const & name)
{
    m_userVolumeLabel->setText(name);
    m_userVolumeLabel->setAlignment(Qt::AlignCenter);
}

void VolumeLayout::initScrollArea()
{
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFixedSize(QSize(75,25));

    m_scrollArea->setWidget(m_userVolumeLabel);
    m_scrollArea->setAlignment(Qt::AlignCenter);

    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void VolumeLayout::initSlider(int volume)
{
    m_volumeSlider->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    m_volumeSlider->setMaximum(100);
    m_volumeSlider->setMinimum(0);

    m_volumeSlider->setValue(volume);
}

void VolumeLayout::initLayout()
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

void VolumeLayout::onNameChanged(QString const & name)
{
    m_userVolumeLabel->setText(name);
}

void VolumeLayout::onVolumeChanged(int volume)
{
    emit volumeChanged(volume);
}

