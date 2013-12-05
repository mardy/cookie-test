import QtQuick 2.0
import Ubuntu.Components 0.1
import Ubuntu.OnlineAccounts 0.1
import Ubuntu.OnlineAccounts.Client 0.1

MainView {
    property string provider: "facebook"
    property string url: provider === "google" ? "http://www.gmail.com" : "http://www.facebook.com"

    applicationName: "cookie-test"
    
    width: units.gu(100)
    height: units.gu(75)

    PageStack {
        id: pageStack
        Component.onCompleted: push(page0)

        Page {
            id: page0
            title: i18n.tr("Cookie test")

            ListView {
                id: accountsList
                spacing: units.gu(1)
                anchors {
                    top: parent.top
                    left: parent.left
                    right: parent.right
                    bottom: authorizeBtn.top
                }
                model: accountsModel
                delegate: Rectangle {
                    id: wrapper
                    width: accountsList.width
                    height: units.gu(10)
                    color: accts.enabled ? "green" : "red"

                    AccountService {
                        id: accts
                        objectHandle: accountServiceHandle
                        onAuthenticated: getCookies(authData.credentialsId)
                        onAuthenticationError: {
                            console.log("Authentication failed, code " + error.code)
                        }
                    }

                    Button {
                        anchors.fill: parent
                        anchors.margins: units.gu(2)

                        text: i18n.tr("Log into %1").arg(displayName)

                        onClicked: accts.authenticate(null)
                    }
                }
            }

            Button {
                id: authorizeBtn
                anchors {
                    left: parent.left
                    right: parent.right
                    bottom: parent.bottom
                }
                text: "Add new account"
                onClicked: setup.exec()
            }
        }
    }

    AccountServiceModel {
        id: accountsModel
        includeDisabled: false
        serviceType: "webapps"
    }

    Setup {
        id: setup
        providerId: provider
        applicationId: "cookie-test"
    }

    function getCookies(id) {
        console.log("Getting cookies for " + id)
        fileOps.copyCookies(id,
        fileOps.homeDir + "/.local/share/cookie-test/.QtWebKit/cookies.db")
        pageStack.push(Qt.resolvedUrl("WebPage.qml"), { "url": url })
    }
}
