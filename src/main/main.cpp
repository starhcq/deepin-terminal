// Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd
// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "mainwindow.h"
#include "environments.h"
#include "dbusmanager.h"
#include "service.h"
#include "utils.h"
#include "terminalapplication.h"
#include "define.h"


#include <DApplication>
#include <DApplicationSettings>
#include <DLog>

#include <QDir>
#include <QDebug>
#include <QCommandLineParser>
#include <QTranslator>
#include <QTime>

DWIDGET_USE_NAMESPACE

DCORE_USE_NAMESPACE

Q_DECLARE_LOGGING_CATEGORY(LogMain)

int main(int argc, char *argv[])
{
    if (!QString(qgetenv("XDG_CURRENT_DESKTOP")).toLower().startsWith("deepin")) {
        setenv("XDG_CURRENT_DESKTOP", "Deepin", 1);
    }
    // 应用计时
    QTime useTime;
    useTime.start();
    //为了更精准，起动就度量时间
    qint64 startTime = QDateTime::currentDateTime().toMSecsSinceEpoch();

    // 启动应用
    TerminalApplication app(argc, argv);
    app.setStartTime(startTime);
    DApplicationSettings set(&app);

    // 系统日志
#if (DTK_VERSION >= DTK_VERSION_CHECK(5,6,8,0))
    qCDebug(LogMain) << "current libdtkcore5 > 5.6.8.0";
    DLogManager::registerJournalAppender();
    qCInfo(LogMain) << "Current log register journal!";
#ifdef QT_DEBUG
    DLogManager::registerConsoleAppender();
    qCInfo(LogMain) << "Current log register console!";
#endif
#else
    qCDebug(LogMain) << "current libdtkcore5 < 5.6.8.0";
//    DLogManager::registerJournalAppender();
    DLogManager::registerConsoleAppender();
    DLogManager::registerFileAppender();
    qCInfo(LogMain) << "Current log register console and file!";
#endif

    // 参数解析
    TermProperties properties;
    Utils::parseCommandLine(app.arguments(), properties, true);
    QStringList args = app.arguments();

    if(!(args.contains("-w") || args.contains("--work-directory"))) {
        args += "-w";
        args += QDir::currentPath();
    }

    DBusManager manager;
    if (!manager.initDBus()) {
        // 非第一次启动
        DBusManager::callTerminalEntry(args);
        return 0;
    }
    // 第一次启动
    // 这行不要删除
    qputenv("TERM", "xterm-256color");
    // 首次启动
    QObject::connect(&manager, &DBusManager::entryArgs, Service::instance(), &Service::Entry);
    Service::instance()->EntryTerminal(args);
    // 监听触控板事件
    manager.listenTouchPadSignal();

    return app.exec();
}
