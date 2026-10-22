#!/bin/bash
# SPDX-License-Identifier: BSD-3-Clause
# Copyright (c) 2026 Matthieu Hentz

DIR=${1}
if [ -z ${DIR} ]; then
  echo "Error: no path provided. Provide path to directory that contains scoring data."
  echo "Usage: ./total_slice_dose.sh path/to/scoring"
  exit
fi

tmpfile=$(mktemp /tmp/sliceDoseSummary.XXX)

trap "rm ${tmpfile}; echo; exit" INT

for f in ${DIR}/sliceDose*.csv; do
  cat $f | tail -n 1 | cut -d "," -f 4 >>${tmpfile}
done

awk '{ sum += $1 } END { print sum " Gy"}' ${tmpfile}

rm ${tmpfile}
