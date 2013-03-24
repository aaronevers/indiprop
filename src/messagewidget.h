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

#ifndef MESSAGEWIDGET_H
#define MESSAGEWIDGET_H

#include <QtGui>

class MessageWidget : public QTextEdit
{
	Q_OBJECT;
	
public:
	MessageWidget(QWidget *parent = 0);

	void insertMessage(const QString &timestamp, const QString &device, const QString &name, const QString &message);

	QToolBar *getToolBar(QDockWidget *);
	QDockWidget *mDockWidget;

private:	
	QToolBar *mToolBar;

public slots:
	void toggleFloat();
	
signals:

};

#endif
