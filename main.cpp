#include "MainWindow.h"

#include <QApplication>

#include "DarkStyle.h"
#include "framelesswindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setStyle(new DarkStyle);

    // create frameless window (and set windowState or title)
    FramelessWindow framelessWindow;

      framelessWindow.setWindowTitle("Barnabio");
      framelessWindow.setWindowIcon(a.style()->standardIcon(QStyle::SP_DesktopIcon));

      // create our mainwindow instance
      MainWindow * mainWindow = new MainWindow(&framelessWindow);

      // add the mainwindow to our custom frameless window
      framelessWindow.setContent(mainWindow);
      framelessWindow.show();


    return a.exec();
}
