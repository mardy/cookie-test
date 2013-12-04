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

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QGuiApplication>
#include <QQmlContext>
#include <QQuickView>

class FileOps: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString homeDir READ homeDir CONSTANT)

public:
    FileOps(QObject *parent = 0): QObject(parent) {}
    ~FileOps() {};

    QString homeDir() const;

    Q_INVOKABLE void copyFile(const QString &source, const QString &dest) const;
};

QString FileOps::homeDir() const
{
    return QDir::homePath();
}

void FileOps::copyFile(const QString &source, const QString &dest) const
{
    qDebug() << Q_FUNC_INFO << source << dest;
    QFileInfo info(dest);
    info.dir().mkpath(".");
    QFile::copy(source, dest);
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
