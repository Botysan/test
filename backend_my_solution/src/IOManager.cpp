#include "IOManager.h"

#include <QDebug>
#include <QTextStream>

IOManager::IOManager(QObject *parent) : QObject(parent)
{    

}

void IOManager::Initialize(QString strPath)//not good solution, but need send a signal with this path to scanner class
{
//    m_consoleSocket = new QSocketNotifier(STDIN_FILENO, QSocketNotifier::Read);

    m_readTimer = new QTimer(this);

    connect(m_readTimer, &QTimer::timeout, this, &IOManager::ReadConsoleInput);

    m_readTimer->start(100);//each 100ms, read console for filter

//    connect(m_consoleSocket, &QSocketNotifier::activated, this, &IOManager::ReadConsoleInput);

    emit InitializeEnd(strPath);//send to scanner class
}

void IOManager::OnChange(QString strChange)
{
    qInfo() << strChange;
}

void IOManager::ReadConsoleInput()
{
    QTextStream qin(stdin);
    QString value = qin.readLine();
    if(!value.isEmpty())
        emit NewFilter(value);
}
