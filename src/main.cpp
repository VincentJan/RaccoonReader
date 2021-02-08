/*
**  This file is part of Raccoon Reader.
**
** 	main.cpp: The entry of whole program, cerate a QApplication object and
** 			a MainWindow object which is the main interface of the program;
** 			After that, the control switch to the QApplication object;
**
**  Copyright 2021 Yang XiLong
*/


#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
