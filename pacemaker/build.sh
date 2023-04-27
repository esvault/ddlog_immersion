#!/bin/bash

ddlog -i prog.dl
cd prog_ddlog && cargo build --release && cd ..

./prog_ddlog/target/release/prog_cli < data.dat
