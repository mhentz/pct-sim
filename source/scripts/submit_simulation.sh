#!/bin/bash
# SPDX-License-Identifier: BSD-3-Clause
# Copyright (c) 2026 Matthieu Hentz

trap "echo; exit" INT
SIM=${1}
STEP_SIZE=${2}

__usage="Usage: $(basename $0) [<sim> [ci|cr|gi|gr|si|sr]] [step_size]"

print_usage() {
  echo "$__usage"
}

if [ -z ${SIM} ]; then
  echo "Error: no sim chosen. Please choose one of the following: "
  echo "   - ci: ctp528 ideal"
  echo "   - cr: ctp528 realistic"
  echo "   - gi: gammex ideal"
  echo "   - gr: gammex realistic"
  echo "   - si: spiral ideal"
  echo "   - sr: spiral realistic"
  print_usage
  exit 1
fi

if [ -z ${STEP_SIZE} ]; then
  echo "Error: no rotation step size chosen. Give a step size in degrees."
  print_usage
  exit 1
fi

if [[ ! ${SIM:0:1} =~ ^[cgs]$ ]] || [[ ! ${SIM:1:1} =~ ^[ir]$ ]]; then
  echo "Error: sim not available. Please choose one of the following: "
  echo "   - ci: ctp528 ideal"
  echo "   - cr: ctp528 realistic"
  echo "   - gi: gammex ideal"
  echo "   - gr: gammex realistic"
  echo "   - si: spiral ideal"
  echo "   - sr: spiral realistic"
  print_usage
  exit 1
fi

case ${SIM:0:1} in
c)
  PHANTOM="ctp528"
  ;;
g)
  PHANTOM="gammex"
  ;;
s)
  PHANTOM="spiral"
  ;;
esac

case ${SIM:1:1} in
i)
  SETUP="ideal"
  ;;
r)
  SETUP="realistic"
  ;;
esac

LAST_PROJECTION=359
: "${GEANT4_INSTALL:?Set GEANT4_INSTALL to your Geant4 installation prefix}"
WORKDIR="${WORKDIR:-${HOME}/simulations/protonct-sim/build-${SIM}}"

if [ "${STEP_SIZE}" -gt 2 ]; then
  GROUP=${STEP_SIZE}
else
  GROUP=6
fi

for FIRST in $(seq 0 ${GROUP} ${LAST_PROJECTION}); do
  LAST=$(echo "${FIRST} ${GROUP} 1" | awk '{print $1+$2-$3}')
  JOBNAME=$(printf "pct.${SIM}.%03d" ${FIRST})

  qsub -N ${JOBNAME} -o ${WORKDIR} \
    -v WORKDIR=${WORKDIR},GEANT4_INSTALL=${GEANT4_INSTALL} \
    -v PHANTOM=${PHANTOM} \
    -v SETUP=${SETUP} \
    -v START=${FIRST} \
    -v STOP=${LAST} \
    -v STEP_SIZE=${STEP_SIZE} \
    ${WORKDIR}/scripts/simulation.job
done
