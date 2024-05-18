#include "mainwindow.h"

#include <QApplication>
#include "choosedlg.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ChooseDlg dlg;
    if(dlg.exec()){
        MainWindow w;
        w.show();
        return a.exec();
    }
    return 0;
}
