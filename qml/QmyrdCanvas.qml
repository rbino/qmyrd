import QtQuick 2.5;
import QtQuick.Window 2.2;

Window {
    id: mainView;

    visible: true;

    property int t: 0;

    Timer {
        interval: 16;
        running: true;
        repeat: true;

        onTriggered: {
            t += 16;
        }
    }

    Loader {
        id: loader;

        source: $ComponentUrl;

        function reload() {
            source = "";
            source = $ComponentUrl;
        }

        anchors {
            fill: parent;
        }
    }

    Connections {
        target: $Engine;
        onComponentChanged: {
            console.log("Source changed, reloading");
            loader.reload();
        }
    }
}
