#!/usr/bin/env bash

green=`tput setaf 2`
yellow=`tput setaf 3`
reset=`tput sgr0`


function checkanswer {
    if [ "$1" != "y" ] && [ "$1" != "n" ]
    then
        echo "${yellow}bad input, please run again (make sure you use lowercase 'y' and 'n' only)${reset}"
        echo "${green}finish${reset}"
        exit
    fi    
}

#-------------------------------------------------------------------------------

compile_q="${yellow}compile? [y\n]${reset}"
run_q="${yellow}run? [y\n]${reset}"
valgrind_q="${yellow}with valgrind? [y\n]${reset}"

echo $compile_q
read compile_var
checkanswer $compile_var $compile_q

echo $run_q
read run_var
checkanswer $run_var $run_q
if [ "$run_var" == "y" ]
then
    echo $valgrind_q
    read val_var
    checkanswer $val_var $valgrind_q
fi

if [ "$compile_var" == "y" ]
then
    rm -f unit_test_exec
    echo "${yellow}compiling${reset}"
    g++ -std=c++11 -g -Wall -Werror -pedantic-errors -ggdb3 -DNDEBUG ./unit_tests/*.cpp ./*.cpp -o unit_test_exec
fi

if [ "$run_var" == "y" ]
then
    if [ -e ./unit_test_exec ]
    then
    if [ "$val_var" == "y" ]
    then    
        echo "${yellow}running with valgrind${reset}"
        valgrind --leak-check=full -s ./unit_test_exec
    elif [ "$val_var" == "n" ]
    then
        echo "${yellow}running${reset}"
        ./unit_test_exec
    fi
    else
        echo "${yellow}can't run - no executable present${reset}"
    fi
fi

echo "${green}finish${reset}"

