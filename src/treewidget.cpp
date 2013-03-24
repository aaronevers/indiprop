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
 
#include "treewidget.h"

TreeItem::TreeItem() : widget(0), min(0), max(0), group(0), button(0), edit(0), disabled(false)
{
}

void TreeItem::groupClicked(QAbstractButton *b)
{
	QDomDocument doc("");
	QDomElement switchVector = doc.createElement("newSwitchVector");
	switchVector.setAttribute("device", device);
	switchVector.setAttribute("name", vector);
	QDomElement swtch = doc.createElement("oneSwitch");
	swtch.setAttribute("name", b->toolTip());
	swtch.appendChild(doc.createTextNode(b->isChecked()?"On":"Off"));
	switchVector.appendChild(swtch);
	doc.appendChild(switchVector);
	disabled = true;
	widget->parent()->setDisabled(true);
	emit propertyUpdated(doc);	
}

void TreeItem::editClicked()
{
	QDomDocument doc("");
	QDomElement property = doc.createElement("new" + type + "Vector");
	property.setAttribute("device", device);
	property.setAttribute("name", vector);

	QListIterator<TreeItem*> i(children);
	while (i.hasNext())
	{
		TreeItem *n = i.next();
		QDomElement element = doc.createElement("one" + type);
		element.setAttribute("name", n->property);
		element.appendChild(doc.createTextNode(n->edit->text()));
		property.appendChild(element);
	}
	doc.appendChild(property);
	disabled = true;
	widget->parent()->setDisabled(true);
	emit propertyUpdated(doc);	
}

