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
	mTreeWidget->setHeaderLabels(QStringList() << "Property" << "Value" << "" << "Label");
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
	
	QMapIterator<QString, TreeItem*> tree(mTreeWidgetItems);
	while (tree.hasNext())
	{
		tree.next();
		if (tree.value()->widget->childCount())
			settings.setValue("TreeWidget/State/" + tree.key(), tree.value()->widget->isExpanded()); 
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
	else if (status == "Ok")
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
			TreeItem *d;				
			if (!mTreeWidgetItems.contains(device))
			{
				d = new TreeItem();
				mTreeWidgetItems[device] = d;
				d->widget = new QTreeWidgetItem(mTreeWidget->invisibleRootItem());
				d->widget->setText(0, device);
				d->widget->setExpanded(settings.value("TreeWidget/State/" + device).toBool());
				d->device = device;
				mTreeWidget->invisibleRootItem()->sortChildren(0, Qt::AscendingOrder);
			}
			
			d = mTreeWidgetItems[device];

			if (element.hasAttribute("name"))
			{
				QString name = element.attribute("name");
				QString type = element.tagName().mid(3);
				QString devicename = device + "/" + name;
				TreeItem *dn;				
				if (!mTreeWidgetItems.contains(devicename))
				{
					dn = new TreeItem();
					mTreeWidgetItems[devicename] = dn;
					dn->widget = new QTreeWidgetItem(d->widget);
					dn->widget->setText(0, name);
					dn->widget->setExpanded(settings.value("TreeWidget/State/" + devicename).toBool());
					dn->widget->setText(3, element.attribute("label"));		
					dn->device = device;
					dn->vector = name;
					d->widget->sortChildren(0, Qt::AscendingOrder);

					dn->rule = element.attribute("rule");
					dn->perm = element.attribute("perm");
					
					if (type == "SwitchVector" && dn->perm.contains("w"))
					{
						dn->group = new QButtonGroup();
						connect(dn, SIGNAL(propertyUpdated(QDomDocument)), &mClient, SLOT(sendProperty(QDomDocument)));
						
						if (dn->rule == "OneOfMany")
							dn->group->setExclusive(true);
						else
							dn->group->setExclusive(false);

						connect(dn->group, SIGNAL(buttonClicked(QAbstractButton *)), dn, SLOT(groupClicked(QAbstractButton *)));
					}
				}

				dn = mTreeWidgetItems[devicename];
				dn->widget->setIcon(0, toIcon(state));
				dn->widget->setToolTip(0, state);
				if (element.hasAttribute("message"))
					dn->widget->setToolTip(0, element.attribute("message"));
				
				QDomElement child;
				for (child = element.firstChildElement(); !child.isNull(); child = child.nextSiblingElement())
				{
					if (child.hasAttribute("name"))
					{
						QString property = child.attribute("name");
						QString devicenameproperty = devicename + "/" + property;

						TreeItem *dnp;
						if (!mTreeWidgetItems.contains(devicenameproperty))
						{
							dnp = new TreeItem();
							mTreeWidgetItems[devicenameproperty] = dnp;
							dnp->widget = new QTreeWidgetItem(dn->widget);
							dnp->widget->setText(0, property);
							dnp->widget->setExpanded(settings.value("TreeWidget/State/" + devicenameproperty).toBool());
							dnp->widget->setText(3, child.attribute("label"));
							dnp->device = device;
							dnp->vector = name;
							dnp->property = property;
							dnp->min = child.attribute("min").toDouble();
							dnp->max = child.attribute("max").toDouble();
							dnp->format = child.attribute("format");
							
							if (dn->group)
							{
								if (dn->rule == "OneOfMany")
									dnp->button = new QRadioButton();
								else  if (dn->rule == "AnyOfMany")
									dnp->button = new QCheckBox();
								else
									dnp->button = new QPushButton(property);

								dnp->button->setToolTip(property);
								dnp->button->setCheckable(true);
								dn->group->addButton(dnp->button);
								mTreeWidget->setItemWidget(dnp->widget, 2, dnp->button);
							}
						}
						
						dnp = mTreeWidgetItems[devicenameproperty];
						dnp->text = child.text().trimmed();
						QString text = dnp->text;

						if (type == "LightVector" || type == "SwitchVector")
						{
							dnp->widget->setIcon(1, toIcon(text));
							dnp->widget->setToolTip(1, text);
							if (dnp->button)
								dnp->button->setChecked((text == "On")?true:false);
						}
						else
						{
							if (type == "NumberVector")
							{
								text = IndiClient::formatNumber(dnp->format, text, mSexagesimal->isChecked());
								if (dnp->min != dnp->max)
								{
									double n = text.toDouble();
									if (n < dnp->min || n > dnp->max)
										dnp->widget->setIcon(0, toIcon("Alert"));
									else
										dnp->widget->setIcon(0, QIcon());
								}
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
	QMapIterator<QString, TreeItem*> tree(mTreeWidgetItems);
	while (tree.hasNext())
	{
		tree.next();
		if (tree.value()->format.endsWith('m'))
			tree.value()->widget->setText(1, IndiClient::formatNumber(tree.value()->format, tree.value()->text, mSexagesimal->isChecked()));
	}
}

