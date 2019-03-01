#! /usr/bin/env python
# -*- coding: utf-8 -*-
# Copyright (C) 2017-2018  EDF R&D
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
#
# See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#
import os
import py2yacs
import SalomePyQt
import otguibase

def loadYacsPyStudy(pyFile,
                    study_name = "OTStudy",
                    model_name = "PhysicalModel",
                    yacs_file = ""):
  """Open OPENTURNS module and create a study using a YACS model from a python
     file.
  """
  if not yacs_file:
    yacs_file = os.path.join(os.path.dirname(pyFile), "yacs.xml")
  errors = py2yacs.main(pyFile, yacs_file)
  if not errors:
    with open(pyFile, 'r') as f:
      file_text = f.read()
    sgPyQt = SalomePyQt.SalomePyQt()
    sgPyQt.activateModule("OpenTURNS")
    otStudy = otguibase.Study(study_name)
    otguibase.Study.Add(otStudy)
    physicalModel = otguibase.YACSPhysicalModel(model_name, file_text)
    otStudy.add(physicalModel)
  return errors
