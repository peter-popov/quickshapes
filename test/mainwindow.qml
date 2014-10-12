import QtQuick 2.1
import QtQuick.Controls 1.0
import ShapesView 1.0

ApplicationWindow {
    id: applicationWindow1
    title: qsTr("QML + Shape files")
    visible: true
    width: 600
    height: 400

    ShapefileView {
        id: view
        anchors.fill: parent        
        source: "../test/indoor_1.csv"
        transformOrigin : "TopLeft"             
    }
}