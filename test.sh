#!/bin/bash
make
rm synth.wav
CMIX < test.sco
paplay synth.wav
