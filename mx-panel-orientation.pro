# **********************************************************************
# * Copyright (C) 2015 MX Authors
# *
# * Authors: Adrian
# *          MX & MEPIS Community <http://forum.mepiscommunity.org>
# *
# * This file is part of mx-panel-orientation.
# *
# * mx-panel-orientation is free software: you can redistribute it and/or modify
# * it under the terms of the GNU General Public License as published by
# * the Free Software Foundation, either version 3 of the License, or
# * (at your option) any later version.
# *
# * mx-panel-orientation is distributed in the hope that it will be useful,
# * but WITHOUT ANY WARRANTY; without even the implied warranty of
# * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# * GNU General Public License for more details.
# *
# * You should have received a copy of the GNU General Public License
# * along with mx-panel-orientation.  If not, see <http://www.gnu.org/licenses/>.
# **********************************************************************/

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mx-panel-orientation
TEMPLATE = app


SOURCES += main.cpp\
        mxpanelorientation.cpp

HEADERS  += mxpanelorientation.h

FORMS    += mxpanelorientation.ui

TRANSLATIONS += translations/mx-panel-orientation_ca.ts \
                translations/mx-panel-orientation_de.ts \
                translations/mx-panel-orientation_el.ts \
                translations/mx-panel-orientation_es.ts \
                translations/mx-panel-orientation_fr.ts \
                translations/mx-panel-orientation_it.ts \
                translations/mx-panel-orientation_ja.ts \
                translations/mx-panel-orientation_nl.ts \
                translations/mx-panel-orientation_pl.ts \
                translations/mx-panel-orientation_ro.ts \
                translations/mx-panel-orientation_ru.ts \
                translations/mx-panel-orientation_sv.ts


