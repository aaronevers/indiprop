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

#ifndef TREEITEM_H
#define TREEITEM_H

#include <QtGui>

class TreeItem : public QWidget
{
public:
	TreeItem();
	
	QTreeWidgetItem *widget;
	QString format;
	QString text;
	QString perm;
	QString rule;
	QButtonGroup *group;
	QAbstractButton *button;
};

#endif

