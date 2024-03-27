app:
	g++ -lboost_thread -lboost_chrono sender.cpp receiver.cpp main.cpp -o app.out
dapp:
	g++ -g -lboost_thread -lboost_chrono main.cpp -o app.out
recv:
	g++ receiver.cpp -o receiver.out
send:
	g++ sender.cpp -o sender.out
clear:
	rm *.out
