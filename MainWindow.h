#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class User;
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

private:
    Ui::MainWindow * ui;
    ChannelListModel * model;
    VolumeListLayout * m_volumeListLayout;

    User * m_client;
};
#endif // MAINWINDOW_H
