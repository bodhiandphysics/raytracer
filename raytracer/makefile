mat3: geom/mat3.cpp geom/mat3.h vec3
	g++ -c -pthread -O2 geom/mat3.cpp

nicefp: geom/nicefp.cpp geom/nicefp.h
	g++ -c -pthread -O2 geom/nicefp.cpp

ray: geom/ray.cpp geom/ray.h vec3
	g++ -c -pthread -O2 geom/ray.cpp

shape: geom/shape.cpp geom/shape.h vec3
	g++ -c -pthread -O2 geom/shape.cpp

vec3: geom/vec3.cpp geom/vec3.h nicefp
	g++ -c -pthread -O2 geom/vec3.cpp

geom: mat3 nicefp ray shape vec3


surface: surface.cpp surface.h geom
	g++ -c -pthread -O2 surface.cpp

world: world.cpp world.h surface
	g++ -c -pthread -O2 world.cpp

raytrace: raytrace.cpp raytrace.h world
	g++ -c -pthread -O2 raytrace.cpp

renderer: renderer.cpp renderer.h raytrace
	g++ -c -pthread -O2 renderer.cpp

scene: scene.cpp scene.h renderer
	g++ -c -pthread -O2 scene.cpp

main: main.cpp scene
	g++ -c -pthread -O2 main.cpp 

all: main
	g++ -pthread -o raytrace mat3.o nicefp.o vec3.o shape.o ray.o surface.o world.o raytrace.o renderer.o scene.o main.o


.PHONY: clean
clean:
	rm *.o 


