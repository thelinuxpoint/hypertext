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
	@g++ -std=c++20 -Wall  $(cflags) main.cc -o ./build/hypertext  $(ofiles) $(libs)

# object files


./build/hyptextview.o: ./src/component/hyptextview.cc ./src/header/hyptextview.h
	@printf "Compiling hyptextview.cc ~> hyptextview.o"
	@g++ -std=c++20 -c $(cflags) ./src/component/hyptextview.cc -o ./build/hyptextview.o  >> log 2>&1
	@echo " => [ok]"
#
./build/hyptreeview.o: ./src/component/hyptreeview.cc ./src/header/hyptreeview.h
	@printf "Compiling hyptreeview.cc ~> hyptreeview.o"
	@g++ -std=c++20 -c $(cflags) ./src/component/hyptreeview.cc -o ./build/hyptreeview.o  >> log 2>&1
	@echo " => [ok]"
#
./build/hypworker.o: ./src/component/hypworker.cc ./src/header/hypworker.h
	@printf "Compiling hypworker.cc ~> hypworker.o"
	@g++ -std=c++20 -c $(cflags) ./src/component/hypworker.cc -o ./build/hypworker.o >> log 2>&1
	@echo " => [ok]"
#
./build/hypimgview.o: ./src/component/hypimgview.cc ./src/header/hypimgview.h
	@printf "Compiling hypimgview.cc ~> hypimgview.o"
	@g++ -std=c++20 -c $(cflags) ./src/component/hypimgview.cc -o ./build/hypimgview.o  >> log 2>&1
	@echo " => [ok]"
#
./build/hypshell.o: ./src/component/hypshell.cc ./src/header/hypshell.h
	@printf "Compiling hypshell.cc ~> hypshell.o"
	@g++ -std=c++20 -c $(cflags) ./src/component/hypshell.cc -o ./build/hypshell.o  >> log 2>&1
	@echo " => [ok]"
#
./build/hypertextapp.o: ./src/component/hypertextapp.cc ./src/header/hypertextapp.h
	@printf "Compiling hypertextapp.cc ~> hypertextapp.o"
	@g++ -std=c++20 -c $(cflags) ./src/component/hypertextapp.cc -o ./build/hypertextapp.o  >> log 2>&1
	@echo " => [ok]"
#
./build/hypwindow.o: ./src/component/hypwindow.cc ./src/header/hypwindow.h
	@printf "Compiling hypwindow.cc ~> hypwindow.o"
	@g++ -std=c++20 -c $(cflags) ./src/component/hypwindow.cc -o ./build/hypwindow.o >> log 2>&1
	@echo " => [ok]"


# to run from make
install:

	@if [ $(shell id -u) -ne 0 ]; then \
		echo "You need to run as root"; \
		exit 1; \
	else \
		if [ -e /usr/share/hypertext ]; then \
			echo "hypertext folder exists ... copying it again ... [ok]";\
			rm -f -r /usr/share/hypertext/resource ;\
			cp -r ./src/resource /usr/share/hypertext/ ;\
		else \
			echo "making /usr/share/hypertext directory ... [ok]"; sudo mkdir /usr/share/hypertext ;\
			cp -r ./src/resource /usr/share/hypertext/ ;\
		fi;\
		printf "Installing HyperText@0.1.0 ... /usr/bin ...";\
		rm -f /usr/bin/hypertext;\
		cp ./build/hypertext /usr/bin/ ; \
		rm -f /usr/share/applications/hypertext.desktop;\
		cp hypertext.desktop /usr/share/applications/ ; \
		cp ./src/resource/hyp.svg /usr/share/icons/hicolor/scalable/apps/ ;\
	fi
	@echo " => [ok]"



run:
	@clear
	@echo > log
	@make
	@./build/hypertext
