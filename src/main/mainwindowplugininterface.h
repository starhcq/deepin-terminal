/*
 *  Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd
 *
 * Author:  <wangpeili@uniontech.com>
 *
 * Maintainer:<wangpeili@uniontech.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MAINWINDOWPLUGININTERFACE_H
#define MAINWINDOWPLUGININTERFACE_H

#include <QAction>

class MainWindow;
class MainWindowPluginInterface : public QObject
{
    Q_OBJECT
public:
    MainWindowPluginInterface(QObject *parent) : QObject(parent)
    {
    }

    virtual void initPlugin(MainWindow *mainWindow) = 0;
    virtual QAction *titlebarMenu(MainWindow *mainWindow) = 0;

    QString getPluginName()
    {
        return m_pluginName;
    }

    QString m_pluginName;
};

#endif  // MAINWINDOWPLUGININTERFACE_H
