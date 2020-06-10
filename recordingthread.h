#ifndef RECORDINGTHREAD_H
#define RECORDINGTHREAD_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
class QAudioInput;
class QUdpSocket;
class RecordingThread : public QThread
{
    Q_OBJECT
public:
    RecordingThread(QObject *parent, char *buffer, QWaitCondition *bufferNotEmpty, QWaitCondition *bufferNotFull, QMutex *mutex, int *numBytesInBuffer );
    RecordingThread(QObject *parent,QUdpSocket *socket);
    ~RecordingThread();
    QAudioInput* getInputDevice();

signals:
    void sendAudioInputData(const int a);

//public slots:
//    void stopProcess();

protected:
    void run();

private:
    bool m_abort;
    char* m_buffer;
    QAudioInput *m_audioInput = nullptr;
    QMutex *m_mutex;
    QWaitCondition *m_notFull;
    QWaitCondition *m_notEmpty;
    int *m_numBytesInBuffer;
    QUdpSocket *m_socket;
};

#endif // RECORDINGTHREAD_H
