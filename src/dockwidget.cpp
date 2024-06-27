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

class ToolBar : public QToolBar
{
public:
	ToolBar(const QString &name) : QToolBar(name), mLabel(new QLabel(name))
	{
		addWidget(mLabel);	

		QFont f = font();
		f.setPointSize(f.pointSize() - 2);
		QLineEdit e;
		e.setFont(f);
		size = e.sizeHint();
	}
	
	virtual QSize sizeHint() const
	{
		return size;
	}
	
private:
	QLabel *mLabel;
	QSize size;
};

DockWidget::DockWidget(const QString &name, QMainWindow *parent) : QDockWidget(name, parent), mw(parent)
{
	setObjectName(name);

	mToolBar = new ToolBar(name);
    mToolBar->addWidget(new QSplitter(Qt::Horizontal));

    ToolButton *fb = new ToolButton(style()->standardIcon(QStyle::SP_TitleBarNormalButton), "Float", this);
    connect(fb->action, &QAction::triggered, this, &DockWidget::toggleFloat);
	mAction = mToolBar->addWidget(fb);
	
	ToolButton *cb = new ToolButton(style()->standardIcon(QStyle::SP_TitleBarCloseButton), "Close", this);
    connect(cb->action, &QAction::triggered, this, &DockWidget::close);
	mToolBar->addWidget(cb);
	
	setTitleBarWidget(mToolBar);

	mToolBar->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(mToolBar, &ToolBar::customContextMenuRequested, this, &DockWidget::customContextMenuRequest);
}

void DockWidget::insertSeparator()
{
	mToolBar->insertSeparator(mAction);
}

void DockWidget::insertWidget(QWidget *w)
{
	QFont f = font();
	f.setPointSize(f.pointSize() - 2);
	w->setFont(f);
	
	mToolBar->insertWidget(mAction, w);
}

QAction *DockWidget::insertToolAction(const QString &text, QObject *receiver)
{
	QFont f = font();
	f.setPointSize(f.pointSize() - 2);

	ToolButton *b = new ToolButton(text, receiver);
	b->setFont(f);
	mToolBar->insertWidget(mAction, b);

    return b->action;
}

void DockWidget::toggleFloat()
{
	setFloating(!isFloating());
}

void DockWidget::customContextMenuRequest(const QPoint &p)
{
	mw->createPopupMenu()->exec(mToolBar->mapToGlobal(p));
}
