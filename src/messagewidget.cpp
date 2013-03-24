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
 
#include "messagewidget.h"

MessageWidget::MessageWidget(QWidget *parent) : QTextEdit(parent)
{
	setReadOnly(true);
}

QToolBar *MessageWidget::getToolBar(QDockWidget *dock)
{
	mDockWidget = dock;
	mToolBar = new QToolBar("Message Tools", this);
	mToolBar->addWidget(new QLabel("Messages", this));
	mToolBar->addAction("Clear", this, SLOT(clear()));
	mToolBar->addWidget(new QSplitter(Qt::Horizontal, this));
	
	QAction *fAction = new QAction(style()->standardIcon(QStyle::SP_TitleBarNormalButton), "Float", this);
	connect(fAction, SIGNAL(triggered()), SLOT(toggleFloat()));
	
	mToolBar->addAction(style()->standardIcon(QStyle::SP_TitleBarNormalButton), "Float", this, SLOT(toggleFloat()));
	mToolBar->addAction(style()->standardIcon(QStyle::SP_TitleBarCloseButton), "Close", dock, SLOT(close()));
	
	return mToolBar;
}

void MessageWidget::insertMessage(const QString &timestamp, const QString &device, const QString &name, const QString &message)
{
	QString text = timestamp + ", " + device + "." + name + ", \"" + message + "\"\n";
	setPlainText(text + toPlainText());	
}

void MessageWidget::toggleFloat()
{
	if (mDockWidget)
		mDockWidget->setFloating(!mDockWidget->isFloating());
}
