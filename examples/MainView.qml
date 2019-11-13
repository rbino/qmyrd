import QtQuick 2.5;

import com.rbino.qmyrd 1.0;

Item {
    TidalEnvGen {
        id: kickEnv;

        attack: 0;
        release: 0.2;

        matchKey: "s";
        matchValue: "bd";
    }

    TidalEnvGen {
        id: snEnv;

        attack: 0;
        release: 0.2;

        matchKey: "s";
        matchValue: "sn";
    }

    TidalEnvGen {
        id: chipEnv;

        attack: 0;
        release: 0.2;

        matchKey: "s";
        matchValue: "superchip";
    }


    Rectangle {
        color: "green";

        width: 300 + kickEnv.value * 300;
        height: width;
        rotation: chipEnv.value * 10;

        anchors {
            right: parent.horizontalCenter;
            rightMargin: 300;
            verticalCenter: parent.verticalCenter;
        }
    }

    Rectangle {
        color: "red";

        width: 300 + snEnv.value * 300;
        height: width;
        rotation: chipEnv.value * 10;

        anchors {
            left: parent.horizontalCenter;
            leftMargin: 300;
            verticalCenter: parent.verticalCenter;
        }
    }
}
