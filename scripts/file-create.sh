#!/bin/bash
# file-create.sh

touch "$1"

if [[ -f "$1" ]]; then
    exit 0
else
    exit 1
fi

