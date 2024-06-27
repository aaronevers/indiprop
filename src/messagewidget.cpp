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
 
#include "messagewidget.h"

MessageWidget::MessageWidget(QMainWindow *parent) : 
	DockWidget("Messages", parent)
{
	QSettings settings;
	
	mTextEdit = new QTextEdit();
	mTextEdit->setReadOnly(true);
	setWidget(mTextEdit);

	insertWidget(new QLabel("Regex:"));
	mRegex = settings.value("MessageToolbar/Regex").toString();	

	mRegexEdit = new QLineEdit(mRegex);
	insertWidget(mRegexEdit);
	insertSeparator();

    connect(mRegexEdit, &QLineEdit::editingFinished, this, &MessageWidget::updateRegex);

	insertWidget(new QLabel("Max Lines:"));
	mMaxLines = settings.value("MessageToolbar/MaxLines", "50").toInt();	
	
	mLineCountEdit = new QLineEdit(QString("%1").arg(mMaxLines));
	mLineCountEdit->setMaximumWidth(40);
	insertWidget(mLineCountEdit);
	insertSeparator();
	
    connect(mLineCountEdit, &QLineEdit::editingFinished, this, &MessageWidget::updateMaxLines);

    QAction *a = insertToolAction("Clear", this);
    connect(a, &QAction::triggered, this, &MessageWidget::clear);
}

MessageWidget::~MessageWidget()
{
	QSettings settings;
	settings.setValue("MessageToolbar/MaxLines", mMaxLines);
	settings.setValue("MessageToolbar/Regex", mRegex);
}

void MessageWidget::insertMessage(const QString &timestamp, const QString &device, const QString &name, const QString &message)
{
	QString text = timestamp + ", " + device + "." + name + ", \"" + message + "\"";
	mStringList.prepend(text);
	
	if (mRegex.size())
        mStringList = mStringList.filter(QRegularExpression(mRegex));
	
	while (mStringList.size() > mMaxLines)
		mStringList.removeLast();
	
	mTextEdit->setText(mStringList.join("\n"));	
}

void MessageWidget::clear()
{
	mStringList.clear();
	mTextEdit->setText(mStringList.join("\n"));	
}

void MessageWidget::updateRegex()
{
	mRegex = mRegexEdit->text();

	if (mRegex.size())
        mStringList = mStringList.filter(QRegularExpression(mRegex));

	mTextEdit->setText(mStringList.join("\n"));	
}

void MessageWidget::updateMaxLines()
{
	mMaxLines = mLineCountEdit->text().toInt();

	while (mStringList.size() > mMaxLines)
		mStringList.removeLast();

	mTextEdit->setText(mStringList.join("\n"));	
}

