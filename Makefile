all:
	serv cli task_2

serv:
	gcc -o telnetserv telnet_server2.c

cli:
	gcc -o telnetcli telnet_client2.c

task_2:
	g++ -o mono task_2.cpp -std=c++11 -g
