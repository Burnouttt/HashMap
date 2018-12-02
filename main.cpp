#include "mainwindow.h"
#include <QApplication>

#include "hashtable.h"
#include <cassert>

int main(int argc, char *argv[])
{
    HashTable<int, unsigned int> *table = new HashTable<int, unsigned int>();

    table->add(10, 25);
    assert(table->getSize() == 1);
    assert(table->getSumValue() == 25);

    delete table;

//    QApplication a(argc, argv);


//    MainWindow w;
//    w.show();

//    return a.exec();
}
