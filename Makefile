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
	@echo "Compiling Main HyperText@v0.1.1"
	@g++-11 -std=c++20 -Wall  $(cflags) main.cc -o ./build/hypertext  $(ofiles) $(libs) 

# object files


./build/hyptextview.o: ./src/component/hyptextview.cc ./src/header/hyptextview.h
	@printf "Compiling hyptextview.cc ~> hyptextview.o"
	@g++-11 -std=c++20 -c $(cflags) ./src/component/hyptextview.cc -o ./build/hyptextview.o 
	@echo " => [ok]"
# 
./build/hyptreeview.o: ./src/component/hyptreeview.cc ./src/header/hyptreeview.h
	@printf "Compiling hyptreeview.cc ~> hyptreeview.o"
	@g++-11 -std=c++20 -c $(cflags) ./src/component/hyptreeview.cc -o ./build/hyptreeview.o 
	@echo " => [ok]"
# 
./build/hypworker.o: ./src/component/hypworker.cc ./src/header/hypworker.h
	@printf "Compiling hypworker.cc ~> hypworker.o"
	@g++-11 -std=c++20 -c $(cflags) ./src/component/hypworker.cc -o ./build/hypworker.o
	@echo " => [ok]"
# 
./build/hypimgview.o: ./src/component/hypimgview.cc ./src/header/hypimgview.h
	@printf "Compiling hypimgview.cc ~> hypimgview.o"
	@g++-11 -std=c++20 -c $(cflags) ./src/component/hypimgview.cc -o ./build/hypimgview.o 
	@echo " => [ok]"
# 
./build/hypshell.o: ./src/component/hypshell.cc ./src/header/hypshell.h
	@printf "Compiling hypshell.cc ~> hypshell.o"
	@g++-11 -std=c++20 -c $(cflags) ./src/component/hypshell.cc -o ./build/hypshell.o 
	@echo " => [ok]"
# 
./build/hypertextapp.o: ./src/component/hypertextapp.cc ./src/header/hypertextapp.h
	@printf "Compiling hypertextapp.cc ~> hypertextapp.o"
	@g++-11 -std=c++20 -c $(cflags) ./src/component/hypertextapp.cc -o ./build/hypertextapp.o 
	@echo " => [ok]"
# 
./build/hypwindow.o: ./src/component/hypwindow.cc ./src/header/hypwindow.h
	@printf "Compiling hypwindow.cc ~> hypwindow.o"
	@g++-11 -std=c++20 -c $(cflags) ./src/component/hypwindow.cc -o ./build/hypwindow.o 
	@echo " => [ok]"


# to run from make
install:
	@if [ $(shell id -u) -ne 0 ]; then \
		echo "You need to run as root"; \
		exit 1; \
	else \
		printf "Installing HyperText@0.1.0 ... /usr/bin ...";\
		cp ./build/hypertext /usr/bin/ ; \
		cp hypertext.desktop /usr/share/applications/ ; \
		cp ./src/resource/hypertext.svg /usr/share/icons/hicolor/scalable/apps/ ;\
	fi
	@echo " => [ok]"


run:
	@clear
	@make
	@./build/hypertext
