remove-gifs :
	rm -f *.gif

clean :
	rm -f *.dat *.out *.h.*

simulate : main.cpp simulation.h helpers.cpp initials.cpp iterate.cpp slope_logic.cpp limiters.cpp
	g++ -O3 -Wall main.cpp simulation.h helpers.cpp initials.cpp iterate.cpp slope_logic.cpp limiters.cpp
	echo 5 | ./a.out