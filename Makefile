VPATH=${SRC_PATH}:./src
CC=gcc
CFLAGS=-Wall
ODIR=obj
_OBJ = main.o arm64_add.o emulator.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c
	@mkdir -p $(ODIR)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: %.s
	$(CC) -c -o $@ $< $(CFLAGS)

emualte_arm64: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f $(ODIR)/*.o