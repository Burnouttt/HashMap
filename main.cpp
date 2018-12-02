#include "mainwindow.h"
#include <QApplication>

#include "hashtable.h"
#include <iostream>
#include <cassert>

int main(int argc, char *argv[])
{
    HashTable<int, unsigned int> *table = new HashTable<int, unsigned int>();

    table->add(10, 25);
    assert(table->getSize() == 1);
    assert(table->getSumValue() == 25);

    table->add(26, 242);
    assert(table->getSize() == 2);
    assert(table->getSumValue() == 267);
    //assert(table[10] == 25);

    table->remove(10);
    assert(table->getSize() == 0);
    assert(table->getSumValue() == 0);
    assert(table->contains(10) == false);

    delete table;

    return 0;
//    QApplication a(argc, argv);


//    MainWindow w;
//    w.show();

//    return a.exec();
}
