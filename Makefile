objects = labirynt.o wyswietlacz.o oznaczenie.o dane_labiryntu.o konwerter.o



program : $(objects)

	cc -o program $(objects) -lm

labirynt.o : labirynt.c
	cc -c labirynt.c
wyswietlacz.o : wyswietlacz.c

oznaczenie.o : oznaczenie.c

dane_labiryntu.o : dane_labiryntu.c

konwerter.o : konwerter.c

clean :
	rm program $(objects)
