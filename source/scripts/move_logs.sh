#!/bin/bash -x
# SPDX-License-Identifier: BSD-3-Clause
# Copyright (c) 2026 Matthieu Hentz

# Move logs into log dir

mkdir -p data/logs/{output,job}

mv output_* data/logs/output
mv pct.* data/logs/job

cp -r macros scripts data/logs
