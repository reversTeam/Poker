FROM debian

WORKDIR /home/root

RUN apt-get update
RUN apt-get install -y build-essential
RUN apt-get install -y libncurses5-dev libncursesw5-dev
RUN apt-get install -y xutils-dev
RUN apt-get install -y vim
RUN apt-get install -y default-mysql-client default-libmysqlclient-dev

COPY Makefile /home/root/Makefile
COPY include /home/root/include
COPY src /home/root/src
COPY tests /home/root/tests

RUN make

ENTRYPOINT ["./server", "4242"]
