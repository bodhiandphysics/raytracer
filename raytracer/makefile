mat3: geom/mat3.cpp geom/mat3.h
	g++ -c -O2 geom/mat3.cpp

nicefp: geom/nicefp.cpp geom/nicefp.h
	g++ -c -O2 geom/nicefp.cpp

ray: geom/ray.cpp geom/ray.h
	g++ -c -O2 geom/ray.cpp

shape: geom/shape.cpp geom/shape.h
	g++ -c -O2 geom/shape.cpp

vec3: geom/vec3.cpp geom/vec3.h
	g++ -c -O2 geom/vec3.cpp

geom: mat3 nicefp ray shape vec3


surface: surfade.cpp surface.h geom
	g++ -c -O2 surface.cpp

world: world.cpp world.h surface
	g++ -c -O2 world.cpp

raytrace: raytrace.cpp raytrace.h world
	g++ -c -O2 raytrace.cpp

renderer: renderer.cpp renderer.h raytrace:
	g++ -c -O2 renderer.cpp

scene: scene.cpp scene.h renderer
	g++ -c -O2 scene.cpp

main: main.cpp scene
	g++ -c -O2 main.cpp

all: main
	g++ -o raytrace geom/mat3.o geom/nicefp.o geom/vec3.o geom/shape.o geom/ray.o surface.o world.o raytrace.o renderer.o scene.o main.o


.PHONY
clearn:
	rm *.o geom/*.o 

	
