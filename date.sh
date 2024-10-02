#!/bin/bash

current_date=$(date +%Y-%m-%d)
if [[ "$current_date" < "$1" ]] || [[ "$current_date" == "$1" ]]; then
    exit 0  # Return 0 for success
else
    exit 1  # Return 1 for failure
fi
