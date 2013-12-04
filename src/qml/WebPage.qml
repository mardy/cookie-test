import QtQuick 2.0
import Ubuntu.Components 0.1
import Ubuntu.Components.Extras.Browser 0.1

Page {
    id: root
    title: "Webview"

    UbuntuWebView {
        anchors.fill: parent
        url: "https://www.gmail.com"
    }
}
