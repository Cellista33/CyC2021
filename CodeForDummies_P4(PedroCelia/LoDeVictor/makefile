#Makefile 

todo: ClienteEco ClienteSrobot


ClienteEco: ClienteEco.o 
	g++ ClienteEco.o  -o ClienteEco -lrt -lpthread -std=c++11

ClienteSrobot: ClienteSrobot.o 
	g++ ClienteSrobot.o  -o ClienteSrobot -lrt -lpthread -std=c++11


ClienteEco.o: ClienteEco.c 
	g++ -c ClienteEco.c -std=c++11

ClienteSrobot.o: ClienteSrobot.c header.h
	g++ -c ClienteSrobot.c -std=c++11
