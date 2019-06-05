FROM ubuntu:disco
RUN apt update
RUN apt install git make cmake g++ -y
run apt install googletest libgtest-dev -y
WORKDIR nano-com/emulator
CMD git clone https://github.com/scifi6546/nano-com.git && ls nano-com/ && make -C nano-com/emulator test
