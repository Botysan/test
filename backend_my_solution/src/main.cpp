#include <QCoreApplication>
#include <QThread>
#include <QCommandLineParser>

#include "IOManager.h"
#include "scanner.h"

// Test Project specification

// Modify current application:
// The monitored directory shall be set optionally by argument.
// The initial filter shall be set by arguments (like *.txt|*.bmp), the filter shall be possible to change at runtime from console.
// Monitor also change of file(s)  content. (print info that content of file was changed).
// Optional:
// Monitor by similar way also content of subdirectories
// Enable/disable subdirectory monitoring can be enabled/disabled by argument

//Notes:
// The code shall be platform independent
// Do not break code style
// Why the application exiting with non-zero code?
//

int main(int nArgc, char *p_arrArgv[])
{
    QCoreApplication App(nArgc, p_arrArgv);

    QCommandLineParser parser; //argument parser
    parser.setApplicationDescription("File system scanner program");
    parser.addHelpOption();

    QCommandLineOption targetInitializeOption(QStringList() << "d" << "controled directory",//set -d key to argument
                                             QCoreApplication::translate("main", "Path controled directory."),
                                             QCoreApplication::translate("main", "directory"));
    parser.addOption(targetInitializeOption);
    parser.process(App);

    QString controlledDir;
    if(parser.isSet(targetInitializeOption))
        controlledDir = parser.value(targetInitializeOption);
    else
        controlledDir = QDir::currentPath();

    IOManager ConsoleManager;

    QThread ScannerThread;
    Scanner FileSystemScanner;
    FileSystemScanner.moveToThread(&ScannerThread);
    ScannerThread.start();

    QObject::connect(&ConsoleManager, &IOManager::InitializeEnd, &FileSystemScanner, &Scanner::Initialize);
    QObject::connect(&ConsoleManager, &IOManager::NewFilter, &FileSystemScanner, &Scanner::OnNewFilter);

    QObject::connect(&FileSystemScanner, &Scanner::Change, &ConsoleManager, &IOManager::OnChange);

    ConsoleManager.Initialize(controlledDir);

    return App.exec();
}
