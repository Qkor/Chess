# Chess

To compile, you need to install sfml library first, e.g. on Debian/Ubuntu:
```
sudo apt-get install libsfml-dev
```
then:
```
g++ -c main.cpp && g++ -c gui.cpp && g++ -c game.cpp && g++ -c move.cpp && g++ -c position.cpp &&  g++ -c button.cpp && g++ -c menu.cpp && g++ *.o -o chess -lsfml-graphics -lsfml-window -lsfml-system
```
You can run the program with:
```
./chess
```
