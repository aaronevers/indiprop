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

QTextStream qout(stdout);

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("indiprop");
    app.setApplicationVersion("1.03");
    app.setOrganizationName("indiprop.googlecode.com");
    app.setOrganizationDomain("indiprop.googlecode.com");

    QMap<QString, QString> argm;
    QStringList args = qApp->arguments();
    for (int i = 0; i < args.size(); i++)
        if (args.at(i).startsWith("--"))
            argm[args.at(i).section("=", 0, 0).mid(2)] = args.at(i).section("=", 1);

    if (args.contains("-h") || args.contains("--help"))
    {
        qout << qApp->applicationName() << " " << qApp->applicationVersion() << Qt::endl;
        qout << "Usage: " << qApp->applicationName() << " [options]" << Qt::endl;
        qout << "Where [options] are the following:" << Qt::endl;
        qout << "  -h|--help                Prints this help statement." << Qt::endl;
        qout << "  --host=<host[:port]>     Overrides the cached host string." << Qt::endl;
        qout << "  --clear-cache            Clears all cached settings." << Qt::endl;
        qout << "  --xml                    Shows xml properties via stdout." << Qt::endl;
        qout << Qt::endl;
        return false;
    }

    MainWindow mainWindow(argm);
    mainWindow.show();

    return app.exec();
}
