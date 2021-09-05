#include "scanner.h"

#include <QThread>

Scanner::Scanner(QObject *parent) : QObject(parent)
{

}

void Scanner::Initialize(QString strPath)
{
    m_strPath = strPath;

    m_checkTimer = new QTimer(this);

    connect(m_checkTimer, &QTimer::timeout, this, &Scanner::CheckDir);

    m_checkTimer->start(1000);//need to free thread, without sleep
}

void Scanner::OnNewFilter(QString strFilter)
{
    m_strFilterList = strFilter.split(QLatin1Char('|'), Qt::SkipEmptyParts);
}

void Scanner::CheckDir()//we can use QFileSystemWatcher
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
}
