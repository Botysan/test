#ifndef SCANNER_H
#define SCANNER_H

#include <QObject>
#include <QTimer>
#include <QSet>
#include <QDir>

class Scanner : public QObject
{
    Q_OBJECT
public:
    explicit Scanner(QObject *parent = nullptr);

public slots:
    void Initialize(QString strPath);
    void OnNewFilter(QString strFilter);

signals:
    void Change(QString strChange);

private slots:
    void CheckDir();

private:
    QTimer *m_checkTimer;

    QSet<QString> m_arrLastFiles;
    QString m_strPath;
    QStringList m_strFilterList = {"*"};
};

#endif // SCANNER_H
