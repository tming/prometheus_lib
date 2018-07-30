CFLAGS:=-Wall  -g
CPPFLAGS:=$(CFLAGS)
CPPFLAGS += -MD

srcdir=./
objdir=./

src:=$(wildcard $(srcdir)*.cpp)
obj:=$(patsubst %.cpp, %.o, $(src))
dep:=$(patsubst %.cpp, %.d, $(src))

all: $(objdir)test_obj

deps: $(dep)

$(objdir)test_obj: $(obj)
	g++ $^ $(CPPFLAGS) -o $@

clean: 
	rm *.o
	rm *.d

-include $(dep)

rebuild: clean all