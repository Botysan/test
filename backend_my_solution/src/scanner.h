#ifndef SCANNER_H
#define SCANNER_H

#include <QObject>
#include <QTimer>
#include <QSet>

class Scanner : public QObject
{
    Q_OBJECT
public:
    explicit Scanner(QObject *parent = nullptr);

public slots:
    void Initialize(QString strPath, bool bSubDirCheck);
    void OnNewFilter(QString strFilter);

signals:
    void Change(QString strChange);

private slots:
    void ScanDir();

private:
    QSet<QString> GetFileSet(QString strDirPath);

    QTimer *m_checkTimer;

    bool m_bSubDirCheck;

    QSet<QString> m_arrLastFiles;
    QString m_strPath;
    QStringList m_strFilterList = {"*"};
};

#endif // SCANNER_H
