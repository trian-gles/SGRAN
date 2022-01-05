include package.conf

NAME = SGRAN2
NAME2 = STGRAN2

OBJS = $(NAME).o
OBJS2 = $(NAME2).o
CMIXOBJS += $(PROFILE_O)
CXXFLAGS += -I. -Wall 
PROGS = $(NAME) lib$(NAME).so $(NAME2) lib$(NAME2).so

all: lib$(NAME).so lib$(NAME2).so

standalone: $(NAME) $(NAME2)

lib$(NAME).so: $(OBJS) $(GENLIB)
	$(CXX) $(SHARED_LDFLAGS) -o $@ $(OBJS) $(GENLIB) $(SYSLIBS)

lib$(NAME2).so: $(OBJS2) $(GENLIB)
	$(CXX) $(SHARED_LDFLAGS) -o $@ $(OBJS2) $(GENLIB) $(SYSLIBS)

$(NAME): $(OBJS) $(CMIXOBJS)
	$(CXX) -o $@ $(OBJS) $(CMIXOBJS) $(LDFLAGS)

$(NAME2): $(OBJS2) $(CMIXOBJS)
	$(CXX) -o $@ $(OBJS2) $(CMIXOBJS) $(LDFLAGS)

$(OBJS): $(INSTRUMENT_H) $(NAME).h

$(OBJS2): $(INSTRUMENT_H) $(NAME2).h

clean:
	$(RM) $(OBJS) $(OBJS2)  $(PROGS)

