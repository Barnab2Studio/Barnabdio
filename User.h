#ifndef USER_H
#define USER_H

#include <QObject>

#include "channellistitem.h"

class Channel;
class User : public ChannelListItem
{
    Q_OBJECT

public:
    enum InputState
    {
        INPUT_DEACTIVATED,
        INPUT_MUTED,
        INPUT_IDLE,
        INPUT_ACTIVE
    };

    enum OutputState
    {
        OUTPUT_DEACTIVATED,
        OUTPUT_MUTED,
        OUTPUT_READY
    };

    explicit User(int id, QString const & name, ChannelListItem * parent = nullptr);
    virtual ~User();

    bool setChannel(Channel * channel);

    int volume() const;

    ChannelListItem * itemAt(int index) override;
    int itemCount() const override;

private slots:
    void setVolume(int volume);

signals:
    void channelChanged(Channel *);

private:
    int         m_volume;
    InputState  m_inputState;
    OutputState m_outputState;
};

#endif // USER_H
