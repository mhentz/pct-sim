#!/bin/bash
# SPDX-License-Identifier: BSD-3-Clause
# Copyright (c) 2026 Matthieu Hentz

trap "echo; exit" INT
SIM=${1}
STEP_SIZE=${2}
BATCH_SIZE=${3}

__usage="Usage: $(basename $0) [<sim> [ci|cr|gi|gr|si|sr]] [step_size] [batch_size]"

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

if [ -z ${BATCH_SIZE} ]; then
    echo "Error: no batch size given. Give the number of simulations to be run per batch."
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
WORKDIR="${WORKDIR:-${HOME}/simulations/pct-sim/build-${SIM}}"

BATCH_STEP=$((STEP_SIZE * BATCH_SIZE))

N_CHUNKS=8
for FIRST in $(seq 0 ${BATCH_STEP} ${LAST_PROJECTION}); do
    LAST=$(echo "${FIRST} ${BATCH_STEP} 1" | awk '{print $1+$2-$3}')

    for CHUNK in $(seq 0 $((N_CHUNKS - 1))); do
        JOBNAME=$(printf "pct.${SIM}.%03d.${CHUNK}" ${FIRST})

        qsub -N ${JOBNAME} -o ${WORKDIR} \
            -v WORKDIR=${WORKDIR},GEANT4_INSTALL=${GEANT4_INSTALL} \
            -v PHANTOM=${PHANTOM} \
            -v SETUP=${SETUP} \
            -v START=${FIRST} \
            -v STOP=${LAST} \
            -v STEP_SIZE=${STEP_SIZE} \
            -v CHUNK=${CHUNK} \
            ${WORKDIR}/scripts/split_simulation.job
    done
done
