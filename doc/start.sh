#!/bin/sh
service mosquitto start &
cd prj && ./open &
mosquitto_sub -t detection

