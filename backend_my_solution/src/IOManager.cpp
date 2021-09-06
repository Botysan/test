#include "IOManager.h"

#include <QDebug>

IOManager::IOManager(QObject *parent) : QObject(parent)
{    

}

void IOManager::Initialize(QString strPath, bool bSubDirCheck)//not good solution, but need send a signal with this path to scanner class
{
    m_readTimer = new QTimer(this);
    connect(m_readTimer, &QTimer::timeout, this, &IOManager::ReadConsoleInput);
    m_readTimer->start(100);//each 100ms, read console for filter

    emit InitializeEnd(strPath, bSubDirCheck);//send to scanner class
}

void IOManager::OnChange(QString strChange)
{
    qInfo() << strChange;
}

void IOManager::ReadConsoleInput()
{
    QTextStream consoleStream(stdin);
    QString value = consoleStream.readLine();//console blocking
    if(!value.isEmpty())
        emit NewFilter(value);
}
