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
    assert(table[10] == 25);

    table.add(26, 242);
    assert(table.contains(26));
    assert(table.getSize() == 2);
    assert(table[26] == 242);
    assert(table.getSumValue() == 267);
    assert(table[10] == 25);

    std::ofstream ofs;
    ofs.open("file.txt");
    ofs << table;
    ofs.close();

    HashTable<int, unsigned int> fileTable;

    std::ifstream ifs;
    ifs.open("file.txt");
    ifs >> fileTable;
    ifs.close();
    assert(table == fileTable);

    HashTable<int, unsigned int> copyTable(table);
    assert(table == copyTable);

    Iterator<int, unsigned int> iter = table.begin();
    assert(iter.currentKey() == 10);
    int first = iter.currentKey();
    iter++;
    int second = iter.currentKey();
    assert(first != second);

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

    for (int i = 0; i < 32; i++)
        table.add(i, 250);

    table.add(70, 40);
    assert(table[70] == 40);

    return 0;
//    QApplication a(argc, argv);


//    MainWindow w;
//    w.show();

//    return a.exec();
}
