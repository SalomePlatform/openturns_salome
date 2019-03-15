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
     This is the old way to create an OtStudy. getYacsPyStudy should be prefered.
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

def getYacsPyStudy(pyScript, study_name = None, model_name = None):
  """ Open OPENTURNS module and create a study using a YACS model from a python
      script.
      Parameters:
        - pyScript : python script as text
        - study_name : if None, use a default name
        - model_name : if None, use a default name
      Return:
        - otStudy
        - physical model

      You can set the job parameters this way:
      study, model = salome_ot.getYacsPyStudy(myScript)
      model.jobParameters().salome_parameters.in_files = ["/one/file", "/another/file"]

      You can find all the options of 'salome_parameters' in the structure
      JobParameters from SALOME_Launcher.idl (KERNEL module).
  """
  sgPyQt = SalomePyQt.SalomePyQt()
  sgPyQt.activateModule("OpenTURNS")
  if study_name is None:
    study_name = "MyStudy"
  study_name = otguibase.Study.GetAvailableName(study_name)
  otStudy = otguibase.Study(study_name)
  otguibase.Study.Add(otStudy)
  if model_name is None:
    model_name = "PhysicalModel"
  physicalModel = otguibase.YACSPhysicalModel(model_name, pyScript)
  otStudy.add(physicalModel)
  return otStudy, physicalModel
