#include <QtMultimedia/QAudioDeviceInfo>
#include <QtMultimedia/QAudioOutput>
#include <QDebug>
#include <QUdpSocket>
#include "playingSoundThread.h"

PlayingSoundThread::PlayingSoundThread(QObject *parent, char *buffer, QWaitCondition *bufferNotEmpty, QWaitCondition *bufferNotFull, QMutex *mutex, int *numBytesInBuffer )
    : QThread(parent),
      m_buffer(buffer),
      m_mutex(mutex),
      m_notFull(bufferNotFull),
      m_notEmpty(bufferNotEmpty),
      m_numBytesInBuffer(numBytesInBuffer)
{
    m_abort = false;

    const QAudioDeviceInfo outputDeviceInfo = QAudioDeviceInfo::defaultOutputDevice();

    QAudioFormat formatAudio;
    formatAudio.setSampleRate(8000);
    formatAudio.setChannelCount(1);
    formatAudio.setSampleSize(16);
    formatAudio.setCodec("audio/pcm");
    formatAudio.setByteOrder(QAudioFormat::LittleEndian);
    formatAudio.setSampleType(QAudioFormat::SignedInt);

    m_audioOutput = new QAudioOutput(outputDeviceInfo, formatAudio, this);
}
PlayingSoundThread::PlayingSoundThread(QObject *parent, QUdpSocket* socket) :QThread(parent),m_socket(socket){
    const QAudioDeviceInfo outputDeviceInfo = QAudioDeviceInfo::defaultOutputDevice();

    QAudioFormat formatAudio;
    formatAudio.setSampleRate(8000);
    formatAudio.setChannelCount(1);
    formatAudio.setSampleSize(16);
    formatAudio.setCodec("audio/pcm");
    formatAudio.setByteOrder(QAudioFormat::LittleEndian);
    formatAudio.setSampleType(QAudioFormat::SignedInt);

    m_audioOutput = new QAudioOutput(outputDeviceInfo, formatAudio, this);
    m_device = m_audioOutput->start();
    connect(socket, SIGNAL(readyRead()), this, SLOT(playData()));

}

void PlayingSoundThread::playData()
{
    //You need to read datagrams from the udp socket
    while (m_socket->hasPendingDatagrams())
    {
     QByteArray data;
     data.resize(m_socket->pendingDatagramSize());
     m_socket->readDatagram(data.data(), data.size());
     m_device->write(data.data(), data.size());
    }
}


PlayingSoundThread::~PlayingSoundThread()
{
    qDebug() <<"Playing thread started";
}



void PlayingSoundThread::run()
{

}
