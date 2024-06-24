/***********************************************************************
 * Copyright © 2012 Aaron Evers
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***********************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include "indiclient.h"
#include "treewidget.h"
#include "messagewidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT;

public:
    MainWindow(const QMap<QString, QString> &argm);

private:
    IndiClient mClient;
    QToolBar *mToolbar;
    QLineEdit *mHostnameLineEdit;
    QTreeWidget *mTreeWidget;
    QAction *mSexagesimal;
    MessageWidget *mMessageWidget;

    QMap<QString, TreeItem*> mTreeWidgetItems;

    bool mXml;


protected:
    virtual void closeEvent(QCloseEvent *);
    virtual QMenu *createPopupMenu();

private slots:
    void propertyUpdated(QDomDocument);
    void socketConnect();
    void sexagesimalToggled();
    void customContextMenuRequest(const QPoint &);
};

#endif
