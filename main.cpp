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
    assert(table->getValue(10) == 25);

    table->add(26, 242);
    assert(table->getSize() == 2);
    assert(table->getValue(26) == 242);
    assert(table->getSumValue() == 267);
//    assert(table[10] == 25);
    int i=0;
    Iterator<int, unsigned int> *iter = table->begin();
    for (int i = 0; i < 32; i++, iter++)
    {
        if (i == 1) assert(iter->currentValue() == 10);
        if (i == 18) assert(iter->currentValue() == 242);
    }
    std::cout << table->getValue(26) << std::endl;

    table->remove(10);
//    assert(table->getSumValue() == 242);
    assert(table->contains(10) == false);

    table->remove(26);
    assert(table->getSize() == 0);
    assert(table->getSumValue() == 0);
    assert(table->contains(26) == false);

    delete iter;
    delete table;

    return 0;
//    QApplication a(argc, argv);


//    MainWindow w;
//    w.show();

//    return a.exec();
}
