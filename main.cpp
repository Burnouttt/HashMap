#include "mainwindow.h"
#include <QApplication>

#include "hashtable.h"
#include <iostream>
#include <cassert>
#include <fstream>

int main(int argc, char *argv[])
{
    HashTable<int, unsigned int> table;

    assert(table.getSize() == 0);

    table.add(10, 25);
    assert(table.contains(10));
    assert(table.getSize() == 1);
    assert(table.getSumValue() == 25);
    assert(table.getValue(10) == 25);

    table.add(26, 242);
    assert(table.contains(26));
    assert(table.getSize() == 2);
    assert(table.getValue(26) == 242);
    assert(table.getSumValue() == 267);
    assert(table[10] == 25);

    std::ofstream ofs;
    ofs.open("file.txt");
    ofs << table;
    ofs.close();

    std::ifstream ifs;
    ifs.open("file.txt");
    ifs >> table;
    ifs.close();
    assert(table.getSize() == 2);
    assert(table.getSumValue() == 267);
    assert(table.contains(10));
    assert(table.contains(26));

    HashTable<int, unsigned int> copyTable(table);
    assert(table == copyTable);

    table.remove(10);
    assert(table.getSumValue() == 242);
    assert(table.contains(10) == -1);

    table.remove(26);
    assert(table.getSize() == 0);
    assert(table.getSumValue() == 0);
    assert(table.contains(26) == -1);

    copyTable.add(25, 36);
    assert(table != copyTable);

    copyTable.clear();
    assert(copyTable.contains(25) == -1);
    assert(copyTable.isEmpty());

    return 0;
//    QApplication a(argc, argv);


//    MainWindow w;
//    w.show();

//    return a.exec();
}
