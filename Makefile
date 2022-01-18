#################### start ###########################
cfiles1 := $(wildcard ./src/component/*)
hfiles  := $(wildcard ./src/header/*)
cfiles  := $(cfiles1) 
temp    := $(cfiles:.cc=.o)
ofiles  := $(addprefix ./build/, $(notdir $(temp)))

libs    := $(shell pkg-config --libs gtkmm-3.0)
libs    += $(shell pkg-config --libs glibmm-2.4)
libs    += $(shell pkg-config --libs giomm-2.4) 
libs    += $(shell pkg-config --libs gtksourceviewmm-3.0) 


cflags  := $(shell pkg-config --cflags gtkmm-3.0)
cflags  += $(shell pkg-config --cflags glibmm-2.4)
cflags  += $(shell pkg-config --cflags giomm-2.4)
cflags  += $(shell pkg-config --cflags gtksourceviewmm-3.0)

######################################################
# libgtkmm-3.0-dev
# libgtksourceviewmm-3.0-dev


main: $(cfiles) $(hfiles) $(ofiles)
	@echo "Compiling Main HyperText@v0.1.0"
	@g++ --std=c++20 -Wall  $(cflags) main.cc -o ./build/hyper_text  $(ofiles) $(libs) >> log 2>&1

# object files


./build/hyptextview.o: ./src/component/hyptextview.cc ./src/header/hyptextview.h
	@printf "Compiling hyptextview.cc ~> hyptextview.o"
	@g++ --std=c++20 -c $(cflags) ./src/component/hyptextview.cc -o ./build/hyptextview.o >> log 2>&1
	@echo " => [ok]"

./build/hypworker.o: ./src/component/hypworker.cc ./src/header/hypworker.h
	@printf "Compiling hypworker.cc ~> hypworker.o"
	@g++ --std=c++20 -c $(cflags) ./src/component/hypworker.cc -o ./build/hypworker.o >> log 2>&1
	@echo " => [ok]"

./build/hypimgview.o: ./src/component/hypimgview.cc ./src/header/hypimgview.h
	@printf "Compiling hypimgview.cc ~> hypimgview.o"
	@g++ --std=c++20 -c $(cflags) ./src/component/hypimgview.cc -o ./build/hypimgview.o >> log 2>&1
	@echo " => [ok]"

./build/hypertextapp.o: ./src/component/hypertextapp.cc ./src/header/hypertextapp.h
	@printf "Compiling hypertextapp.cc ~> hypertextapp.o"
	@g++ --std=c++20 -c $(cflags) ./src/component/hypertextapp.cc -o ./build/hypertextapp.o >> log 2>&1
	@echo " => [ok]"

./build/hypwindow.o: ./src/component/hypwindow.cc ./src/header/hypwindow.h
	@printf "Compiling hypwindow.cc ~> hypwindow.o"
	@g++ --std=c++20 -c $(cflags) ./src/component/hypwindow.cc -o ./build/hypwindow.o >> log 2>&1
	@echo " => [ok]"


# to run from make

run:
	@clear
	@touch log
	@echo > log
	@make
	@./build/hyper_text
