#!/bin/bash - 
#===============================================================================
#
#          FILE: 1.sh
# 
#         USAGE: ./1.sh 
# 
#   DESCRIPTION: 
# 
#       OPTIONS: ---
#  REQUIREMENTS: ---
#          BUGS: ---
#         NOTES: ---
#        AUTHOR: Dr. Fritz Mehner (fgm), mehner.fritz@fh-swf.de
#  ORGANIZATION: FH Südwestfalen, Iserlohn, Germany
#       CREATED: 2016年02月05日 21:39
#      REVISION:  ---
#===============================================================================


g++ main.cpp Server.cpp Client.cpp getInfo.cpp ./common/config.cpp -ljson -lpthread -std=c++11 -o workServer


