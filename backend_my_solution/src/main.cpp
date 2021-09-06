#include <QCoreApplication>
#include <QThread>
#include <QCommandLineParser>
#include <QDir>

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

    QCommandLineOption targetDirOption(QStringList() << "d" << "controled directory",//set -d key to argument
                                             QCoreApplication::translate("main", "Path controled directory."),
                                             QCoreApplication::translate("main", "directory"));
    parser.addOption(targetDirOption);

    QCommandLineOption targetSubDirOption(QStringList() << "s" << "controled subdirectory",//set -s key to argument
                                             QCoreApplication::translate("main", "Flag to control subdirectory"));
    parser.addOption(targetSubDirOption);

    parser.process(App);

    QString strControlledDir;
    if(parser.isSet(targetDirOption))
        strControlledDir = parser.value(targetDirOption);
    else
        strControlledDir = QDir::currentPath();

    bool bSubdirectoryControl = parser.isSet(targetSubDirOption);

    IOManager ConsoleManger;

    QThread ScannerThread;
    Scanner FileSystemScanner;
    FileSystemScanner.moveToThread(&ScannerThread);
    ScannerThread.start();

    QObject::connect(&ConsoleManger, &IOManager::InitializeEnd, &FileSystemScanner, &Scanner::Initialize);
    QObject::connect(&FileSystemScanner, &Scanner::Change, &ConsoleManger, &IOManager::OnChange, Qt::DirectConnection);
    //directConnection is used because it is necessary to separate
    //console write and read streams, in this case the slot will
    //in this case the slot will be called in the scanner thread.
    //It should only be used for thread-safe functions in other cases autoconnect is better

    QObject::connect(&ConsoleManger, &IOManager::NewFilter, &FileSystemScanner, &Scanner::OnNewFilter);

    ConsoleManger.Initialize(strControlledDir, bSubdirectoryControl);

    QObject::connect(&App, &QCoreApplication::aboutToQuit, [&](){
        ScannerThread.exit();
    });

    return App.exec();
}
