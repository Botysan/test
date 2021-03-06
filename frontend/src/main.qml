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
  \file   main.qml
  \author kubins
  \date   17.5.2019
  \brief
*/

import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.5

Window {
    id: root

    visible: true
    width: 660
    height: 640
    title: qsTr("TestQml")

    // maximum possible smiles is 4^2 = 16
    property int smilesCount: 4
    property int smileSize: Math.min(width / smilesCount, height / smilesCount)

    GridView {
        anchors.fill: parent
        anchors.leftMargin: root.smileSize * 0.05
        anchors.topMargin: anchors.leftMargin

        cellWidth: root.smileSize
        cellHeight: cellWidth

        model: smiles
        delegate: Smile {
            width: root.smileSize * 0.9
            height: width

            angle: model.angle
            color: model.color
            mouth: model.mouth
        }
    }

    Button {
        id: buttonSpeedUp
        x: 0
        y: parent.height * 0.9

        text: "+"

        width: parent.width / 2
        height: parent.height * 0.1

        onClicked: {
            for (var i = 0; i < smiles.length; i++)
                smiles[i].SetSpeedUp();
        }
    }
    Button {
        id: buttonSpeedDown
        x: buttonSpeedUp.x + buttonSpeedUp.width
        y: buttonSpeedUp.y


        text: "-"

        width: buttonSpeedUp.width
        height: buttonSpeedUp.height

        onClicked: {
            for (var i = 0; i < smiles.length; i++)
                smiles[i].SetSpeedDown();
        }
    }
}
