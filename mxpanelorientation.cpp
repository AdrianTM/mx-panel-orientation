/**********************************************************************
 *  mxpanelorientation.cpp
 **********************************************************************
 * Copyright (C) 2015 MX Authors
 *
 * Authors: Adrian
 *          Dophin Oracle
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


#include "mxpanelorientation.h"
#include "ui_mxpanelorientation.h"

#include <QDebug>

mxpanelorientation::mxpanelorientation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::mxpanelorientation)
{
    ui->setupUi(this);
    setup();
}

mxpanelorientation::~mxpanelorientation()
{
    delete ui;
}

// setup versious items first time program runs
void mxpanelorientation::setup()
{
    version = getVersion("mx-panel-orientation");
    this->setWindowTitle(tr("MX Panel Orientation"));
    this->adjustSize();
}


// Util function for getting bash command output and error code
Result mxpanelorientation::runCmd(QString cmd)
{
    QEventLoop loop;
    proc = new QProcess(this);
    proc->setReadChannelMode(QProcess::MergedChannels);
    connect(proc, SIGNAL(finished(int)), &loop, SLOT(quit()));
    proc->start("/bin/bash", QStringList() << "-c" << cmd);
    loop.exec();
    disconnect(proc, 0, 0, 0);
    Result result = {proc->exitCode(), proc->readAll().trimmed()};
    delete proc;
    return result;
}

// Get version of the program
QString mxpanelorientation::getVersion(QString name)
{
    QString cmd = QString("dpkg -l %1 | awk 'NR==6 {print $3}'").arg(name);
    return runCmd(cmd).output;
}


//// slots ////


// Apply button clicked
void mxpanelorientation::on_buttonApply_clicked()
{
    //read in plugin ID's
    if (ui->radioHorizontalPanel->isChecked()) {
        QString file_content;
        QStringList pluginIDs;
        file_content = runCmd("xfconf-query -c xfce4-panel -p /panels/panel-1/plugin-ids | grep -v Value | grep -v ^$").output;
        pluginIDs = file_content.split("\n");
        qDebug() << pluginIDs;


        // figure out systrayID, and tasklistID

        QString tasklistID = runCmd("grep tasklist ~/.config/xfce4/xfconf/xfce-perchannel-xml/xfce4-panel.xml").output;
        tasklistID=tasklistID.remove("\"").section("-",1,1).section(" ",0,0);
        qDebug() << "tasklist: " << tasklistID;

        QString systrayID = runCmd("grep systray ~/.config/xfce4/xfconf/xfce-perchannel-xml/xfce4-panel.xml").output;
        systrayID=systrayID.remove("\"").section("-",1,1).section(" ",0,0);
        qDebug() << "systray: " << systrayID;

        //get tasklist index in list
        int tasklistindex = pluginIDs.indexOf(tasklistID);
        qDebug() << "tasklistIDindex 1" << tasklistindex;

        //check next plugin in list to see if its an expanding separator
        int expsepindex = tasklistindex + 1;
        qDebug() << "expsepindex" << expsepindex;
        QString expsepID = pluginIDs.value(expsepindex);
        qDebug() << "expsepID to test" << expsepID;
        QString test = runCmd("xfconf-query -c xfce4-panel -p /plugins/plugin-" + expsepID + "/expand").output;
        qDebug() << "test parm" << test;


        //move the notification area (systray) to above window buttons (tasklist) in the list

        pluginIDs.removeAll(systrayID);
        tasklistindex = pluginIDs.indexOf(tasklistID);
        qDebug() << "tasklistIDindex 2" << tasklistindex;
        pluginIDs.insert(tasklistindex, systrayID);
        qDebug() << "reordered list" << pluginIDs;

        //move the expanding separator

        if (test == "true") {
            pluginIDs.removeAll(expsepID);
            tasklistindex = pluginIDs.indexOf(tasklistID);
            qDebug() << "tasklistIDindex 2" << tasklistindex;
            pluginIDs.insert(tasklistindex, expsepID);
            qDebug() << "reordered list" << pluginIDs;
        }

        //now reverse the list

        std::reverse(pluginIDs.begin(), pluginIDs.end());
        qDebug() << "reversed list" << pluginIDs;

        //now build xfconf command

        QStringListIterator changeIterator(pluginIDs);
        QString cmdstring;
        while (changeIterator.hasNext()) {
            QString value = changeIterator.next();
            cmdstring = QString(cmdstring + "-s " + value + " ");
            qDebug() << cmdstring;
        }

        //flip the panel plugins

        runCmd("xfconf-query -c xfce4-panel -p /panels/panel-1/plugin-ids " + cmdstring);

        //change orientation to horizontal

        system("xfconf-query -c xfce4-panel -p /panels/panel-1/mode -s 0");

        //change mode of window buttons

        runCmd("xfconf-query -c xfce4-panel -p /plugins/plugin-" + tasklistID + "/show-labels -s true");

        //restart xfce4-panel

        system("xfce4-panel -r");


    }
    if (ui->radioVerticalPanel->isChecked()) {
        QString file_content;
        QStringList pluginIDs;
        file_content = runCmd("xfconf-query -c xfce4-panel -p /panels/panel-1/plugin-ids | grep -v Value | grep -v ^$").output;
        pluginIDs = file_content.split("\n");
        qDebug() << pluginIDs;


        // figure out whiskerID, appmenuID, systrayID, tasklistID, and pagerID

        QString tasklistID = runCmd("grep tasklist ~/.config/xfce4/xfconf/xfce-perchannel-xml/xfce4-panel.xml").output;
        tasklistID=tasklistID.remove("\"").section("-",1,1).section(" ",0,0);
        qDebug() << "tasklist: " << tasklistID;

        QString whiskerID = runCmd("grep whisker ~/.config/xfce4/xfconf/xfce-perchannel-xml/xfce4-panel.xml").output;
        whiskerID=whiskerID.remove("\"").section("-",1,1).section(" ",0,0);
        qDebug() << "whisker: " << whiskerID;

        QString systrayID = runCmd("grep systray ~/.config/xfce4/xfconf/xfce-perchannel-xml/xfce4-panel.xml").output;
        systrayID=systrayID.remove("\"").section("-",1,1).section(" ",0,0);
        qDebug() << "systray: " << systrayID;

        QString pagerID = runCmd("grep pager ~/.config/xfce4/xfconf/xfce-perchannel-xml/xfce4-panel.xml").output;
        pagerID=pagerID.remove("\"").section("-",1,1).section(" ",0,0);
        qDebug() << "pager: " << pagerID;

        QString appmenuID = runCmd("grep applicationmenu ~/.config/xfce4/xfconf/xfce-perchannel-xml/xfce4-panel.xml").output;
        appmenuID=appmenuID.remove("\"").section("-",1,1).section(" ",0,0);
        qDebug() << "appmenuID: " << appmenuID;

        //get tasklist index in list
        int tasklistindex = pluginIDs.indexOf(tasklistID);
        qDebug() << "tasklistIDindex 1" << tasklistindex;

        //check next plugin in list to see if its an expanding separator
        int expsepindex = tasklistindex + 1;
        qDebug() << "expsepindex" << expsepindex;
        QString expsepID = pluginIDs.value(expsepindex);
        qDebug() << "expsepID to test" << expsepID;
        QString testexpandsep = runCmd("xfconf-query -c xfce4-panel -p /plugins/plugin-" + expsepID + "/expand").output;
        qDebug() << "test parm" << testexpandsep;


        //move the notification area (systray) to an appropriate area.

        //1.  determine if menu is present, place in front of menu

        QString switchID;
        if (whiskerID != "") {
            switchID = whiskerID;
            qDebug() << "switchID whisker: " << switchID;
        } else {
            if (appmenuID != "") {
                switchID = appmenuID;
                qDebug() << "switchID appmenu: " << switchID;
            }
        }

        //2.  if so, check second plugin is separator, if so place in front of separator

        if (switchID != "") {
            QString test = runCmd("xfconf-query -c xfce4-panel -p /plugins/plugin-" + pluginIDs.value(1)).output;
            if (test == "separator") {
                qDebug() << "test parm" << test;
                switchID = pluginIDs.value(1);
                qDebug() << "switchID sep: " << switchID;
            }
        }

        //3.  if so, check third plugin is pager.  if so, place tasklist in front of pager

        if (switchID != ""){
            QString test = runCmd("xfconf-query -c xfce4-panel -p /plugins/plugin-" + pluginIDs.value(2)).output;
            if (test == "pager") {
                qDebug() << "test parm" << test;
                switchID = pluginIDs.value(2);
                qDebug() << "switchID pager: " << switchID;
            }
        }

        // give a default value that is sane but might not be correct

        if (switchID == "") {
            switchID = pluginIDs.value(1);
            qDebug() << "switchID default: " << switchID;
        }


        //4.  move the systray

        pluginIDs.removeAll(systrayID);
        int switchindex = pluginIDs.indexOf(switchID) + 1;
        qDebug() << "switchindex" << switchindex;
        pluginIDs.insert(switchindex, systrayID);
        qDebug() << "reordered list" << pluginIDs;

        //move the expanding separator

        if (testexpandsep == "true") {
            pluginIDs.removeAll(expsepID);
            tasklistindex = pluginIDs.indexOf(tasklistID);
            qDebug() << "tasklistIDindex 2" << tasklistindex;
            pluginIDs.insert(tasklistindex, expsepID);
            qDebug() << "reordered list" << pluginIDs;
        }

        //now reverse the list

        std::reverse(pluginIDs.begin(), pluginIDs.end());
        qDebug() << "reversed list" << pluginIDs;

        //now build xfconf command

        QStringListIterator changeIterator(pluginIDs);
        QString cmdstring;
        while (changeIterator.hasNext()) {
            QString value = changeIterator.next();
            cmdstring = QString(cmdstring + "-s " + value + " ");
            qDebug() << cmdstring;
        }

        //flip the panel plugins

        runCmd("xfconf-query -c xfce4-panel -p /panels/panel-1/plugin-ids " + cmdstring);

        //change orientation to vertical

        system("xfconf-query -c xfce4-panel -p /panels/panel-1/mode -s 2");

        //change mode of window buttons

        runCmd("xfconf-query -c xfce4-panel -p /plugins/plugin-" + tasklistID + "/show-labels -s false");

        //restart xfce4-panel

        system("xfce4-panel -r");

    }

}

// About button clicked
void mxpanelorientation::on_buttonAbout_clicked()
{
    this->hide();
    QMessageBox msgBox(QMessageBox::NoIcon,
                       tr("About MX Panel Orientation"), "<p align=\"center\"><b><h2>" +
                       tr("MX Panel Orientation") + "</h2></b></p><p align=\"center\">" + tr("Version: ") + version + "</p><p align=\"center\"><h3>" +
                       tr("Program for changing the Xfce panel orientation in MX Linux") +
                       "</h3></p><p align=\"center\"><a href=\"http://www.mepiscommunity.org/mx\">http://www.mepiscommunity.org/mx</a><br /></p><p align=\"center\">" +
                       tr("Copyright (c) MX Linux") + "<br /><br /></p>", 0, this);
    msgBox.addButton(tr("Cancel"), QMessageBox::AcceptRole); // because we want to display the buttons in reverse order we use counter-intuitive roles.
    msgBox.addButton(tr("License"), QMessageBox::RejectRole);
    if (msgBox.exec() == QMessageBox::RejectRole) {
        system("mx-viewer file:///usr/share/doc/mx-panel-orientation/license.html '" + tr("MX Panel Orientation").toUtf8() + " " + tr("License").toUtf8() + "'");
    }
    this->show();
}

// Help button clicked
void mxpanelorientation::on_buttonHelp_clicked()
{
    this->hide();
    QString cmd = QString("mx-viewer http://www.mepiscommunity.org/wiki/help-files/help-mx-panel-orientation '%1'").arg(tr("MX Panel Orientation"));
    system(cmd.toUtf8());
    this->show();
}
