/***************************************************************************
**
** Copyright (C) 2016 Medical Technologies CZ a.s.. All rights reserved.
** Contact: http://www.medictech.com
**
** This file is a part of 088 project, projects based on 088
** or related utilities.
**
** Without prior written permission from Medical Technologies CZ a.s.,
** the file must not be modified, distributed and/or otherwise used.
**
***************************************************************************/

/*!
  \file   main.cpp
  \author kubins
  \date   15.4.2019
  \brief  it is a console application which periodically monitors working folder and reports any change
*/

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QThread>

#include <QTextStream>
#include <QTimer>
#include <QCommandLineParser>

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

/*
 * I think this is not the best way to use QThread,
 * in my opinion it is much better to inherit from QObject
 * and execute the moveToThread method, it has its own specifics
 * but it is more thread safe
 */
class Scanner : public QThread
{
    Q_OBJECT

public:
    Scanner(QString strPath);

protected:
    virtual void run() override;

private:
    bool m_bRunning = true;         //!< when terminating, it is set to false

    QString m_strPath;              //!< a directory which is scanned for changes
    QSet<QString> m_arrLastFiles;   //!< the last directory content

    QStringList m_strFilterList = {"*"};

public slots:
    //! must be called before a destruction
    void Terminate();
    void OnNewFilter(QStringList strFilterList);

signals:
    //! it is emitted when a change in the scanned directory is found
    void Change(QString strChange);
};

class Reporter : public QObject
{
    Q_OBJECT
signals:
    void newFilter(QStringList filter);

public:
    Reporter(QObject *pScanner);

private slots:
    //! it is called when a change if found
    void OnChange(QString strChange);
private:
    QTimer m_readTimer;
};

int main(int nArgc, char *p_arrArgv[])
{
    QCoreApplication App(nArgc, p_arrArgv);

    QCommandLineParser parser; //add argument parser
    parser.setApplicationDescription("File system scanner program");
    parser.addHelpOption();//help

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

    Scanner FileSystemScanner(controlledDir);
    Reporter ConsoleReporter(&FileSystemScanner);

    QObject::connect(&App, &QCoreApplication::aboutToQuit, &FileSystemScanner, &Scanner::Terminate);

    //the program ends with a non-zero code because
    //at the time of exit the created thread is not deleted,
    //in particular it may be the situation when this thread
    //is in the sleep method
    //the next windows event

    return App.exec();
}

Scanner::Scanner(QString strPath) : m_strPath(strPath)
{
    start();
}

void Scanner::run()
{
    qDebug() << "thread" << QThread::thread()->currentThreadId() << "started";

    while (m_bRunning)
    {
        QSet<QString> arrFiles = QDir(m_strPath).entryList(m_strFilterList).toSet();

        // report all changes and store the current state
        for (const QString &strFile : (arrFiles - m_arrLastFiles))
        {
            emit Change(QString(tr("File %1 was added")).arg(strFile));
        }
        for (const QString &strFile : (m_arrLastFiles - arrFiles))
        {
            emit Change(QString(tr("File %1 was removed")).arg(strFile));
        }
        m_arrLastFiles = arrFiles;

        QThread::sleep(1);//bad idea becouse thread not released, it "works" all the time
    }
}

void Scanner::Terminate()
{
    m_bRunning = false;
}

void Scanner::OnNewFilter(QStringList strFilterList)
{
    m_strFilterList = strFilterList;
}

Reporter::Reporter(QObject *pScanner)
{
    connect(pScanner, SIGNAL(Change(QString)), this, SLOT(OnChange(QString)));
    connect(this, SIGNAL(newFilter(QStringList)), pScanner, SLOT(OnNewFilter(QStringList)));

    connect(&m_readTimer, &QTimer::timeout, [=](){
        QTextStream s(stdin);
        QString value = s.readLine();
        if(!value.isEmpty())
            emit newFilter(value.split(QLatin1Char('|'), Qt::SkipEmptyParts));
    });
    m_readTimer.setInterval(100);//each 100ms, read console for filter
    m_readTimer.setSingleShot(false);
    m_readTimer.start();
}

void Reporter::OnChange(QString strChange)
{
    qInfo() << strChange;
}

#include "main.moc"
