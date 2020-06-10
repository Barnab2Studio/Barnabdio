#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class User;
class TCPClient;
class QModelIndex;
class ConnectionDialog;
class ChannelListModel;
class VolumeListLayout;
class QAudioInput;

class QUdpSocket;

class ChannelCreationDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void treeView_onDoubleClick(const QModelIndex & index);
    void chatInput_onReturnPressed();

private slots:
    void on_actionConnect_triggered();

    void expandClientChannel(int idChannel, int idUser);
    void displayMenu(const QPoint & pos);
    void finishConnectionInit();
    void cleanup();

    void on_actionCreateChannel_triggered();
    void on_actionRenameChannel_triggered();
    void on_actionDeleteChannel_triggered();

private:
    void initTreeView();
    void initConnectionDialog();
    void initChannelCreationDialog();

    Ui::MainWindow * ui;
    ChannelListModel * m_channelList;
    VolumeListLayout * m_volumeListLayout;
    ConnectionDialog * m_connectionDialog;
    ChannelCreationDialog * m_channelCreationDialog;
    QMenu * m_treeViewMenu;
    QMenu * m_userMenu;
    QMenu * m_clientMenu;
    QMenu * m_channelMenu;
    QModelIndex m_previousActionIndex;
    TCPClient * m_tcpclient;


    QUdpSocket *m_udpSocket = nullptr;
    QAudioInput *m_audioInput = nullptr;

    User * m_client;

};
#endif // MAINWINDOW_H
