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
#include "dockwidget.h"

class MessageWidget : public DockWidget
{
	Q_OBJECT;
	
public:
	MessageWidget(QMainWindow *parent);
	~MessageWidget();

	void insertMessage(const QString &timestamp, const QString &device, const QString &name, const QString &message);

private:	
	QStringList mStringList;
	QTextEdit *mTextEdit;
	
	QString mRegex;	
	QLineEdit *mRegexEdit;

	int mMaxLines;	
	QLineEdit *mLineCountEdit;

public slots:
	void clear();
	void updateRegex();
	void updateMaxLines();
	
signals:

};

#endif
