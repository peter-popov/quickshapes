import QtQuick 2.1
import QtQuick.Controls 1.0
import ShapefileView 1.0

ApplicationWindow {
    id: applicationWindow1
    title: qsTr("Hello World")
    visible: true
    width: 1200
    height: 840

    menuBar: MenuBar {
        Menu {
            title: "File"
            MenuItem { text: "Open..." }
        }
    }        

    Button {
        id: hello
        text: "Hello"
        width: 60; height: 40
    }

    ShapefileView {
        id: view
        anchors.left: hello.right
        width: 1000; height: 800
        source: "indoor_1.csv"
    }

    Button {
        text: "World"
        anchors.left: view.right
        width: 60; height: 40
    }    
}