#!/usr/bin/env python
# coding: utf-8

import openturns as ot
import persalys

MyStudy0 = persalys.Study('MyStudy0')
persalys.Study.Add(MyStudy0)
a = persalys.Input('a', 0, '')
b = persalys.Input('b', 0, '')
d = persalys.Output('d', '')
inputs = [a, b]
outputs = [d]
code = '''
def _exec(a, b):
    d=a/b
    return d
'''
PhysicalModel = persalys.YACSPhysicalModel('PhysicalModel', inputs, outputs, code)
PhysicalModel.jobParameters().salome_parameters.job_name = 'idefix_job'
PhysicalModel.jobParameters().salome_parameters.work_directory = '/scratch/I35256/workingdir/persalys_light'
PhysicalModel.jobParameters().salome_parameters.result_directory = '/tmp/idefixb1vigq5k'
PhysicalModel.jobParameters().salome_parameters.resource_required.name = 'gaia'
PhysicalModel.jobParameters().salome_parameters.resource_required.nb_proc = 10
PhysicalModel.jobParameters().salome_parameters.wckey = 'P11N0:SALOME'
PhysicalModel.jobParameters().nb_branches = 10
# use customized physical model
import pydefx
import os
pydefx_path = os.path.dirname(pydefx.__file__)
light_executor_path = os.path.join(pydefx_path, "plugins", "lightexecutor.py")
mybuilder = pydefx.slurmbuilder.SlurmBuilder(executor=light_executor_path)
myModel = pydefx.SlurmStudy(schemaBuilder=mybuilder)
PhysicalModel.setJobModel(myModel)
#
MyStudy0.add(PhysicalModel)
values = [[0, 1, 2, 3, 4, 5, 6, 7, 8, 9],
[0, 1, 2, 3, 4, 5, 6, 7, 8, 9]]
design_0 = persalys.GridDesignOfExperiment('design_0', PhysicalModel, values)
design_0.setBlockSize(100)
interestVariables = ['d']
design_0.setInterestVariables(interestVariables)
design_0.run()
MyStudy0.add(design_0)
