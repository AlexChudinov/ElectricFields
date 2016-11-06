#ifndef QACTIONSLIST_H
#define QACTIONSLIST_H

#include <QAction>
#include <QList>

/**
 * List of all actions in pogramm
 */
static QList<QAction*> actions_list;

enum ACTION_ID
{
    FILE_OPEN_ACTION_ID = 0
};

#endif // QACTIONSLIST_H
