#include "recordingthread.h"
#include <QtMultimedia/QAudioDeviceInfo>
#include <QtMultimedia/QAudioInput>
#include <QUdpSocket>
#include <QDebug>

RecordingThread::RecordingThread(QObject *parent, char *buffer, QWaitCondition *bufferNotEmpty, QWaitCondition *bufferNotFull, QMutex *mutex, int *numBytesInBuffer)
    : QThread(parent),
      m_buffer(buffer),
      m_mutex(mutex),
      m_notFull(bufferNotFull),
      m_notEmpty(bufferNotEmpty),
      m_numBytesInBuffer(numBytesInBuffer)
{
    m_abort = false;

    const QAudioDeviceInfo inputDeviceInfo = QAudioDeviceInfo::defaultInputDevice();

    QAudioFormat formatAudio;
    formatAudio.setSampleRate(8000);
    formatAudio.setChannelCount(1);
    formatAudio.setSampleSize(16);
    formatAudio.setCodec("audio/pcm");
    formatAudio.setByteOrder(QAudioFormat::LittleEndian);
    formatAudio.setSampleType(QAudioFormat::SignedInt);

    m_audioInput = new QAudioInput(inputDeviceInfo, formatAudio, this);
}

RecordingThread::RecordingThread(QObject *parent, QUdpSocket *socket) : QThread(parent),m_socket(socket){
    const QAudioDeviceInfo inputDeviceInfo = QAudioDeviceInfo::defaultInputDevice();

    QAudioFormat formatAudio;
    formatAudio.setSampleRate(8000);
    formatAudio.setChannelCount(1);
    formatAudio.setSampleSize(16);
    formatAudio.setCodec("audio/pcm");
    formatAudio.setByteOrder(QAudioFormat::LittleEndian);
    formatAudio.setSampleType(QAudioFormat::SignedInt);

    m_audioInput = new QAudioInput(inputDeviceInfo, formatAudio, this);
    m_audioInput->start(m_socket);

}

RecordingThread::~RecordingThread()
{
    qDebug() <<"Recording thread started";
}

QAudioInput* RecordingThread::getInputDevice(){
    return  m_audioInput;
}


void RecordingThread::run()
{

}
