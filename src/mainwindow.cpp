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

#include "mainwindow.h"

MainWindow::MainWindow() : mContextMenu(NULL)
{
	QSettings settings;

	setWindowTitle(qApp->applicationName() + " " + qApp->applicationVersion());
	setObjectName(windowTitle());

	mHostnameLineEdit = new QLineEdit(settings.value("Toolbar/Hostname").toString());
	mToolbar = addToolBar("Show Toolbar");
	mToolbar->setObjectName(mToolbar->windowTitle());
	mToolbar->addWidget(new QLabel("Hostname:"));
	mToolbar->addWidget(mHostnameLineEdit);
	mToolbar->addAction("Connect", this, SLOT(socketConnect()));
	connect(mHostnameLineEdit, SIGNAL(returnPressed()), SLOT(socketConnect()));
	
	mTreeWidget = new QTreeWidget(this);
	mTreeWidget->setObjectName(windowTitle() + " treewidget");
	mTreeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	mTreeWidget->setHeaderLabels(QStringList() << "Property" << "Value" << "Label");
	mTreeWidget->header()->restoreState(settings.value("TreeWidgetHeader/State", mTreeWidget->header()->saveState()).toByteArray());
	setCentralWidget(mTreeWidget);
	connect(mTreeWidget, SIGNAL(customContextMenuRequested(const QPoint &)), SLOT(customContextMenuRequest(const QPoint &)));

	mSexagesimal = new QAction("Show Sexagesimal", this);
	mSexagesimal->setCheckable(true);
	mSexagesimal->setChecked(settings.value("Sexigesimal", true).toBool());
	connect(mSexagesimal, SIGNAL(triggered()), SLOT(sexagesimalToggled()));
	
	restoreState(settings.value("MainWindow/State", saveState()).toByteArray());
	restoreGeometry(settings.value("MainWindow/Geometry", saveGeometry()).toByteArray());
	
	connect(&mClient, SIGNAL(propertyUpdate(QDomDocument)), SLOT(propertyUpdated(QDomDocument)));
	
	if (settings.value("Client/ConnectedOnClose", false).toBool())
		socketConnect();
}

void MainWindow::closeEvent(QCloseEvent *)
{
	QSettings settings;
	
	settings.setValue("MainWindow/Geometry", saveGeometry());
	settings.setValue("MainWindow/State", saveState());
	settings.setValue("Toolbar/Hostname", mHostnameLineEdit->text());
	settings.setValue("TreeWidgetHeader/State", mTreeWidget->header()->saveState());
	settings.setValue("Client/ConnectedOnClose", mClient.connected());
	if (mSexagesimal != NULL)
		settings.setValue("Sexigesimal", mSexagesimal->isChecked());
	
	QMapIterator<QString, TreeItem> tree(mTreeWidgetItems);
	while (tree.hasNext())
	{
		tree.next();
		if (tree.value().widget->childCount())
			settings.setValue("TreeWidget/State/" + tree.key(), tree.value().widget->isExpanded()); 
	}
}

QMenu *MainWindow::createPopupMenu()
{
	if (!mContextMenu)
	{
		mContextMenu = QMainWindow::createPopupMenu();
		mContextMenu->addSeparator();
		mContextMenu->addAction(mSexagesimal);
	}
	
	return mContextMenu;
}

void MainWindow::customContextMenuRequest(const QPoint &p)
{
	createPopupMenu()->exec(mTreeWidget->viewport()->mapToGlobal(p));
}

void MainWindow::socketConnect()
{
	mClient.socketConnect(mHostnameLineEdit->text());
}

QIcon toIcon(const QString &status)
{
	if (status == "On")
		return QIcon(":/led/blue.png");
	if (status == "Ok")
		return QIcon(":/led/green.png");
	else if (status == "Busy")
		return QIcon(":/led/orange.png");
	else if (status == "Alert")
		return QIcon(":/led/red.png");
	return QIcon(":/led/grey.png");
}

