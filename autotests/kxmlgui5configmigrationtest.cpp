/*
    SPDX-FileCopyrightText: 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

// test object
#include <kxmlgui5configmigration.h>
// Qt
#include <QFile>
#include <QObject>
#include <QStandardPaths>
#include <QTemporaryDir>
#include <QTest>

class KXmlGui5ConfigMigrationTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void init();

    void nothingToMigrate();
    void migrateUiFiles_data();
    void migrateUiFiles();
};

Q_DECLARE_METATYPE(KXmlGui5ConfigMigration::MigrationOptions)

void KXmlGui5ConfigMigrationTest::initTestCase()
{
    qRegisterMetaType<KXmlGui5ConfigMigration::MigrationOptions>("KXmlGui5ConfigMigration::MigrationOptions");

    QStandardPaths::setTestModeEnabled(true);
}

void KXmlGui5ConfigMigrationTest::init()
{
    const QString configHome = QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation);
    QDir(configHome).removeRecursively();
    const QString dataHome = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
    QDir(dataHome).removeRecursively();
}

void KXmlGui5ConfigMigrationTest::nothingToMigrate()
{
    const QString appName = QStringLiteral("foo");
    const QStringList uiFileNames{QStringLiteral("appuirc"), QStringLiteral("app2uirc")};

    // try migration
    QCOMPARE(KXmlGui5ConfigMigration::migrate(appName, uiFileNames), false);
}

void KXmlGui5ConfigMigrationTest::migrateUiFiles_data()
{
    QTest::addColumn<KXmlGui5ConfigMigration::MigrationOptions>("options");
    QTest::addColumn<bool>("oldFileKept");

    QTest::newRow("oldFileKept") << KXmlGui5ConfigMigration::MigrationOptions(KXmlGui5ConfigMigration::NoMigrationOptions) << true;
    QTest::newRow("oldFileRemoved") << KXmlGui5ConfigMigration::MigrationOptions(KXmlGui5ConfigMigration::RemoveOldFiles) << false;
}

void KXmlGui5ConfigMigrationTest::migrateUiFiles()
{
    QFETCH(KXmlGui5ConfigMigration::MigrationOptions, options);
    QFETCH(bool, oldFileKept);

    const QString appName = QStringLiteral("foo");
    const QStringList uiFileNames{QStringLiteral("appuirc"), QStringLiteral("app2uirc")};

    // generate data dir
    const QString xdgDatahome = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);

    QDir().mkpath(xdgDatahome + QLatin1String("/kxmlgui5/") + appName);
    QVERIFY(QDir(xdgDatahome).exists());

    // set up files in old location
    const QString xdgConfigHome = QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation);
    for (const QString &uifileName : uiFileNames) {
        QFile templateFile(QLatin1String(KXMLGUI5CONFIGMIGRATION_DATA_DIR) + QLatin1Char('/') + uifileName);
        QVERIFY(templateFile.exists());
        // deploy
        const QString oldUiFilePath = xdgDatahome + QLatin1String("/kxmlgui5/") + appName + QLatin1Char('/') + uifileName;
        QVERIFY(templateFile.copy(oldUiFilePath));

        // new not yet existing
        const QString newUiFilePath = xdgConfigHome + QLatin1Char('/') + appName + QLatin1String("/kxmlgui6/") + uifileName;
        QVERIFY(!QFile::exists(newUiFilePath));
    }

    // do migration
    QVERIFY(KXmlGui5ConfigMigration::migrate(appName, uiFileNames, options));

    // check files in new location
    for (const QString &uifileName : uiFileNames) {
        const QString newUiFilePath = xdgConfigHome + QLatin1Char('/') + appName + QLatin1String("/kxmlgui6/") + uifileName;
        QFile uiFile(newUiFilePath);
        QVERIFY(uiFile.exists());

        const QString oldUiFilePath = xdgDatahome + QLatin1String("/kxmlgui5/") + appName + QLatin1Char('/') + uifileName;
        QCOMPARE(QFile::exists(oldUiFilePath), oldFileKept);
    }
}

QTEST_GUILESS_MAIN(KXmlGui5ConfigMigrationTest)

#include "kxmlgui5configmigrationtest.moc"
