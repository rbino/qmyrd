import QtQuick 2.5;
import QtQuick.Window 2.2;

Window {
    id: mainView;

    visible: true;

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
