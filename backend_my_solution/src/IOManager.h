#ifndef IOMANAGER_H
#define IOMANAGER_H

#include <QObject>
#include <QTimer>
#include <QTextStream>

class IOManager : public QObject
{
    Q_OBJECT
public:
    explicit IOManager(QObject *parent = nullptr);
    void Initialize(QString strPath, bool bSubDirCheck);

public slots:
    void OnChange(QString strChange);

signals:
    void NewFilter(QString filter);
    void InitializeEnd(QString strPath, bool bSubDirCheck);

private slots:
    void ReadConsoleInput();

private:
    QTimer *m_readTimer;
};

#endif // IOMANAGER_H
