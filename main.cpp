#include <choose.h>

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Choose choose;
    choose.show();
    return a.exec();
}
