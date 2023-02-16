
PATH_TO_LIB := reachability_ddlog
LIB := $(PATH_TO_LIB)/target/release/libreachability_ddlog.a

FLAGS := -I $(PATH_TO_LIB) -lpthread -ldl -lm

ALL: compile compile_c
	cp $(LIB) ../$(PATH_TO_LIB)

DDLOG: reachability.dl
	ddlog -i reachability.dl

RUST:
	cd reachability_ddlog && cargo build --release

compile: DDLOG RUST

compile_c:
	gcc ddlog_util.c $(LIB) -I $(PATH_TO_LIB) $(FLAGS)
