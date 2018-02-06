#! /bin/bash

HOST="127.0.0.1"
PORT=12345

ARGS="$@"
REPS=`echo -n "${ARGS}" | netcat ${HOST} ${PORT}`

for l in "${REPS}"; do
	MODE=`echo "${l}" | cut -c 1`
	LINE=`echo "${l}" | cut -c 3-`
	if [ "${MODE}" == "1" ]; then
		echo "${LINE}" >&1
	else
		echo "${LINE}" >&2
	fi
done
