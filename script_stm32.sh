#!/bin/bash

#----------------------------------
#DIRECTORIOS

sudo mkdir /opt/stm32
cd /opt/stm32

#PAQUETES

sudo apt-get install --yes libsgutils2-dev
sudo apt-get install --yes libusb-1.0-0-dev 
sudo apt-get install --yes pkgconfig
sudo apt-get install --yes autotools-dev
sudo apt-get install --yes autoconf
sudo apt-get install --yes automake
sudo apt-get install --yes git


sudo apt-get install --yes zlib1g-dev libtool flex \
bison libgmp3-dev libmpfr-dev libncurses5-dev libmpc-dev \
texinfo build-essential libftdi-dev

#----------------------------------
#TOOLCHAIN

sudo apt-get remove binutils-arm-none-eabi gcc-arm-none-eabi
sudo add-apt-repository ppa:terry.guo/gcc-arm-embedded -y
sudo apt-get update 
sudo apt-get install --yes gcc-arm-none-eabi

#----------------------------------
#STLINK

sudo git clone https://github.com/texane/stlink stlink.git
cd stlink.git
sudo ./autogen.sh
sudo ./configure
sudo make
sudo cp 49-stlinkv1.rules /etc/udev/rules.d
sudo udevadm control --reload-rules
sudo modprobe -r usb-storage && modprobe usb-storage quirks=483:3744:i

#----------------------------------
#EJEMPLOS DE LIBOPENCM3 PARA VARIAS PLACAS
cd /opt/stm32
sudo git clone git://github.com/libopencm3/libopencm3.git
cd libopencm3
sudo make
sudo make install
sudo git clone https://github.com/libopencm3/libopencm3-examples
cd libopencm3-examples
sudo git submodule init
sudo git submodule update
sudo make

#----------------------------------
#ECLIPSE

sudo add-apt-repository ppa:webupd8team/java -y
sudo apt-get update
sudo apt-get install --yes oracle-java8-installer

cd /opt
sudo wget http://mirrors.linux-bg.org/eclipse/technology/epp/downloads/release/luna/SR1a/eclipse-cpp-luna-SR1a-linux-gtk-x86_64.tar.gz
sudo tar -zxvf eclipse-cpp-luna-SR1a-linux-gtk-x86_64.tar.gz
sudo rm eclipse-cpp-luna-SR1a-linux-gtk-x86_64.tar.gz



