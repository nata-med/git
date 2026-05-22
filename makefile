all: mygit

clean:
	rm -f mygit mygit.exe main.o help.o utility.o modification_commands.o reading_analytics_commands.o

mygit: main.o help.o utility.o modification_commands.o reading_analytics_commands.o
	gcc main.o help.o utility.o modification_commands.o reading_analytics_commands.o -o mygit

main.o: main.c commands.h
	gcc -c main.c -o main.o

help.o: help.c commands.h
	gcc -c help.c -o help.o

utility.o: utility.c commands.h
	gcc -c utility.c -o utility.o

modification_commands.o: modification_commands.c commands.h
	gcc -c modification_commands.c -o modification_commands.o

reading_analytics_commands.o: reading_analytics_commands.c commands.h
	gcc -c reading_analytics_commands.c -o reading_analytics_commands.o