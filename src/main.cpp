#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <stdlib.h>

#include "terminal_widget.h"

int main(int argc, char *argv[])
{
    setenv("QT_QPA_PLATFORMTHEME", "gtk3", 1);
    QApplication a(argc, argv);

    QMainWindow w;
    TerminalWidget *terminal = new TerminalWidget(&w);
    w.setCentralWidget(terminal);

    QMenu *fileMenu = w.menuBar()->addMenu("File");
    QAction *openFontAction = new QAction("Open Font", &w);
    fileMenu->addAction(openFontAction);

    QObject::connect(openFontAction, &QAction::triggered, terminal, &TerminalWidget::openFontDialog);

    w.show();
    return a.exec();
}
