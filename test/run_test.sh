#!/bin/bash
# Copyright (C) 2021-2026  EDF
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
TEST_TIMEOUT=90
CHECK_DELAY=3

tfile=$(mktemp --suffix=.persalystest)
$ROOT_SALOME_INSTALL/salome start --splash 0 $1 args:$2,$tfile &
SALOME_PID=$!

while (( TEST_TIMEOUT > 0 ))
do
  if ! kill -0 $SALOME_PID &>/dev/null
  then
    TEST_TIMEOUT=0
  else
    sleep $CHECK_DELAY
    (( TEST_TIMEOUT -= CHECK_DELAY ))
  fi
done

if kill -0 $SALOME_PID &>/dev/null
then
  $ROOT_SALOME_INSTALL/salome killall
  kill -9 $SALOME_PID &>/dev/null
  echo Test timeout exceeded! >> $tfile
fi
wait $SALOME_PID

#&> trace.log
if [[ $(< $tfile) == "ok!" ]]
then
  RESULT=0
else
  RESULT=1
  # sometimes ctest is hanging when salome crushes because of remaining processes
  # we need to killall in order to get rid of them.
  $ROOT_SALOME_INSTALL/salome killall
  cat $tfile
fi

echo "Test result:" $RESULT
exit $RESULT
