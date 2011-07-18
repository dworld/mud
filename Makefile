TARGET  = mud
BUILDIR = build
SOURCES = main.cc parser.cc lexer.cc value.cc
OBJS    = $(SOURCES:%.cc=$(BUILDIR)/%.o)
DEPS	= $(SOURCES:%.cc=$(BUILDIR)/%.d)

CPP		= g++ -E
CC      = g++ 
LD		= g++ 
CFLAGS  = -g -O0
CXXFLAGS= -g -O0
CPPFLAGS=
LDFLAGS = 

.PHONY  : all clean deps objs

all: $(TARGET)

$(BUILDIR):
	mkdir $(BUILDIR)

$(BUILDIR)/%.d: %.cc
	@echo -e "$<\t: generate deps" ; \
	mkdir -p $(BUILDIR); set -e; rm -f %@; \
	$(CC) -MM $(CPPFLAGS) $< > $@.$$$$; \
	sed 's/\($*\).o[ :]/$(BUILDIR)\/\1.o :/g' <$@.$$$$ >$@ ;\
	sed 'a$@ : $<' <$@ > $@.$$$$; \
	cp $@.$$$$ $@ ;\
	rm -f $@.$$$$

$(TARGET) :  $(BUILDIR) $(DEPS) $(OBJS)
	$(CC) -o $@ $(OBJS)

objs: $(OBJS)

deps: $(DEPS)
	
$(BUILDIR)/%.o: %.cc
	$(CC) -c -o $@ $(CXXFLAGS)  $<

# '-' : ignore warning message
-include $(DEPS)

clean:
	rm -f $(DEPS) 	
	rm -f $(OBJS)	
	rm -f $(TARGET) 


