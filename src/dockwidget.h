/***********************************************************************
 * Copyright © 2013 Aaron Evers
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

#ifndef DOCKWIDGET_H
#define DOCKWIDGET_H

#include <QtGui>

class DockWidget : public QDockWidget
{
	Q_OBJECT;
	
public:
	DockWidget(const QString &name, QMainWindow *parent = 0);

	void insertSeparator();
	void insertWidget(QWidget *w);
	void insertToolAction(const QString & text, QObject * receiver, const char * member);

private:	
	QToolBar *mToolBar;
	QMainWindow *mw;
	QAction *mAction;

public slots:
	void toggleFloat();
	void customContextMenuRequest(const QPoint &p);

signals:

};

#endif
