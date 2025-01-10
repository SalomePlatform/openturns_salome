# Copyright (C) 2021-2025 EDF
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

# For salome test
SET(LOCAL_TEST_DIR ${SALOME_OT_INSTALL_TEST})
SET(TESTFILES
driver_test.py
test_deterministic_analyses.py
test_field_analyses.py
test_probabilistic_analyses.py
test_basicOT.py
)

FILE(GLOB COUPLING_FILES
${PROJECT_SOURCE_DIR}/test/coupling_model/*
)

INSTALL(FILES ${TESTFILES} DESTINATION ${LOCAL_TEST_DIR})
INSTALL(PROGRAMS run_test.sh
        DESTINATION ${LOCAL_TEST_DIR})
INSTALL(FILES ${COUPLING_FILES} DESTINATION ${LOCAL_TEST_DIR}/coupling_model)

INSTALL(FILES CTestTestfileInstall.cmake
        DESTINATION ${LOCAL_TEST_DIR}
        RENAME CTestTestfile.cmake)
