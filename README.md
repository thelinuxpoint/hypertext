# Hyper-Text
Hyper-Text is currently only for linux systems
<div align="center">
    <img src="https://github.com/thelinuxpoint/hypertext/blob/main/src/resource/hyp.svg" width="200px" height="200px"/>
</div>

[![CircleCI](https://circleci.com/gh/thelinuxpoint/hypertext/tree/main.svg?style=shield)](https://circleci.com/gh/thelinuxpoint/hypertext/tree/main)

## Download HyperText

```shell
git clone https://github.com/thelinuxpoint/hypertext.git

cd hypertext
```

## Installing Dependencies

The following dependencies need to be resolved for successfull build

```shell
sudo apt-get install libgtkmm-3.0-dev -y

sudo apt-get install libgtksourceviewmm-3.0-dev
```

## Building HyperText

create a folder `build` and compile all source file using `make`

```shell
mkdir build

make

```

## To run

```shell
make run
```
