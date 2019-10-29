import QtQuick 2.5;
import QtQuick.Window 2.2;

Window {
    id: mainView;

    visible: true;

    property int t: 0;
    property var hiddenLoader: loaderA.visible ? loaderB : loaderA;

    Timer {
        interval: 16;
        running: true;
        repeat: true;

        onTriggered: {
            t += 16;
        }
    }

    Loader {
        id: loaderA;

        source: $ComponentUrl;
        asynchronous: true;
        visible: true;

        function reload(componentUrl) {
            source = "";
            source = componentUrl;
        }

        onLoaded: {
            loaderA.visible = true;
            loaderB.visible = false;
        }

        anchors {
            fill: parent;
        }
    }

    Loader {
        id: loaderB;

        source: "";
        asynchronous: true;
        visible: false;


        function reload(componentUrl) {
            source = "";
            source = componentUrl;
        }

        onLoaded: {
            loaderB.visible = true;
            loaderA.visible = false;
        }

        anchors {
            fill: parent;
        }
    }

    Connections {
        target: $Engine;
        onComponentChanged: {
            console.log("Source changed, reloading");
            hiddenLoader.reload($ComponentUrl);
        }
    }
}
