/*
    SPDX-FileCopyrightText: 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "kxmlgui5configmigration.h"

// lib
#include "debug.h"
// Qt
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QStandardPaths>

namespace KXmlGui5ConfigMigration
{

bool migrate(const QString &appName, const QStringList &uiFileNames, MigrationOptions options)
{
    if (appName.isEmpty() && !uiFileNames.isEmpty()) {
        qCCritical(DEBUG_KXMLGUI) << "Can not migrate ui files. the app name is missing";
        return false;
    }

    const QString oldConfigDir =
        QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QLatin1String("/kxmlgui5/") + appName + QLatin1Char('/');
    if (!QDir(oldConfigDir).exists()) {
        return false;
    }

    const QString newConfigDir =
        QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation) + QLatin1Char('/') + appName + QLatin1String("/kxmlgui6/");

    for (const QString &uiFileName : uiFileNames) {
        const QString newConfigLocation = newConfigDir + uiFileName;

        // skip if already copy in new one location
        if (QFile(newConfigLocation).exists()) {
            continue;
        }

        QFileInfo fileInfo(newConfigLocation);
        QDir().mkpath(fileInfo.absolutePath());
        const QString oldConfigLocation = oldConfigDir + uiFileName;
        QFile oldConfigFile(oldConfigLocation);

        if (!oldConfigFile.exists()) {
            continue;
        }
        if (oldConfigFile.copy(newConfigLocation)) {
            qCDebug(DEBUG_KXMLGUI) << "ui file" << oldConfigLocation << "was migrated to" << newConfigLocation;
            if (options.testFlag(RemoveOldFiles)) {
                if (!oldConfigFile.remove()) {
                    qCWarning(DEBUG_KXMLGUI) << "Old ui file" << oldConfigLocation << "could not be removed, kept around.";
                }
            }
        } else {
            qCWarning(DEBUG_KXMLGUI) << "ui file" << oldConfigLocation << "could not be copied to" << newConfigLocation;
        }
    }

    if (options.testFlag(RemoveOldFiles)) {
        QDir().rmpath(oldConfigDir);
    }

    return true;
}

}
