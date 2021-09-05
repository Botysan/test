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
  \date   10.5.2019
  \brief
*/

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTimer>

// Add smile mouth to graphics
// The mouth shall be settable (a smile/frown) at Smile instance creation time
// Add buttons to graphics for increase and decrease the rotation speed of all Smiles
// Change the random smile color on Smile click (any Smile click change random Smile color to black)
// The code shall be platform and resolution independent
// Do not break code style

class Smile : public QObject
{
    Q_OBJECT

    Q_PROPERTY(qint32 angle MEMBER m_nAngle NOTIFY angleChanged)
    Q_PROPERTY(QString color MEMBER m_strColor NOTIFY colorChanged)
    Q_PROPERTY(bool mouth MEMBER m_bMouthSmile NOTIFY mouthChanged)

public:
    Smile(QString strColor, qint32 nRotationSpeed, bool bRotationDirection, bool bMouth) : QObject(), m_strColor(strColor), m_bMouthSmile(bMouth)
    {
        connect(&m_RotationTimer, &QTimer::timeout, [this, bRotationDirection]() {
            m_nAngle += bRotationDirection ? 2 : -2;
            if (m_nAngle < 0)
            {
                m_nAngle = 360;
            }
            if (m_nAngle > 360)
            {
                m_nAngle = 0;
            }
            emit angleChanged(m_nAngle);
        });
        m_RotationTimer.start(nRotationSpeed);
        emit mouthChanged(m_bMouthSmile);
    }

private:
    qint32 m_nAngle = 0;
    QString m_strColor;

    QTimer m_RotationTimer;

    bool m_bMouthSmile;//we can use emun/or class mouth in this place
    //if we use emun/or class, we can easily add or modify the mouth without changing the smile class

signals:
    void angleChanged(qint32 angle);
    void colorChanged(QString color);
    void mouthChanged(bool mouth);
};

int main(int nArgc, char *p_arrArgv[])
{
    QGuiApplication App(nArgc, p_arrArgv);

    auto GetRandomSpeed = []() {
        return (qrand() % 50) + 1;
    };

    auto GetRandomDirection = []() {
        return (qrand() % 2) ? true : false;
    };

    auto GetRandomMouth = []() {
        return (qrand() % 2) ? true : false;
    };

    QList<QObject*> arrSmiles;
    arrSmiles.append(new Smile("#6b8e23", /*GetRandomSpeed()*/1000000, GetRandomDirection(), GetRandomMouth()));
    arrSmiles.append(new Smile("#ffa500", GetRandomSpeed(), GetRandomDirection(), GetRandomMouth()));
    arrSmiles.append(new Smile("#ff4500", GetRandomSpeed(), GetRandomDirection(), GetRandomMouth()));
    arrSmiles.append(new Smile("#da70d6", GetRandomSpeed(), GetRandomDirection(), GetRandomMouth()));
    arrSmiles.append(new Smile("#eee8aa", GetRandomSpeed(), GetRandomDirection(), GetRandomMouth()));
    arrSmiles.append(new Smile("#98fb98", GetRandomSpeed(), GetRandomDirection(), GetRandomMouth()));
    arrSmiles.append(new Smile("#afeeee", GetRandomSpeed(), GetRandomDirection(), GetRandomMouth()));
    arrSmiles.append(new Smile("#db7093", GetRandomSpeed(), GetRandomDirection(), GetRandomMouth()));
    arrSmiles.append(new Smile("#ffefd5", GetRandomSpeed(), GetRandomDirection(), GetRandomMouth()));
    arrSmiles.append(new Smile("#ffdab9", GetRandomSpeed(), GetRandomDirection(), GetRandomMouth()));
    arrSmiles.append(new Smile("#cd853f", GetRandomSpeed(), GetRandomDirection(), GetRandomMouth()));
    arrSmiles.append(new Smile("#ffc0cb", GetRandomSpeed(), GetRandomDirection(), GetRandomMouth()));

    QQmlApplicationEngine Engine;
    QQmlContext *pContext = Engine.rootContext();
    if (pContext)
    {
        pContext->setContextProperty("smiles", QVariant::fromValue(arrSmiles));
    }
    Engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (Engine.rootObjects().isEmpty())
    {
        return -1;
    }

    return App.exec();
}

#include "main.moc"
