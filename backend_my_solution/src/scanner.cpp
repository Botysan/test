#include "scanner.h"

#include <QThread>
#include <QDir>
#include <QDirIterator>

Scanner::Scanner(QObject *parent) : QObject(parent)
{

}

void Scanner::Initialize(QString strPath, bool bSubDirCheck)
{
    m_strPath = strPath;

    m_bSubDirCheck = bSubDirCheck;

    m_checkTimer = new QTimer(this);

    connect(m_checkTimer, &QTimer::timeout, this, &Scanner::ScanDir);

    m_checkTimer->start(1000);//need to free thread, without sleep
}

void Scanner::OnNewFilter(QString strFilter)
{
    m_strFilterList = strFilter.split(QLatin1Char('|'), Qt::SkipEmptyParts);
}

void Scanner::ScanDir()//we can use QFileSystemWatcher
{
    QSet<QString> arrFiles = GetFileSet(m_strPath);

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

QSet<QString> Scanner::GetFileSet(QString strDirPath) //we can use recursion instead of QDirIterator
{
    QSet<QString> arrResult;

    if(m_bSubDirCheck)
    {
        QDirIterator iterator(strDirPath, m_strFilterList, QDir::NoFilter, QDirIterator::Subdirectories);
        while (iterator.hasNext())
            arrResult.insert(iterator.next().remove(strDirPath));
    }
    else
        arrResult = QDir(strDirPath).entryList(m_strFilterList).toSet();

    return arrResult;
}
