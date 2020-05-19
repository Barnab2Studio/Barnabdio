#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class User;
class TCPClient;
class ConnectionDialog;
class ChannelListModel;
class VolumeListLayout;
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

private:
    void initTreeView();
    void initConnectionDialog();

    Ui::MainWindow * ui;
    ChannelListModel * m_channelList;
    VolumeListLayout * m_volumeListLayout;
    ConnectionDialog * m_connectionDialog;
    QMenu * m_treeViewMenu;
    QMenu * m_userMenu;
    QMenu * m_clientMenu;
    QMenu * m_channelMenu;
    TCPClient * m_tcpclient;
};
#endif // MAINWINDOW_H
