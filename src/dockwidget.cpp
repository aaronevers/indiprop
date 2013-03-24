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
 
#include "dockwidget.h"

class ToolButton : public QToolButton
{
public:
	ToolButton(const QIcon &icon, const QString &text, QObject *parent):
		icon(true),
		action(new QAction(icon, text, parent))
	{
		setDefaultAction(action);
	}
	
	ToolButton(const QString &text, QObject *parent):
		icon(false),
		action(new QAction(text, parent))
	{
		setDefaultAction(action);
	}

	virtual QSize sizeHint() const
	{
		if (icon)
		{
			int size = style()->pixelMetric(QStyle::PM_SmallIconSize, 0, this);
			return QSize(size, size);
		}
		
		return QToolButton::sizeHint();
	}
	
	bool icon;	
	QAction *action;
};

DockWidget::DockWidget(const QString &name, QMainWindow *parent) : QDockWidget(name, parent), mw(parent)
{
	setObjectName(name);

	mToolBar = new QToolBar(name);
	mToolBar->addWidget(new QLabel(name));	
	mAction = mToolBar->addWidget(new QSplitter(Qt::Horizontal));	

	ToolButton *fb = new ToolButton(style()->standardIcon(QStyle::SP_TitleBarNormalButton), "Float", this);
	connect(fb->action, SIGNAL(triggered()), SLOT(toggleFloat()));
	mToolBar->addWidget(fb);
	
	ToolButton *cb = new ToolButton(style()->standardIcon(QStyle::SP_TitleBarCloseButton), "Close", this);
	connect(cb->action, SIGNAL(triggered()), SLOT(close()));
	mToolBar->addWidget(cb);
	
	setTitleBarWidget(mToolBar);

	mToolBar->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(mToolBar, SIGNAL(customContextMenuRequested(const QPoint &)), SLOT(customContextMenuRequest(const QPoint &)));
}

void DockWidget::insertToolAction(const QString & text, QObject * receiver, const char * member)
{
	ToolButton *b = new ToolButton(text, receiver);
	connect(b->action, SIGNAL(triggered()), receiver, member);
	mToolBar->insertWidget(mAction, b);
}

void DockWidget::toggleFloat()
{
	setFloating(!isFloating());
}

void DockWidget::customContextMenuRequest(const QPoint &p)
{
	mw->createPopupMenu()->exec(mToolBar->mapToGlobal(p));
}
