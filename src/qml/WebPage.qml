import QtQuick 2.0
import Ubuntu.Components 0.1
import Ubuntu.Components.Extras.Browser 0.1

Page {
    id: root

    property alias url: view.url

    title: "Webview"

    UbuntuWebView {
        id: view
        anchors.fill: parent
    }
}
