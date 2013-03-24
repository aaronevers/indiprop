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

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	app.setApplicationName("indiprop");
	app.setApplicationVersion("1.02");
	app.setOrganizationName("indiprop.googlecode.com");
	app.setOrganizationDomain("indiprop.googlecode.com");

    QMap<QString, QString> argm; 
    QStringList args = qApp->arguments();
    for (int i = 0; i < args.size(); i++)
        if (args.at(i).startsWith("--"))
            argm[args.at(i).section("=", 0, 0).mid(2)] = args.at(i).section("=", 1);
	
    if (args.contains("-h") || args.contains("--help"))
    {
        qout << qApp->applicationName() << " " << qApp->applicationVersion() << endl;
        qout << "Usage: " << qApp->applicationName() << " [options]" << endl;
        qout << "Where [options] are the following:" << endl;
        qout << "  -h|--help                Prints this help statement." << endl;
        qout << "  --host=<host[:port]>     Overrides the cached host string." << endl;
        qout << "  --clear-cache            Clears all cached settings." << endl;
        qout << endl;
        return false;
    }

	MainWindow mainWindow(argm);
	mainWindow.show();

	return app.exec();
}

