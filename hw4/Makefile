compiler = g++
compiler_flags = -Wall -g
linker_flags = 

object_files = main.o lib.o

output_file = onegin.exe

$(output_file): $(object_files)
	$(compiler) $(object_files) $(linker_flags) -o $(output_file)

main.o: main.cpp lib.h
	$(compiler) $(compiler_flags) -c main.cpp

lib.o: lib.cpp lib.h
	$(compiler) $(compiler_flags) -c lib.cpp

clean:
	rm -f $(object_files) $(output_file)