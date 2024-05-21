/*
    This file is part of the KDE project
    SPDX-FileCopyrightText: 2006 Peter Simonsson <peter.simonsson@gmail.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "kundoactions.h"

#include <QAction>
#include <QUndoStack>

#include <KLocalizedString>
#include <KStandardAction>
#include <KStandardShortcut>

#include <kactioncollection.h>

QAction *KUndoActions::createRedoAction(QUndoStack *undoStack, KActionCollection *actionCollection, const QString &actionName)
{
    QAction *action = undoStack->createRedoAction(actionCollection);

    if (actionName.isEmpty()) {
        action->setObjectName(KStandardActions::name(KStandardActions::Redo));
    } else {
        action->setObjectName(actionName);
    }

    action->setIcon(QIcon::fromTheme(QStringLiteral("edit-redo")));
    action->setIconText(i18n("Redo"));
    KActionCollection::setDefaultShortcuts(action, KStandardShortcut::redo());

    actionCollection->addAction(action->objectName(), action);

    return action;
}

QAction *KUndoActions::createUndoAction(QUndoStack *undoStack, KActionCollection *actionCollection, const QString &actionName)
{
    QAction *action = undoStack->createUndoAction(actionCollection);

    if (actionName.isEmpty()) {
        action->setObjectName(KStandardActions::name(KStandardActions::Undo));
    } else {
        action->setObjectName(actionName);
    }

    action->setIcon(QIcon::fromTheme(QStringLiteral("edit-undo")));
    action->setIconText(i18n("Undo"));
    KActionCollection::setDefaultShortcuts(action, KStandardShortcut::undo());

    actionCollection->addAction(action->objectName(), action);

    return action;
}
