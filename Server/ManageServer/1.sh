g++ main.cpp -std=c++11 -ljson $(mysql_config --cflags)  $(mysql_config --libs) -g -lglog -lpthread
