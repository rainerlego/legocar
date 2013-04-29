#!/bin/bash

#das erste mal:
#nios2-bsp hal ./carbsp ../qsys_design.sopcinfo

#nach jedem anpassen von settings.bsp:
nios2-bsp-generate-files --settings=carbsp/settings.bsp --bsp-dir=./carbsp/

#software generieren
cd hw2
nios2-app-generate-makefile --bsp-dir ../carbsp --elf-name blubb --src-dir .
