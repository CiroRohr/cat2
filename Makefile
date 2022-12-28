CC = gcc

.PHONY: compile
compile:
	$(CC) $(FILE)

.PHONY: clean
clean: 
	rm -f *.out


