#ifndef PLAYINGSOUNDTHREAD_H
#define PLAYINGSOUNDTHREAD_H


#include <QThread>
#include <QMutex>
#include <QWaitCondition>
class QAudioOutput;
class QUdpSocket;
class QIODevice;
class PlayingSoundThread : public QThread
{
    Q_OBJECT
public:
    PlayingSoundThread(QObject *parent, char *buffer, QWaitCondition *bufferNotEmpty, QWaitCondition *bufferNotFull, QMutex *mutex, int *numBytesInBuffer );
    PlayingSoundThread(QObject *parent,QUdpSocket *socket);
    ~PlayingSoundThread();

signals:
    void sendAudioInputData(const int a);

//public slots:
//    void stopProcess();
private slots:
    void playData();

protected:
    void run();

private:
    bool m_abort;
    char* m_buffer;
    QAudioOutput *m_audioOutput = nullptr;
    QMutex *m_mutex;
    QWaitCondition *m_notFull;
    QWaitCondition *m_notEmpty;
    int *m_numBytesInBuffer;
    QUdpSocket *m_socket;
    QIODevice *m_device;
};


#endif // PLAYINGSOUNDTHREAD_H
