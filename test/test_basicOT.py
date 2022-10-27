import unittest

class TestOt(unittest.TestCase):

  def test_importOT(self):
    import openturns
    import otfftw
    import otfmi
    import otmixmod
    import otmorris
    import otpmml
    import otpod
    import otrobopt
    import otsubsetinverse
    import otsvm
    import otwrapy

if __name__ == '__main__':
  import tempfile
  import os
  import sys
  dir_test = tempfile.mkdtemp(suffix=".persalystest")
  file_test = os.path.join(dir_test,"UnitTestsResult")
  with open(file_test, 'a') as f:
      suite = unittest.makeSuite(TestOt)
      result=unittest.TextTestRunner(f, descriptions=1, verbosity=1).run(suite)
  sys.exit(not result.wasSuccessful())
