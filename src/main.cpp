/*
 * Copyright (C) 2013 Canonical Ltd.
 *
 * Contact: Alberto Mardegan <alberto.mardegan@canonical.com>
 *
 * This file is part of online-accounts-ui
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 3, as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranties of
 * MERCHANTABILITY, SATISFACTORY QUALITY, or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusMetaType>
#include <QDebug>
#include <QDir>
#include <QGuiApplication>
#include <QMap>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QQmlContext>
#include <QQuickView>

typedef QMap<QString,QString> RawCookies;

Q_DECLARE_METATYPE(RawCookies)

class FileOps: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString homeDir READ homeDir CONSTANT)

public:
    FileOps(QObject *parent = 0);
    ~FileOps() {};

    QString homeDir() const;

    Q_INVOKABLE void copyCookies(quint32 id, const QString &dest) const;

private:
    QDBusConnection m_conn;
};

FileOps::FileOps(QObject *parent):
    QObject(parent),
    m_conn(QDBusConnection::sessionBus())
{
    qRegisterMetaType<RawCookies>("RawCookies");
    qDBusRegisterMetaType<RawCookies>();
}

QString FileOps::homeDir() const
{
    return QDir::homePath();
}

void FileOps::copyCookies(quint32 id, const QString &dest) const
{
    qDebug() << Q_FUNC_INFO << id << dest;

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dest);
    if (!db.open()) {
        qWarning() << "Could not open DB";
        return;
    }

    /* reset existing DB */
    QSqlQuery q(db);
    q.exec("DELETE FROM cookies;");

    /* Get the cookies from the identity */
    QDBusMessage message =
        QDBusMessage::createMethodCall("com.nokia.singlesignonui",
                                       "/SignonUi",
                                       "com.nokia.singlesignonui",
                                       "cookiesForIdentity");
    message.setArguments(QVariantList() << id);
    QDBusMessage reply = m_conn.call(message);
    qDebug() << "Got DBus reply" << reply;
    if (reply.type() == QDBusMessage::ErrorMessage) {
        qWarning() << "Got error:" << reply.errorMessage();
        return;
    }

    RawCookies cookies = qdbus_cast<RawCookies>(reply.arguments()[0]);
    qDebug() << "Cookies:" << cookies;
    q.prepare("INSERT INTO cookies (cookieId, cookie) "
              "VALUES (:cookieId, :cookie)");
    for (RawCookies::const_iterator i = cookies.constBegin();
         i != cookies.constEnd();
         i++) {
        q.bindValue(":cookieId", i.key());
        q.bindValue(":cookie", i.value());
        if (!q.exec()) {
            qWarning() << "Couldn't insert cookie into DB" << i.key();
        }
    }
}

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);

    FileOps fileOps;
    view.rootContext()->setContextProperty("fileOps", &fileOps);

    view.setSource(QUrl(QStringLiteral("qrc:/qml/cookie-test.qml")));
    view.show();

    return app.exec();
}

#include "main.moc"
