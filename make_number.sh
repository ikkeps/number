#!/bin/bash

set -e

DEVICE_NAME=$1
CODE=$2
MODULE_NAME="number"

function usage {
  echo "Usage is: $0 device_name code_to_emit"
  echo " Example: $0 zeroes 48  # create device /dev/zeroes that emit \"0\" "
}

if [ -z "${DEVICE_NAME}" ]; then
  usage
  exit 1
fi

if [ -z "${CODE}" ]; then
    usage
    exit 1
fi


MAJOR=$(awk "\$2==\"$MODULE_NAME\" {print \$1}" /proc/devices)
if [ -z "${MAJOR}" ]; then
  echo "\"${MODULE_NAME}\" not found in /proc/devices. Is module loaded?"
  exit 2
fi

mknod /dev/${DEVICE_NAME} c $MAJOR $CODE
chmod a+r /dev/${DEVICE_NAME}
