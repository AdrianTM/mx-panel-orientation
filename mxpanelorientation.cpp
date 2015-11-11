/**********************************************************************
 *  mxpanelorientation.cpp
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
    ui->buttonCancel->setEnabled(true);
    checkBackup();
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
    if (ui->radioHorizontalPanel->isChecked()) {
        backupPanel();
        // copy template files
        system("cp -vR /usr/local/share/appdata/panels/horizontal/panel ~/.config/xfce4");
        system("cp -v /usr/local/share/appdata/panels/horizontal/xfce4-panel.xml ~/.config/xfce4/xfconf/xfce-perchannel-xml/xfce4-panel.xml");
        QMessageBox::information(0, tr("Panel settings"),
                                 tr(" Your current panel settings have been backed up in a hidden folder called .restore in your home folder (~/.restore/)"));

    } else if (ui->radioVerticalPanel->isChecked()) {
        backupPanel();
        // copy template files
        system("cp -vR /usr/local/share/appdata/panels/vertical/panel ~/.config/xfce4");
        system("cp -v /usr/local/share/appdata/panels/vertical/xfce4-panel.xml ~/.config/xfce4/xfconf/xfce-perchannel-xml/xfce4-panel.xml");
        QMessageBox::information(0, tr("Panel settings"),
                                 tr(" Your current panel settings have been backed up in a hidden folder called .restore in your home folder (~/.restore/)"));
    } else if (ui->radioRestoreBackup->isChecked()) {
        system("cp -vR ~/.restore/.config/xfce4/panel ~/.config/xfce4");
        system("cp -v ~/.restore/.config/xfce4/xfconf/xfce-perchannel-xml/xfce4-panel.xml ~/.config/xfce4/xfconf/xfce-perchannel-xml");

    }
    checkBackup();
    system("pkill xfconfd; xfce4-panel -r");
}


// About button clicked
void mxpanelorientation::on_buttonAbout_clicked()
{
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
}

// Help button clicked
void mxpanelorientation::on_buttonHelp_clicked()
{
    QString cmd = QString("mx-viewer http://www.mepiscommunity.org/user_manual_mx15/mxum.html#test '%1'").arg(tr("MX Panel Orientation"));
    system(cmd.toUtf8());
}

// backs up the current panel
void mxpanelorientation::backupPanel()
{
    system("mkdir -p ~/.restore/.config/xfce4");
    system("mkdir -p ~/.restore/.config/xfce4/xfconf/xfce-perchannel-xml");
    system("cp -vR ~/.config/xfce4/panel ~/.restore/.config/xfce4");
    system("cp -v ~/.config/xfce4/xfconf/xfce-perchannel-xml/xfce4-panel.xml ~/.restore/.config/xfce4/xfconf/xfce-perchannel-xml/");
}

// enable restore backup option if backup is present
void mxpanelorientation::checkBackup()
{
    QString cmd = QString("test -f ~/.restore/.config/xfce4/xfconf/xfce-perchannel-xml/xfce4-panel.xml");
    if (system(cmd.toUtf8()) == 0) {
        ui->radioRestoreBackup->setEnabled(true);
    } else {
        ui->radioRestoreBackup->setEnabled(false);
    }
}
