import QtQuick 1.1
import com.nokia.meego 1.0

import MyComponents 1.1

Page {
    id: page
    tools: commonTools

    property string title : "SMS Exporter"

    Image {
        id: pageHeader
        anchors {
            top: page.top
            left: page.left
            right: page.right
        }

        height: parent.width < parent.height ? 72 : 46
        width: parent.width
        source: "image://theme/meegotouch-view-header-fixed" + (theme.inverted ? "-inverted" : "")
        z: 1

        Label {
            id: header
            anchors {
                verticalCenter: parent.verticalCenter
                left: parent.left
                leftMargin: 16
            }
            platformStyle: LabelStyle {
                fontFamily: "Nokia Pure Text Light"
                fontPixelSize: 32
            }
            text: page.title
        }
    }

    Column {
        id: pageFlickableContent
        anchors {
            top: pageHeader.bottom
            left: page.left
            right: page.right
            margins: 16
        }

        Button{
            id: actionButton
            anchors.horizontalCenter: parent.horizontalCenter
            text: qsTr("Read SMS")
            onClicked: {
                if (smsModel.count == 0)
                    smsExporterObject.readSMS();
                else {
                    if (actionButton.text == "Export SMS")
                        smsExporterObject.exportSMS();
                    else {
                        // call open file
                        smsExporterObject.openArchive();
                    }
                }
            }
        }

        Text{
            id: numberOfSMS_t
            anchors.horizontalCenter: parent.horizontalCenter
            text: ""
            font.pointSize: 18
        }

        Text{
            id: exportStatus_t
            anchors.horizontalCenter: parent.horizontalCenter
            text: ""
            font.pointSize: 18
            width: parent.width
            elide: Text.ElideRight
        }
    }

    ListView {
        id: smsListView
        clip: true

        anchors {
            top: pageFlickableContent.bottom
            bottom: page.bottom
            left: page.left
            right: page.right
            margins: 16
        }

        model: smsModel

        delegate: Column{
            Text {
                text: "On " + date
                font.pointSize: 18
                font.bold: true
                width: smsListView.width
            }
            Text {
                id: fromText
                font.pointSize: 18
                text: "   From : " + from
                width: smsListView.width
                elide: Text.ElideRight
            }
            Text {
                font.pointSize: 18
                text: "   To : " + to
                width: smsListView.width
                elide: Text.ElideRight
            }
            Text {
                font.pointSize: 18
                text: content
                width: smsListView.width
                wrapMode: Text.WordWrap
                font.italic: true
            }
        }
    }

    ListModel{
        id: smsModel
    }

    SMSExporter{
        id: smsExporterObject
        onFoundMessages: {
            numberOfSMS_t.text = "Found " + count + " messages";
            actionButton.text = "Export SMS";
        }

        onFoundMessageDetails:{//QString date, QString from, QString to, QString content);
            smsModel.append({"date": date, "from":from, "to":to, "content":content })
        }

        onMessagesExportedTo: {
            numberOfSMS_t.text = "SMS saved to file :";
            exportStatus_t.text = filepath;

            actionButton.text = "Open export file";
        }
    }
}
