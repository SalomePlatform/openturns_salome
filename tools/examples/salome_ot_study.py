# Create a persalys study
import salome_ot
myScript = """
def _exec(a, b):
    d=a/b
    return d
"""
study, model = salome_ot.getYacsPyStudy(myScript)
# Customize the executor
import pydefx
import os
pydefx_path = os.path.dirname(pydefx.__file__)
light_executor_path = os.path.join(pydefx_path, "plugins", "lightexecutor.py")
mybuilder = pydefx.slurmbuilder.SlurmBuilder(executor=light_executor_path)
myModel = pydefx.SlurmStudy(schemaBuilder=mybuilder)
model.setJobModel(myModel)
# Job parameters
model.jobParameters().salome_parameters.job_name = 'idefix_job'
model.jobParameters().salome_parameters.work_directory = '/scratch/I35256/workingdir/persalys_light'
model.jobParameters().salome_parameters.result_directory = '/tmp/idefixb1vigq5k'
model.jobParameters().salome_parameters.resource_required.name = 'gaia'
model.jobParameters().salome_parameters.resource_required.nb_proc = 10
model.jobParameters().salome_parameters.wckey = 'P11N0:SALOME'
model.jobParameters().nb_branches = 10
# Persalys analysis
import persalys
values = [[0, 1, 2, 3, 4, 5, 6, 7, 8, 9],
[0, 1, 2, 3, 4, 5, 6, 7, 8, 9]]
design_0 = persalys.GridDesignOfExperiment('design_0', model, values)
design_0.setBlockSize(100)
interestVariables = ['d']
design_0.setInterestVariables(interestVariables)
design_0.run()
study.add(design_0)
