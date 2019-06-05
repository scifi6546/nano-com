FROM ubuntu:disco
RUN apt update
RUN apt install git make cmake g++ -y
run apt install googletest libgtest-dev -y
RUN git clone https://github.com/scifi6546/nano-com.git
WORKDIR nano-com/emulator
CMD make test
