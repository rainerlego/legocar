#!/bin/bash

# execute this script with:
# ". ./environment.sh"

case `hostname` in
  "e330" )
    ALTERADIR=/home/rainer/altera
    ALTERAVERSION=13.0sp1
    ;;
  "victorique" )
    ALTERADIR=/home/xtremely/altera
    ALTERAVERSION=13.0sp1
    ;;
  "idefix" )
    ALTERADIR=/home/dragon/altera
    ALTERAVERSION=13.0sp1
    ;;
  "anton" )
    ALTERADIR=/home/dragon/altera
    ALTERAVERSION=13.0sp1
#ALTERAVERSION=12.1sp1
    ;;
  * )
    echo "unknown hostname please edit file"
    exit 0
    ;;
esac

#quartos rootdir
export QUARTUS_ROOTDIR=$ALTERADIR/$ALTERAVERSION/quartus

#quartos binaries
export PATH=$PATH:$ALTERADIR/$ALTERAVERSION/quartus/bin

#nios2 binaries
export PATH=$PATH:$ALTERADIR/$ALTERAVERSION/nios2eds/bin
export PATH=$PATH:$ALTERADIR/$ALTERAVERSION/nios2eds/bin/gnu/H-i686-pc-linux-gnu/bin
export PATH=$PATH:$ALTERADIR/$ALTERAVERSION/nios2eds/sdk2/bin
