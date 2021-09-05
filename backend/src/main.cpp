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

public slots:
    //! must be called before a destruction
    void Terminate();

signals:
    //! it is emitted when a change in the scanned directory is found
    void Change(QString strChange);
};

class Reporter : public QObject
{
    Q_OBJECT

public:
    Reporter(QObject *pScanner);

private slots:
    //! it is called when a change if found
    void OnChange(QString strChange);
};

int main(int nArgc, char *p_arrArgv[])
{
    QCoreApplication App(nArgc, p_arrArgv);

    Scanner FileSystemScanner(QDir::currentPath());
    Reporter ConsoleReporter(&FileSystemScanner);

    QObject::connect(&App, &QCoreApplication::aboutToQuit, &FileSystemScanner, &Scanner::Terminate);

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
        QSet<QString> arrFiles = QDir(m_strPath).entryList({"*"}).toSet();

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

        QThread::sleep(1);
    }
}

void Scanner::Terminate()
{
    m_bRunning = false;
}

Reporter::Reporter(QObject *pScanner)
{
    connect(pScanner, SIGNAL(Change(QString)), this, SLOT(OnChange(QString)));
}

void Reporter::OnChange(QString strChange)
{
    qInfo() << strChange;
}

#include "main.moc"
