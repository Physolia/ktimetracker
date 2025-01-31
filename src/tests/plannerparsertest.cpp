/*
 * Copyright (c) 2019 Alexander Potashev <aspotashev@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QTest>

#include "helpers.h"
#include "model/task.h"
#include "model/tasksmodel.h"
#include "taskview.h"
#include "widgets/taskswidget.h"

class PlannerParserTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testEmpty();
    void testAtTopLevel();
    void testAtSubTask();
};

void PlannerParserTest::testEmpty()
{
    auto *taskView = createTaskView(this, false);
    QVERIFY(taskView);

    taskView->importPlanner(QFINDTESTDATA("data/kitchen.planner"));

    auto *model = taskView->storage()->tasksModel();

    QCOMPARE(model->getAllItems().size(), 21);

    QCOMPARE(model->topLevelItemCount(), 4);
    QCOMPARE(dynamic_cast<Task *>(model->topLevelItem(0))->name(), QStringLiteral("Initiation"));

    auto *closureTask = dynamic_cast<Task *>(model->topLevelItem(3));
    QCOMPARE(closureTask->name(), QStringLiteral("Closure"));
    QCOMPARE(closureTask->childCount(), 3);
}

void PlannerParserTest::testAtTopLevel()
{
    auto *taskView = createTaskView(this, false);
    QVERIFY(taskView);

    Task *task1 = taskView->addTask(QStringLiteral("1"));
    QVERIFY(task1);
    taskView->tasksWidget()->setCurrentIndex(taskView->storage()->tasksModel()->index(task1, 0));

    taskView->importPlanner(QFINDTESTDATA("data/kitchen.planner"));

    auto *model = taskView->storage()->tasksModel();

    QCOMPARE(model->getAllItems().size(), 22);

    QCOMPARE(model->topLevelItemCount(), 5);
    QCOMPARE(dynamic_cast<Task *>(model->topLevelItem(0))->name(), QStringLiteral("1"));
    QCOMPARE(dynamic_cast<Task *>(model->topLevelItem(1))->name(), QStringLiteral("Initiation"));

    auto *closureTask = dynamic_cast<Task *>(model->topLevelItem(4));
    QCOMPARE(closureTask->name(), QStringLiteral("Closure"));
    QCOMPARE(closureTask->childCount(), 3);
}

void PlannerParserTest::testAtSubTask()
{
    auto *taskView = createTaskView(this, false);
    QVERIFY(taskView);

    Task *task1 = taskView->addTask(QStringLiteral("1"));
    QVERIFY(task1);
    Task *task2 = taskView->addTask(QStringLiteral("2"), QString(), 0, 0, QVector<int>(0, 0), task1);
    QVERIFY(task2);
    taskView->tasksWidget()->setCurrentIndex(taskView->storage()->tasksModel()->index(task2, 0));

    taskView->importPlanner(QFINDTESTDATA("data/kitchen.planner"));

    auto *model = taskView->storage()->tasksModel();

    QCOMPARE(model->getAllItems().size(), 23);

    QCOMPARE(model->topLevelItemCount(), 1);
    QCOMPARE(task1->childCount(), 5);
    QCOMPARE(dynamic_cast<Task *>(task1->child(0))->name(), QStringLiteral("2"));
    QCOMPARE(dynamic_cast<Task *>(task1->child(1))->name(), QStringLiteral("Initiation"));

    auto *closureTask = dynamic_cast<Task *>(task1->child(4));
    QCOMPARE(closureTask->name(), QStringLiteral("Closure"));
    QCOMPARE(closureTask->childCount(), 3);
}

QTEST_MAIN(PlannerParserTest)

#include "plannerparsertest.moc"
