#!/bin/bash
# time.sh

if [[ "$(date +%H:%M)" <="$1" ]]; then
    exit 0
else
    exit 1
fi

