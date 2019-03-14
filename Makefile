all: hid_rw_registers

hid_rw_registers: src/hid_rw_registers.o
	gcc -o hid_rw_registers src/hid_rw_registers.o

hid_rw_registers.o: hid_rw_registers.c
	gcc -c src/hid_rw_registers.c
clean:
	rm src/hid_rw_registers.o hid_rw_registers
