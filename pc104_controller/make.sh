g++ -g -Wall -Iinclude -I/usr/include -c src/odrive_can_functions.cpp	-o build/odrive_can_functions.o 
g++ -g -Wall -Iinclude -I/usr/include -c src/misc_functions.cpp	-o build/misc_functions.o 
g++ -g -Wall -Iinclude -I/usr/include -c src/controller.cpp -o build/controller.o -L/usr/lib -lm -lpthread
g++ -Wall -o bin/controller build/odrive_can_functions.o build/misc_functions.o build/controller.o -L/usr/lib -lm -lpthread
