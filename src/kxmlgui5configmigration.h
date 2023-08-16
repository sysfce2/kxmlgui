/*
    SPDX-FileCopyrightText: 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KXMLGUI5CONFIGMIGRATION_H
#define KXMLGUI5CONFIGMIGRATION_H

#include <kxmlgui_export.h>
// Qt
#include <QFlags>
#include <QStringList>

/**
 * The namespace for the migratios of selected .ui files
 * from the KF5 location in XDG_DATA_HOME
 * to the KF6 location in XDG_CONFIG_HOME.
 */
namespace KXmlGui5ConfigMigration
{

/**
 * Options to configure the migration.
 * @see MigrationOptions
 */
enum MigrationOption {
    NoMigrationOptions = 0x0, ///< No options
    RemoveOldFiles = 0x1, ///< Remove files in the old location. Failing is not an error.
};
/**
 * Stores a combination of #MigrationOption values.
 */
Q_DECLARE_FLAGS(MigrationOptions, MigrationOption)

/**
 * Migrate the .ui files, if any.
 *
 * It will return @c false if there was nothing to migrate.
 * The returned value is unrelated to error handling. It is just a way to skip anything else
 * related to migration on a clean system, by code like
 * @code
 * if (KXmlGui5ConfigMigration::migrate("foo", {"fooui.rc"})) {
 *    // look for related old data to migrate as well
 * }
 * @endcode
 *
 * @param appName the application name, which is used for the directory containing the .ui files.
 * @param uiFileNames list of ui files
 * @param options flags to control the migration, default asks to removes the old files
 * @return @c true if the migration happened, otherwise @c false.
 *
 * @since 6.0
 */
KXMLGUI_EXPORT
bool migrate(const QString &appName, const QStringList &uiFileNames, MigrationOptions options = RemoveOldFiles);

}

#endif
