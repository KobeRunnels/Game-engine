#include "character.h"

int pause(int he) // pauses for he number of seconds
{
    time_t then;
    
    time(&then);
    while(difftime(time(NULL),then) < he);
    fflush(stdin); // clears any extra enter presses
}

int main()
{
	srand((unsigned)time(NULL));
			
	SetDllDirectory("/.");
	string n;
	
	cout << "Enter your name:";
	cin >> n;
	
	player_character player = player_character(n);
	demon a = demon("Dave",2);
	
	party cast = party(&player);
	cast.addMember(&a);
	
	demon en = demon("Jack Frost", 2);
	demon dou = demon("Pyro Jack", 2);
	
	party enemy = party();
	enemy.addMember(&en);
	enemy.addMember(&dou);
	enemy.addMember(&en);
	enemy.addMember(&dou);
	enemy.addMember(&en);
	enemy.addMember(&dou);
	
	system("cls");
	battle fight = battle(&cast,&enemy);
	//cin >> n;
	
	return 0;
}