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
    ddlog -i playpen.dl
    cd playpen_ddlog && cargo build --release && cd ..
fi

./playpen_ddlog/target/release/playpen_cli < data.dat
