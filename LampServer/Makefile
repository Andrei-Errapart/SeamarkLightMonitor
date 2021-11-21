DFLAGS		:= -g
GFLAGS		:= -I ../../base/utils -I../../base/include -Wall -I/usr/include/mysql

LAMPSERVER_SRC	:= main.cxx LampServer.cxx LampConfig.cxx globals.cxx LampDB.cxx
LAMPSERVER_OBJ	:= $(LAMPSERVER_SRC:.cxx=.o)

all:	lampserver

%.o:	%.cxx
	g++ $(DFLAGS) $(GFLAGS) -o $@ -c $<

lampserver: $(LAMPSERVER_OBJ)
	g++ $(DFLAGS) $(GFLAGS) -o $@ $(LAMPSERVER_OBJ) ../../base/utils/libutils.a -levent -lmysqlclient -lpthread

clean:
	rm -f $(LAMPSERVER_OBJ)
	rm -f lampserver

lampserver2.gz:	lampserver
	rm -f $@
	rm -f lampserver2
	cp $< lampserver2
	strip lampserver2
	gzip -9 lampserver2

