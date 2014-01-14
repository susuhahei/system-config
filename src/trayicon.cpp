/*
    SnoreNotify is a Notification Framework based on Qt
    Copyright (C) 2013-2014  Patrick von Reth <vonreth@kde.org>


    SnoreNotify is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SnoreNotify is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with SnoreNotify.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "trayicon.h"
#include "core/snore.h"
#include "core/snore_p.h"

#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>

#include "core/version.h"

using namespace Snore;

TrayIcon::TrayIcon():
    m_trayIcon(new QSystemTrayIcon(QIcon(":/root/snore.png")))
{	
}

void TrayIcon::initConextMenu(SnoreCore *snore)
{
    m_snore = snore;
    m_trayIcon->setVisible(true);

    m_trayMenu = new QMenu("SnoreNotify");
    m_trayMenu->addAction(QString("SnoreNotify ").append(Version::version()));
    m_trayMenu->addSeparator();
    m_trayMenu->addAction("Test Notification", this, SLOT(slotTestNotification()));
    m_trayMenu->addSeparator();
    foreach(const QString &back,m_snore->notificationBackends())
    {
        QAction *b = m_trayMenu->addAction(back, this, SLOT(setPrimaryBackend()));
        b->setCheckable(true);
        if(back == m_snore->primaryNotificationBackend())
        {
            b->setChecked(true);
        }
        m_backendActions.append(b);
    }
    m_trayMenu->addSeparator();
    m_trayMenu->addAction("Exit",qApp,SLOT(quit()));


    m_trayIcon->setContextMenu(m_trayMenu);
}

void TrayIcon::hide()
{
    m_trayIcon->setVisible(false);
}

QSystemTrayIcon* TrayIcon::trayIcon()
{
    return m_trayIcon;
}

void TrayIcon::setPrimaryBackend(){
    QAction *a = qobject_cast<QAction*>(sender());
    m_snore->setPrimaryNotificationBackend(a->text());

    foreach(QAction *action,m_backendActions)
    {
        action->setChecked(false);
    }
    a->setChecked(true);

}

void TrayIcon::slotTestNotification()
{
    const Application &app = m_snore->d()->defaultApplication();
    m_snore->registerApplication(app);
    Notification n(app, *app.alerts().begin(), "Hello World", "This is Snore", Icon(":/root/snore.png"));
    n.addAction(Notification::Action(1,"Test Action"));
    m_snore->broadcastNotification(n);
    m_snore->deregisterApplication(app);
}

