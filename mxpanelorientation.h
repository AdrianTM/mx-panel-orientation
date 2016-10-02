/**********************************************************************
 *  mxpanelorientation.h
 **********************************************************************
 * Copyright (C) 2015 MX Authors
 *
 * Authors: Adrian
 *          MX & MEPIS Community <http://forum.mepiscommunity.org>
 *
 * This file is part of mx-panel-orientation.
 *
 * mx-panel-orientation is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * mx-panel-orientation is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with mx-panel-orientation.  If not, see <http://www.gnu.org/licenses/>.
 **********************************************************************/


#ifndef MXPANELORIENTATION_H
#define MXPANELORIENTATION_H

#include <QMessageBox>
#include <QProcess>
#include <QFile>

namespace Ui {
class mxpanelorientation;
}

struct Result {
    int exitCode;
    QString output;
};


class mxpanelorientation : public QDialog
{
    Q_OBJECT

protected:
    QProcess *proc;
    QTimer *timer;

public:
    explicit mxpanelorientation(QWidget *parent = 0);
    ~mxpanelorientation();

    Result runCmd(QString cmd);
    QString getVersion(QString name);

    QString version;
    QString output;

    void setup();
    void setupuiselections();
    void fliptohorizontal();
    void fliptovertical();
    void backupPanel();
    void restoreDefaultPanel();
    void restoreBackup();
    void message();

public slots:


private slots:
    void on_buttonApply_clicked();
    void on_buttonAbout_clicked();
    void on_buttonHelp_clicked();

private:
    Ui::mxpanelorientation *ui;
};


#endif // MXSNAPSHOT_H

