#!/bin/bash

has_b_option=false
while getopts :hb opt; do
    case $opt in
        h) show_some_help; exit ;;
        b) has_b_option=true ;;
        :) echo "Missing argument for option -$OPTARG"; exit 1;;
       \?) echo "Unknown option -$OPTARG"; exit 1;;
    esac
done

if $has_b_option; then
    ddlog -i prog.dl
    cd prog_ddlog && cargo build --release && cd ..
fi

./prog_ddlog/target/release/prog_cli