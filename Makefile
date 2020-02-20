# The final executable
PROGRAM = traffic.x

# The source files (*.cc)
SRCS = traffic.cc input_gen.cc traffic_testbench.cc 

##################################
## DO NOT CHANGE ANYTHING BELOW ##
##################################

SYSTEMC = /courses/TDTS07/sw/systemc
TARGET_ARCH = linux64

LIBDIR = -L. -L.. -L$(SYSTEMC)/lib-$(TARGET_ARCH)
LIBS = -Wl,-Bstatic -lsystemc -Wl,-Bdynamic -lm -lpthread
LDFLAGS = -g

CC = g++-4.8
OPT = -O3
CFLAGS = -Wno-deprecated -std=c++11
EXE = $(PROGRAM)

INCDIR= -I. -I.. -I$(SYSTEMC)/include

OBJS = $(SRCS:.cc=.o)

.SUFFIXES: .cc .o .x

$(EXE): $(OBJS)
	$(CC) $(LDFLAGS) $(INCDIR) $(LIBDIR) -o $(EXE) $(OBJS) $(LIBS)

.cc.o:
	$(CC) -c $(CFLAGS) $(INCDIR) $< -o $@

clean:
	rm -f $(OBJS) $(EXE) *~
