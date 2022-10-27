# Copyright (C) 2021  EDF R&D
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

SET(SALOME_TEST_DRIVER "$ENV{KERNEL_ROOT_DIR}/bin/salome/appliskel/salome_test_driver.py")

SET(COMPONENT_NAME PERSALYS)

SET(TEST_NAME ${COMPONENT_NAME}_deterministic_analyses)
ADD_TEST(${TEST_NAME} ./run_test.sh driver_test.py test_deterministic_analyses.py)
SET_TESTS_PROPERTIES(${TEST_NAME} PROPERTIES
                                  LABELS "${COMPONENT_NAME};OPENTURNS"
                                  TIMEOUT 120
                    )

SET(TEST_NAME ${COMPONENT_NAME}_probabilistic_analyses)
ADD_TEST(${TEST_NAME} ./run_test.sh driver_test.py test_probabilistic_analyses.py)
SET_TESTS_PROPERTIES(${TEST_NAME} PROPERTIES
                                  LABELS "${COMPONENT_NAME};OPENTURNS"
                                  TIMEOUT 120
                    )

SET(TEST_NAME ${COMPONENT_NAME}_field_analyses)
ADD_TEST(${TEST_NAME} ./run_test.sh driver_test.py test_field_analyses.py)
SET_TESTS_PROPERTIES(${TEST_NAME} PROPERTIES
                                  LABELS "${COMPONENT_NAME};OPENTURNS"
                                  TIMEOUT 120
                    )

SET(TEST_NAME ${COMPONENT_NAME}_basicOT)
ADD_TEST(${TEST_NAME} python3 test_basicOT.py )
SET_TESTS_PROPERTIES(${TEST_NAME} PROPERTIES
                                  LABELS "${COMPONENT_NAME};OPENTURNS"
                                  TIMEOUT 120
                    )
