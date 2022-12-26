main: main.o vec3.h ray.h hitable.h sphere.h hitable_list.h camera.h
	g++ -g main.o -o main

main.o: main.cpp vec3.h ray.h hitable.h sphere.h hitable_list.h camera.h
	g++ -g -c main.cpp -o main.o

clean:
	rm -f main main.o image.ppm