all: compile link

compile:
	g++ -c main.cpp -Isrc/include
	g++ -c particle.cpp -Isrc/include
	g++ -c particle_sim.cpp -Isrc/include
	g++ -c configuration_menu.cpp -Isrc/include
	
# add -mwindows at end of link to hide console
link:
	g++ main.o particle.o particle_sim.o configuration_menu.o -o main -Lsrc/lib -lsfml-graphics -lsfml-window -lsfml-system -lopengl32 -lfreetype -lwinmm -lgdi32 	
