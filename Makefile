all: example_simple

example_simple:
	pio ci examples/simple.cpp --lib=. --board=uno
