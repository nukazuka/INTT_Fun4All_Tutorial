# Answers of C++ tutorial

## sample1
The simplest sample.
Just try compiling:
```
g++ sample1.cc
```
If it's successful, a new executable file a.out is generated.
You can execute it by
```
./a.out
```
Since the code does nothing, nothing happen.
It's fine.

## sample2
Typical sample of C/C++ language.
It prints "Hello, world!".
You can comple it in the same way of sample1 but need to give right file name sample2.cc.
You should see something like
```
[nukazuka@sphnx05 04:10:49 answers] $ g++ sample2.cc
[nukazuka@sphnx05 04:10:55 answers] $ ./a.out
Hello, world!
```

## sample3
A very simple caclulation is done, and the result is printed on your terminal.
Do same as sample2.

## sample4
Same behaviour as sample3, but the caclulation is done in a function in function4.h.
You can learn how to make a function from this sample.

## sample5
See the head par of sample5.cc for the detailed explanation.
This is the demonstration of ways of compilation.

The simplest way is
```
g++ g++ sample5.cc function5.cc
```

The next step is split compilation. See the code for a hint.

Makefile is for this sample.
Try
```
make sample5_1
```
,
```
make sample5_2
```
or
```
make sample5_3
```
.

### Makefile
If you write rules to compile files (or do something), the defined processes are executed with make command.
Google how to use it as it's too much for me.
