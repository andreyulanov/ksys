import QtQuick 2.5
import QtQuick.Window 2.5
import QtQuick.Controls 2.5
import QtLocation 5.15
import QtQuick.Layouts 1.11


Item {
    id: root
    visible: true
    anchors.fill: parent

    Rectangle {
        anchors.fill: parent
        color: "white"
        border.color: "white"
    }

    signal connectToServer(login: string, password: string)
    property bool logInFlag: false
    property bool logInError: false

    LogIn {
        id: popupLogIn
        anchors.centerIn: parent
        width: parent.width * 0.8
        height: parent.height * 0.8
    }

    Drawer {
        id: drawerMenu
        height: parent.height
        width: 400

        ColumnLayout{
            anchors.fill: parent

            FancyButton{
                id: logOutButton
                text: qsTr("Log Out")

                onClicked: {
                    kClient.disconnectFromServer();
                }
            }

            FancyButton{
                id: connectToChate
                text: qsTr("Connectetd by JID")

                onClicked: {

                }
            }
        }
    }

    Item {
        id: chatArea
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: mapAria.left

        Item{
            visible: logInFlag
            id: chatAreaHead

            anchors.top: parent.top
            anchors.right: parent.right
            anchors.left: parent.left
            height: 50

            CircleButton {
                id: menuButton
                anchors.left: parent.left
                anchors.top: parent.top
                width: parent.height
                height: parent.height

                imageSource: "/labels/menu.png"

                onClicked: drawerMenu.open()
            }

            FancyTextField {
                id: chatSearch

                anchors.right: parent.right
                anchors.left: menuButton.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.margins: 5

                placeholderText: "Search"
            }
        }

        Item{
            id: logInArea

            anchors.top: parent.top
            anchors.right: parent.right
            anchors.left: parent.left
            height: 50

            visible: !logInFlag

            FancyButton {
                id: logButtont

                anchors.fill: parent
                anchors.margins: 5

                text: qsTr("Log In")
                onClicked: {
                    popupLogIn.open();
                    logInError = false;
                }
            }
        }

        Item{
            id: chats
            // visible: logInFlag
            visible: true

            anchors.top: chatAreaHead.bottom
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.left: parent.left
            height: 50

            ListView {
                id: view

                anchors.margins: 10
                anchors.fill: parent
                spacing: 10
                model: _mucRoomsModel
                clip: true

                highlight: Rectangle {
                    color: "skyblue"
                }
                highlightFollowsCurrentItem: true

                header: Rectangle {
                    width: view.width
                    height: 40

                    Grid {
                        id: addFormGrid
                        RowLayout {
                            width: view.width
                            TextField {
                                id: roomJidTextField1
                                anchors.fill: parent
                                Layout.fillWidth: true
                                text: _mucBackEnd.room_jid
                                placeholderText: qsTr("Room Jid")
                                onEditingFinished: _mucBackEnd.room_jid = text
                                onFocusChanged: color = "black"
                            }
                            Button {
                                text: "Add"
                                onClicked: {
                                    _mucBackEnd.add();
                                }
                            }
                            Button {
                                text: "Remove"
                                onClicked: {
                                    console.log("Removeing rooom at row" + view.currentIndex);
                                    _mucRoomsModel.removeRows(view.currentIndex,1);
                                }
                            }
                        }
                    }

                    Connections {
                        target: _mucBackEnd
                        function onInvalidJid() {
                            roomJidTextField1.color = "red"
                        }
                    }
                }

                delegate: Item {
                    id: listDelegate

                    property var view: ListView.view
                    property var isCurrent: ListView.isCurrentItem

                    width: view.width
                    height: 60

                    ContactCard {
                        anchors.margins: 2
                        anchors.fill: parent

                        name: "%1 (%2)".arg(model.name).arg(model.jid)
                        lastMessage: "%1".arg(model.subject)
                        // time: qsTr("")

                        onClicked: {
                            console.log("Clicked room", model.jid);
                            view.currentIndex = model.index
                        }
                    }
                }
            }
        }
    }

    MapArea {
        id: mapAria

        anchors.top: parent.top
        anchors.bottom: parent.bottom

        x: 300
        width: parent.width - x
    }

    // property string name
    // property string time
    // property string lastMessage

    Connections {
        target: kClient
        onConnected: {
            onConnected();
        }
        onError:{
            onError();
        }
        onDisconnected: {
            onDisconnected();
        }
    }

    function onConnected() {
        logInFlag = true;
        popupLogIn.close();
    }

    function onError() {
        logInError = "true";
    }

    function onDisconnected(){
        logInFlag = false;
        drawerMenu.close();
    }
}


