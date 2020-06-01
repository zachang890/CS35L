randmain: randmain.o randcpuid.o
	$(CC) $(CFLAGS) -ldl -Wl,-rpath=$(PWD) $^ -o $@
randmain.o: randmain.c
	$(CC) $(CFLAGS) -c $^ -o $@
randcpuid.o: randcpuid.c
	$(CC) $(CFLAGS) -c $^ -o $@
randlibhw.so: randlibhw.o 
	$(CC) $(CFLAGS) -shared $^ -o $@
randlibhw.o: randlibhw.c
	$(CC) $(CFLAGS) -fPIC -c $^ -o $@
randlibsw.so: randlibsw.o
	$(CC) $(CFLAGS)	-shared	$^ -o $@
randlibsw.o: randlibsw.c
	$(CC) $(CFLAGS)	-fPIC -c $^ -o $@
