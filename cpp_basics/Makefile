##############################
# The rule for sample5       #
#----------------------------#
# Do as follows:             #
#   $ make sample5           #
##############################
sample5: sample5.o function5.o
	g++ -o sample5 sample5.o function5.o

sample5.o:
	g++ -o sample5.o -c sample5.cc

function5.o:
	g++ -o function5.o -c function5.cc

##############################
# The rule for sample5_2     #
#----------------------------#
# Do as follows:             #
#   $ make sample5_2         #
##############################
sample5_2: sample5.o function5.o
	g++ -o $@ $^

# General rule to make *.o
%.o: %.cc
	g++ -o $@ -c $<

# If you want to do everything, all is commonly used.
all: clean $(TARGET)

# To clean your working directory, clean is commonly used.
clean:
	rm -f $(OBJECTS) $(TARGET) sample5 a.out *.o *~
