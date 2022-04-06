example: example.cpp
	g++ example.cpp -ldl -o example

update-emu:
	cd ../NutShell; make emu GEN_DYNAMIC_LIB=1 EMU_CXX_EXTRA_FLAGS="-DFIRST_INST_ADDRESS=0x80000000"
	cd ../NutShell/build/emu-compile; clang++ SimJTAG.o axi4.o common.o compress.o device.o flash.o keyboard.o lightsss.o ram.o remote_bitbang.o sdcard.o uart.o vga.o difftest.o goldenmem.o interface.o ref.o refproxy.o memdep.o runahead.o spikedasm.o tllogger.o main.o emu.o difftest-dl.o snapshot.o verilated.o verilated_dpi.o VSimTop__ALL.a  -lpthread -lSDL2 -ldl -rdynamic -shared -fPIC -lz -lsqlite3    -o ../emu.so

test: example
	./example --dump-wave -b 0 -e -0 -i ../NutShell/ready-to-run/microbench.bin --no-diff
	# ./example --dump-wave -b 0 -e -0 -i ../NutShell/ready-to-run/microbench.bin --diff=../XiangShan/ready-to-run/riscv64-nemu-interpreter-dual-so

