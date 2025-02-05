# Copyright (C) 2021-2025  EDF
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

"""
Script to be run in salomé GUI.
Steps:
  - activate Persalys
  - execute $1 file
  - if no exception, write "ok!" in $2
"""
if __name__ == '__main__':
    import argparse
    parser = argparse.ArgumentParser(description="Salome GUI test driver")
    parser.add_argument("file", help='Test script')
    parser.add_argument("result", help='Result log file')
    args = parser.parse_args()

    import SalomePyQt
    sgPyQt = SalomePyQt.SalomePyQt()
    sgPyQt.activateModule("Persalys")
    ok = True
    message = ""
    try:
        exec(open(args.file).read())
    except:
        ok = False
        import traceback
        message = traceback.format_exc()

    with open(args.result, "w") as f:
        if ok:
            f.write("ok!")
        else:
            f.write(message)
            f.write("probleme!\n")
    import os
    pid=os.getpid()
    os.kill(pid,9)
