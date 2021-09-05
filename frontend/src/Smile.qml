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
  \file   Smile.qml
  \author kubins
  \date   17.5.2019
  \brief
*/

import QtQuick 2.0

Item {
    id: root

    property alias angle: smile.rotation
    property alias color: smile.color

    Rectangle {
        id: smile

        anchors.fill: parent
        radius: width / 2

        Rectangle {
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: -parent.height / 5
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.horizontalCenterOffset: -parent.width / 5
            width: parent.width * 0.2
            height: width

            radius: width / 2
            color: "white"
        }

        Rectangle {
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: -parent.height / 5
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.horizontalCenterOffset: parent.width / 5
            width: parent.width * 0.2
            height: width

            radius: width / 2
            color: "white"
        }
    }
}
