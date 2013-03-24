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

#ifndef TREEWIDGET_H
#define TREEWIDGET_H

#include <QtGui>
#include <QtXml>

class TreeItem : public QWidget
{
	Q_OBJECT;
	
public:
	TreeItem();
	
	QTreeWidgetItem *widget;
	QString device;
	QString vector;
	QString type;
	QString property;
	QString format;
	QString text;
	QString perm;
	QString rule;
	double min;
	double max;
	QButtonGroup *group;
	QAbstractButton *button;
	QLineEdit *edit;
	bool disabled;
	
	QList<TreeItem*> children;
	
public slots:
	void groupClicked(QAbstractButton *);
	void editClicked();
	
signals:
	void propertyUpdated(QDomDocument);
};

#endif

