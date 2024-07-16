import QtQuick 2.5
import QtQuick.Window 2.5
import QtQuick.Controls 2.5

Popup {
    // title: qsTr("GeoMessenger")
    closePolicy: Popup.CloseOnEscape
    modal: true
    focus: true

    enter: Transition {
        NumberAnimation { property: "opacity"; from: 0.0; to: 1.0; duration: 200 }
    }

    Rectangle {
        id: passwordTextField

        width: 300
        height: 50
        anchors.centerIn: parent

        color: "white"
        border.color: root.logInError ? "red" : "black"
        radius: height/5

        property bool showPass : false

        TextField {
            id: passwordTextFieldin

            width: parent.width - parent.height
            height: parent.height
            anchors.top: parent.top
            anchors.left: parent.left
            font.pointSize: 12

            placeholderText: "Password"

            echoMode:passwordTextField.showPass ? TextInput.Normal : TextInput.Password

            background: Rectangle {
                radius: parent.height/5
                color: "transparent"
                border.color: "transparent"
            }

            text: _kSettings.password;
        }

        CircleButton {
            id: showPassword

            width: parent.height
            height: parent.height
            anchors.right: parent.right
            anchors.top: parent.top

            imageSource: passwordTextField.showPass ? "/labels/eye.png" : "/labels/hide.png"

            onClicked: {
                passwordTextField.showPass = !passwordTextField.showPass
            }
        }
    }


    FancyTextField {
        id: jidTextField

        width: passwordTextField.width
        height: passwordTextField.height
        anchors.bottom: passwordTextField.top
        anchors.horizontalCenter: passwordTextField.horizontalCenter
        anchors.margins: 30

        borderColor: root.logInError ? "red" : "black"

        placeholderText: "jid"

        text: _kSettings.jid
    }

    CheckBox {
        text: qsTr("Automatic login on startup")
        checked: _kSettings.autologin
        onCheckedChanged: {
            _kSettings.autologin = checked;
        }
    }


    Item {
        width: passwordTextField.width
        height: passwordTextField.height
        anchors.top: passwordTextField.bottom
        anchors.horizontalCenter: passwordTextField.horizontalCenter
        anchors.margins: 30

        FancyButton {
            id: okLogIn
            anchors.left: parent.left
            anchors.top: parent.top
            height: parent.height
            width: parent.width/2 - 10

            text: qsTr("OK")

            onClicked: {
                root.connectToServer(jidTextField.text, passwordTextFieldin.text);
                // mainWindow.logInFlag = true;
                // popupLogIn.close();
            }
        }

        FancyButton {
            id: cancelLogIn
            anchors.right: parent.right
            anchors.top: parent.top
            height: parent.height
            width: parent.width/2 - 10

            text: qsTr("Cancel")

            onClicked: popupLogIn.close()
        }
    }
    function saveAccount(){
        _kSettings.saveAccount(jidTextField.text, passwordTextFieldin.text);
    }
}
