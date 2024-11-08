################################
# The rule for sample5_1       #
#------------------------------#
# Do as follows:               #
#   $ make sample5_1           #
################################
sample5_1: 
	g++ -o sample5_1 sample5.cc function5.cc

##############################
# The rule for sample5_2     #
#----------------------------#
# Do as follows:             #
#   $ make sample5_2         #
##############################
sample5_2: sample5.o function5.o
	g++ -o sample5_2 sample5.o function5.o

sample5.o:
	g++ -c sample5.cc

function5.o:
	g++ -c function5.cc

##############################
# The rule for sample5_2_2   #
#----------------------------#
# Do as follows:             #
#   $ make sample5_2_2       #
##############################
sample5_2_2: sample5.o function5.o
	g++ -o $@ $^

# General rule to make *.o
%.o: %.cc
	g++ -o $@ -c $<

##############################
# The rule for sample5_3     #
#----------------------------#
# Do as follows:             #
#   $ make sample5_3         #
##############################
sample5_3: libfunction5.so
	g++ -o $@ -L. -lfunction5 sample5.cc

libfunction5.so: function5.cc
	g++ -shared -fPIC -o $@ $^


# If you want to do everything, all is commonly used.
all: clean $(TARGET)

# To clean your working directory, clean is commonly used.
clean:
	rm -f $(OBJECTS) $(TARGET) sample5_* a.out lib*.so *.o *~
