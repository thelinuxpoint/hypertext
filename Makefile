#################### start ###########################
cfiles1 := $(wildcard ./src/component/*)
hfiles  := $(wildcard ./src/header/*)
cfiles  := $(cfiles1) 
temp    := $(cfiles:.cc=.o)
ofiles  := $(addprefix ./build/, $(notdir $(temp)))

libs    := $(shell pkg-config --libs gtkmm-3.0)
libs    += $(shell pkg-config --libs glib-2.0)
libs    += $(shell pkg-config --libs gio-2.0) 
libs    += $(shell pkg-config --libs gtksourceviewmm-3.0) 


cflags  := $(shell pkg-config --cflags gtkmm-3.0)
cflags  += $(shell pkg-config --cflags glib-2.0)
cflags  += $(shell pkg-config --cflags gio-2.0)
cflags  += $(shell pkg-config --cflags gtksourceviewmm-3.0)

######################################################


main: $(cfiles) $(hfiles) $(ofiles)
	@echo "Compiling Main HyperText v0.1.0"
	@g++ -Wall  $(cflags) main.cc -o ./build/hyper_text  $(ofiles) $(libs) >> log 2>&1

# object files



./build/window.o: ./src/component/window.cc ./src/header/window.h
	@printf "Compiling window.cc ~> window.o"
	@g++ -c $(cflags) ./src/component/window.cc -o ./build/window.o >> log 2>&1
	@echo " => [ok]"



./build/hypertextapp.o: ./src/component/hypertextapp.cc ./src/header/hypertextapp.h
	@printf "Compiling hypertextapp.cc ~> hypertextapp.o"
	@g++ -c $(cflags) ./src/component/hypertextapp.cc -o ./build/hypertextapp.o >> log 2>&1
	@echo " => [ok]"

# to run from make

run:
	@clear
	@touch log
	@echo > log
	@make
	@./build/hyper_text