void MainWindow::propertyUpdated(QDomDocument doc)
{
	QSettings settings;
	
	QDomElement element = doc.documentElement();
	if (element.hasAttribute("device"))
	{
 		QString state = element.attribute("state");
		QString device = element.attribute("device");
		QString op = element.tagName().left(3);
		if (op == "set" || op == "def")
		{
			QMap<QString, TreeItem>::iterator d = mTreeWidgetItems.find(device);
			if (d == mTreeWidgetItems.end())
			{
				d = mTreeWidgetItems.insert(device, TreeItem());
				d->widget = new QTreeWidgetItem(mTreeWidget->invisibleRootItem());
				d->widget->setText(0, device);
				d->widget->setExpanded(settings.value("TreeWidget/State/" + device).toBool());
				mTreeWidget->invisibleRootItem()->sortChildren(0, Qt::AscendingOrder);
			}

			if (element.hasAttribute("name"))
			{
				QString name = element.attribute("name");
				QString devicename = device + "/" + name;				
				QMap<QString, TreeItem>::iterator dn = mTreeWidgetItems.find(devicename);
				if (dn == mTreeWidgetItems.end())
				{
					dn = mTreeWidgetItems.insert(devicename, TreeItem());
					dn->widget = new QTreeWidgetItem(d->widget);
					dn->widget->setText(0, name);
					dn->widget->setExpanded(settings.value("TreeWidget/State/" + devicename).toBool());
					dn->widget->setText(2, element.attribute("label"));						
					d->widget->sortChildren(0, Qt::AscendingOrder);
				}

				dn->widget->setIcon(0, toIcon(state));
				dn->widget->setToolTip(0, state);
				
				QDomElement child;
				for (child = element.firstChildElement(); !child.isNull(); child = child.nextSiblingElement())
				{
					if (child.hasAttribute("name"))
					{
						QString property = child.attribute("name");
						QString devicenameproperty = devicename + "/" + property;				
						QMap<QString, TreeItem>::iterator dnp = mTreeWidgetItems.find(devicenameproperty);
						if (dnp == mTreeWidgetItems.end())
						{
							dnp = mTreeWidgetItems.insert(devicenameproperty, TreeItem());
							dnp->widget = new QTreeWidgetItem(dn->widget);
							dnp->widget->setText(0, property);
							dnp->widget->setExpanded(settings.value("TreeWidget/State/" + devicenameproperty).toBool());
							dnp->widget->setText(2, child.attribute("label"));						
							dn->widget->sortChildren(0, Qt::AscendingOrder);
						}
						
						dnp->text = child.text().trimmed();
						QString text = dnp->text;

						QString type = child.tagName().mid(3);
						if (type == "Light" || type == "Switch")
						{
							dnp->widget->setIcon(1, toIcon(text));
							dnp->widget->setToolTip(1, text);
						}
						else
						{
							if (type == "Number")
							{
								if (child.hasAttribute("format"))
									dnp->format = child.attribute("format");

								text = IndiClient::formatNumber(dnp->format, text, mSexagesimal->isChecked());
							}

							dnp->widget->setText(1, text);
						}
					}
				}
			}
			
			state = "Idle";

			int i;
			for (i = 0; state != "Alert" && i < d->widget->childCount(); i++)
			{
				if (state != "Alert" && d->widget->child(i)->toolTip(0) == "Alert")
					state = "Alert";
				else
				{
					if (state != "Busy" && d->widget->child(i)->toolTip(0) == "Busy")
						state = "Busy";
					else
					{
						if (state != "Ok" && d->widget->child(i)->toolTip(0) == "Ok")
							state = "Ok";
					}
				}
			}

			d->widget->setIcon(0, toIcon(state));
			d->widget->setToolTip(0, state);
		}
	}
}

void MainWindow::sexagesimalToggled()
{
	QMapIterator<QString, TreeItem> tree(mTreeWidgetItems);
	while (tree.hasNext())
	{
		tree.next();
		if (tree.value().format.endsWith('m'))
			tree.value().widget->setText(1, IndiClient::formatNumber(tree.value().format, tree.value().text, mSexagesimal->isChecked()));
	}
}

