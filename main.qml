import QtQuick 2.3
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1

Window {
    property int taskIndex: 0
    property var task : {"name": "", "result": "", "state": "", "id": "", "repo": "", "command": "", "software": 0, "instance": 0}
    visible: true
    width: 510

    Button {
        id: updateApiKey
        text: "Update api-key"
        onClicked: apiKeyDialog.visible = true
        anchors.left: parent.left
    }

    Button {
        id: createTask
        text: "Create task"
        onClicked: createTaskDialog.visible = true
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Button {
        id: refreshTask
        text: "Refresh tasks"
        onClicked: controller.refreshTask();
        anchors.right: parent.right
    }

    TableView {
        id: tasksTable
        anchors.top: refreshTask.bottom
        width: 510
        TableViewColumn {
            role: "id"
            title: "Id"
            width: 60
        }
        TableViewColumn {
            role: "name"
            title: "Name"
            width: 100
            delegate: Rectangle {
                width: 100
                height: 35

                Text {
                    text: styleData.value
                }

                MouseArea {
                    width: 100
                    height: 35

                    onClicked: {
                        taskIndex = styleData.row;
                        task = controller.getTask(taskIndex);
                        taskViewDialog.visible = true;
                    }
                }
            }
        }
        TableViewColumn {
            role: "state"
            title: "State"
            width: 200
        }
        TableViewColumn {
            title: "Action"
            width: 150
            delegate: Rectangle {
                width: 200
                height: 35

                Button {
                    text: "Stop"
                    onClicked: controller.stopTask(styleData.row);
                    anchors.left: parent.left
                }

                Button {
                    text: "Delete"
                    onClicked: controller.deleteTask(styleData.row);
                    anchors.right: parent.right
                }
            }
        }
        model: tasks
    }

    Dialog {
        id: taskViewDialog
        visible: false
        title: "Task view"

        contentItem: Rectangle {
            implicitWidth: 400
            implicitHeight: 280
            anchors.centerIn: parent

            ColumnLayout {
                width: parent.width

                Rectangle {
                    width: parent.width
                    height: 25
                    Text {
                        text: "ID: "
                    }
                    Text {
                        anchors.right: parent.right
                        text: task.id;
                    }
                }

                Rectangle {
                    width: parent.width
                    height: 25
                    Text {
                        text: "State: "
                    }
                    Text {
                        anchors.right: parent.right
                        text: task.state;
                    }
                }

                Rectangle {
                    width: parent.width
                    height: 25
                    Text {
                        text: "Task name: "
                    }
                    Text {
                        anchors.right: parent.right
                        text: task.name;
                    }
                }

                Rectangle {
                    width: parent.width
                    height: 25
                    Text {
                        text: "Result file: "
                    }
                    Text {
                        anchors.right: parent.right
                        text: task.result;
                    }
                }

                Rectangle {
                    width: parent.width
                    height: 25
                    Text {
                        text: "Repository: "
                    }
                    Text {
                        anchors.right: parent.right
                        text: task.repo;
                    }
                }

                Rectangle {
                    width: parent.width
                    height: 25
                    Text {
                        text: "Software: "
                    }
                    Text {
                        anchors.right: parent.right
                        text: swItems.get(task.software).text;
                    }
                }

                Rectangle {
                    width: parent.width
                    height: 25
                    Text {
                        text: "Instance: "
                    }
                    Text {
                        anchors.right: parent.right
                        text: insItems.get(task.instance).text;
                    }
                }

                Rectangle {
                    width: parent.width
                    height: 25
                    Text {
                        text: "Run command: "
                    }
                    Text {
                        anchors.right: parent.right
                        text: task.command;
                    }
                }

                Button {
                    text: "Download result"
                    onClicked: controller.downloadResult(taskIndex);
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                }
            }

        }
    }

    Dialog {
        id: createTaskDialog
        visible: false
        title: "Create task"

        contentItem: Rectangle {
            implicitWidth: 400
            implicitHeight: 210
            anchors.centerIn: parent

            ColumnLayout {
                width: parent.width
                Rectangle {
                    width: parent.width
                    height: 25
                    Text {
                        text: "Task name: "
                    }
                    TextField {
                        text: "TestTask"
                        id: taskNameField
                        anchors.right: parent.right
                    }
                }

                Rectangle {
                    width: parent.width
                    height: 25
                    Text {
                        text: "Result name: "
                    }
                    TextField {
                        text: "TestResult"
                        id: resultNameField
                        anchors.right: parent.right
                    }
                }

                Rectangle {
                    width: parent.width
                    height: 25
                    Text {
                        text: "Repository: "
                    }
                    TextField {
                        text: "https://github.com/FlyElephantNet/python-example.git"
                        id: repoField
                        anchors.right: parent.right
                    }
                }


                Rectangle {
                    width: parent.width
                    height: 25
                    Text {
                        text: "Software: "
                    }
                    ComboBox {
                        id: softwareField
                        currentIndex: 0
                        model: ListModel {
                            id: swItems
                            ListElement { text: "GCC v4.9.2"; value: "3" }
                            ListElement { text: "Python v2"; value: "4" }
                        }
                        anchors.right: parent.right
                        width: 150
                    }
                }

                Rectangle {
                    width: parent.width
                    height: 25
                    Text {
                        text: "Instance: "
                    }
                    ComboBox {
                        id: instanceField
                        currentIndex: 0
                        model: ListModel {
                            id: insItems
                            ListElement { text: "A0 : 1 cores | 1GB RAM | 20GB HDD"; value: "A0" }
                            ListElement { text: "A1 : 1 cores | 2GB RAM | 70GB HDD"; value: "A1" }
                        }
                        anchors.right: parent.right
                        width: 250
                    }
                }

                Rectangle {
                    width: parent.width
                    height: 25
                    Text {
                        text: "Run command: "
                    }
                    TextField {
                        text: "python matrix.py"
                        id: commandField
                        anchors.right: parent.right
                    }
                }
            }

            Button {
                text: "Ok"
                onClicked: {
                    controller.createTask(
                        taskNameField.text,
                        resultNameField.text,
                        repoField.text,
                        swItems.get(softwareField.currentIndex).value,
                        insItems.get(instanceField.currentIndex).value,
                        commandField.text);
                    createTaskDialog.visible = false
                }
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
            }
        }
    }

    Dialog {
        id: apiKeyDialog
        visible: false
        title: "Api Key"

        contentItem: Rectangle {
            implicitWidth: 400
            implicitHeight: 50
            anchors.centerIn: parent
            Text {
                id: apiKeyText
                text: "Key: "
            }
            TextField {
                id: apiKeyField
                anchors.right: parent.right
                anchors.left: apiKeyText.right
            }
            Button {
                text: "Ok"
                onClicked: {
                    controller.setApiKey(apiKeyField.text);
                    apiKeyDialog.visible = false
                }
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
            }
        }
    }
}
