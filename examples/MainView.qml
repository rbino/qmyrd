import QtQuick 2.5;

import com.rbino.qmyrd 1.0;

Item {
    id: root;

    property int t: 0;

    Timer {
        interval: 16;
        running: true;
        repeat: true;

        onTriggered: {
            t += 16;
        }
    }

    JackClient {
        id: cl;
    }

    Grid {
        rows: 4;
        columns: 4;

        spacing: 140;

        anchors {
            centerIn: parent;
            horizontalCenterOffset: Math.sin(t / 100) * 40;
            verticalCenterOffset: Math.cos(t / 100 + 100) * 10;
        }

        Repeater {
            model: 16;

            delegate: myrect;
        }
    }

    Component {
        id: myrect;
        Rectangle {
            id: rectangle;

            color: Qt.hsva(cl.volume, 0.8, 0.8, 1.0);

            height: 100 + 100 * cl.volume;
            width: 100 + 100 * cl.volume;
        }
    }
}
