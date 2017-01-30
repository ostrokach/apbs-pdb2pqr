#!/bin/bash

set +ex
set -v

CWD=`pwd`

function validate () {
  if [[ $1 -eq 0 ]] ; then
    echo -e "Pass!\n"
  else
    echo -e "Fail!\n"
  fi
}

cd "$CWD/examples/geoflow"
# apbs 1a63.in >/dev/null
# validate $?
apbs glycerol.in >/dev/null
validate $?
apbs imidazole.in >/dev/null
validate $?


cd "$CWD/examples/bem"
apbs 1a63_msms.in >/dev/null
validate $?
apbs 1a63_NanoShaper_SES.in >/dev/null
validate $?
apbs 1a63_NanoShaper_Skin.in >/dev/null
validate $?
apbs 451c_order1.in >/dev/null
validate $?
apbs 451c_order5.in >/dev/null
validate $?


cd "$CWD/examples/pbam"
apbs 1a63.in >/dev/null
validate $?
apbs toy_dynamics.in >/dev/null
validate $?
apbs toy_electrostatic.in >/dev/null
validate $?
apbs toy_energyforce.in >/dev/null
validate $?


cd "$CWD/examples/pbsam-gly"
apbs gly_dynamics.in >/dev/null
validate $?
apbs gly_electrostatic.in >/dev/null
validate $?
apbs gly_energyforce.in >/dev/null
validate $?
