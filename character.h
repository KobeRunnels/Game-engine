#include <string.h>
#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <random>
#include <windows.h>
#include <string.h>
#include <vector>
#include <conio.h>

using namespace std;

class DNA;
class ability;
class weapon;

class character;
class demon;
class player_character;

class party;
class battle;

void hideCursor() // hides the cursor so the player can't see it
{
   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 1;
   info.bVisible = FALSE;
   SetConsoleCursorInfo(consoleHandle, &info);
}

void preEnter() // for spacing out dialogue and prompts
{
	printf("\nPress enter");
	getchar();
	fflush(stdin); // clears any extra enter presses
	fseek(stdin,0,SEEK_END);
}

void setColor(int ForgC)
{
 WORD wColor;

	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;

    //We use csbi for the wAttributes word.
	if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
	{
        //Mask out all but the background attribute, and add in the forgournd color
		wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F); // 0xF0 is red
		SetConsoleTextAttribute(hStdOut, wColor);
	}
	return;
}

void setCursor(short int x, short int y) // sets the cursor to a position on the screen 
{
	hideCursor();
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD p = {x,y};
    SetConsoleCursorPosition(h,p);
}	

class DNA // class for DNA handling in humans 
{
	public:
		DNA()
		{
		}
	private:
		int strand[8] = {0,0,0,0,0,0,0,0};
		int humanity = 100;
};	

class ability // class for abilities and handling them
{
	public:
		string name;
		ability()
		{
			name = "";
			staminaCost = 0;
			healthCost = 0;
			type = 0;
		}
	private:
		int staminaCost;
		int healthCost;
		int type;
		int rank;
		int damage;
};	

class weapon // class for weapons
{
	public:
		string name;
		weapon()
		{
			type = 0;
			ammo_cost = 0;
			strength = 0;
		}	
	private:
		int type;
		int ammo_cost;
		int strength;
};	

class character
{
	public:
		string name;
		int health; // health value
		bool withPlayer = false;
		bool backRow = false; // In combat is the character in the back row?

		character()
		{
		}
		character(string n, int race)
		{
			
			name = n;
			switch(race)
			{
				case 1: // human
				attack = 1;
				defense = 1;
				agility = 1;
				
				maxMP = 10;
				maxAtt = 5;
				maxAgi = 5;
				maxDef = 5;
				
				health = 10;
				maxHealth = 10;
				
				maxStamina = 8;
				stamina = 8;
				
				scars = 0;
				break;
				default:
				attack = rand()%4+1;
				defense = rand()%4+1;
				agility = rand()%4+1;
				
				maxMP = rand()%10+5;
				maxAtt = rand()%10+5;
				maxAgi = rand()%10+5;
				maxDef = rand()%10+5;
				
				maxStamina = rand()%10+8;
				stamina = maxStamina;
				
				maxHealth = rand()%5+6;
				health = maxHealth;
				break;
			}
		}
		void printInfo()
		{
			if(withPlayer)
				cout << name << " HP:" << health << "/" << maxHealth << " STM:" << stamina << "/" << maxStamina;
			else
				cout << name;
		}
		virtual void turn(party * enemySide){} // for turns in battle
		int getAgility() { return agility;}
	private:
		int attack; // attack value
		int mp; // muscle points
		int maxMP; // max musclepoints
		int maxAtt; // max attack value
		
		int defense; // defense value 
		int scars; // scars
		int maxDef; // max defense value
		
		int agility; // agility value
		int maxAgi; // max agility value
		
		int stamina; // stamina value
		int maxStamina; // max stamina
		
		int maxHealth; // max health
		ability moves[2]; // abilities
		weapon * currentWpn = nullptr; // weapon the character is holding

};

// demon class
class demon : public character
{
	public:
		demon()
		{
		}
		demon(string n, int r): character(n,r)
		{
			
		}
		void turn(party * enemySide) override
		{
			if(withPlayer)
			{
			}
			else
			{
			}
		}
	private:
		int loyalty;
		int personality;
		bool pactPartner;
};

// player character (human) class
class player_character : public character
{
	public:
		player_character(string n) : character(n,1)
		{
			
		}
		void turn(party * enemySide) override
		{
			//cout << "TURN P";
		}
	private:
		int cash = 100;
		weapon stash[10]; // the weapons the player has
		DNA player = DNA(); // Player character's DNA
		demon owned[10]; // demons owned by the player
};

class party // class for handling party compositions
{
	public:
		character * turnOrder[8];
		party() // initialize non-player party 
		{
			playerTeam = false;
			size = 0;
		}
		party(player_character * a) // initialize player character party 
		{
			playerTeam = true;
			addMember(a);
		}
		void addMember(character * a) // adds member to the party
		{
			if(playerTeam == true)
			{
				a->withPlayer = true;
			}
			if(size < 4)
			{
				front[size] = a;
				a->backRow = false;
			}
			else if(size < 8)
			{
				back[size%4] = a;
				a->backRow = true;
			}
			size++;
		}
		bool isAlive() // checks if the party is alive (if there's at least one member alive)
		{
			bool alive = false;
			for(int i=0;i<size;i++)
			{
				if(i<4)
				{
					if(front[i]->health > 0)
						alive = true;
				}
				else
				{
					if(back[i%4]->health > 0)
						alive = true;
				}
			}
			return alive;
		}
		void print() // prints every character in the party 
		{
			for(int i=0;i<size;i++)
			{
				if(i < 4)
					cout << i << " " << front[i]->name << " " << front[i]->getAgility();
				else
					cout << i << " " << back[i%4]->name << " " << back[i%4]->getAgility();	
				cout << "\n";
			}
		}
		void sortOrder()
		{
			vector<character *> oldOrder;
			int minAt;
			int minval;
			character * y;
			for(int i=0;i<size;i++)
			{
				if(i < 4)
					oldOrder.push_back(front[i]);
				else
					oldOrder.push_back(back[i%4]);
				if(i==0 || minval > oldOrder.at(i)->getAgility())
				{
					minAt = i;
					minval = oldOrder.at(i)->getAgility();
				}
			}	
			turnOrder[0] = oldOrder.at(minAt);
			y = oldOrder.at(oldOrder.size()-1);
			oldOrder.at(minAt) = y;
			oldOrder.pop_back();
			for(int i=1;i<size;i++)
			{
				minval = oldOrder.at(0)->getAgility();
				minAt = 0;
				for(int j=0;j<oldOrder.size();j++)
				{
					if(minval > oldOrder.at(j)->getAgility())
					{
						minAt = j;
						minval = oldOrder.at(j)->getAgility();
					}
				}
				turnOrder[i] = oldOrder.at(minAt);
				y = oldOrder.at(oldOrder.size()-1);
				oldOrder.at(minAt) = y;
				oldOrder.pop_back();
			}
		}
		void printTurnOrder()
		{
			for(int i=0;i<size;i++)
			{
				cout << i << " " << turnOrder[i]->name << " " << turnOrder[i]->getAgility() << "\n";
			}
		}
		character * getMember(int local)
		{
			if(local < 4)
				return front[local];
			else 
				return back[local%4];
		}
		int getSize() {return size;}
	private:
		character * front[4];
		character * back[4];
		int size = 0;
		bool playerTeam = false;
		int playerPos;
};

class battle // main class for handling combat
{
	public:
		battle(party * player, party * enemy) // main combat function/constructor 
		{
			string n;
			
			character * currentPlayer;
			playerSide = player;
			enemySide = enemy;
			
			size += player->getSize();
			size += enemy->getSize();
			
			playerSide->sortOrder();
			enemySide->sortOrder();
			
			sortOrder();
			printTurnOrder();
			
			turnCounter = 0;
			
			printSides();
			// Main combat loop
			while(playerSide->isAlive() && enemySide->isAlive())
			{
				currentPlayer = turnOrder[turnCounter];
				if(currentPlayer->health > 0)
				{
					sortOrder();
					printTurnOrder();
					setCursor(1,15);
					if(currentPlayer->withPlayer == true)
						currentPlayer->turn(enemySide);
					else
						currentPlayer->turn(playerSide);	
					setCursor(1,17);
					cin >> n;
					turns++;
				}
				if(turnCounter == size-1) // resets turn order back to the first character at max
					turnCounter = -1;
				turnCounter++;
			}
		}
		void printSides() // prints characters and their positions
		{
			setCursor(1,0);
			cout << "Player Team:";
			setCursor(40,0);
			cout << "Enemy Team";
			for(int i=0;i<playerSide->getSize();i++)
			{
				if(i==0)
				{
					setCursor(16,2);
					cout << "Front:";
					
					setCursor(1,2);
					cout << "Back:";
				}
				if(i<4)
				{
					setCursor(16,4+i);
					cout << playerSide->getMember(i)->name;
				}
				else
				{
					setCursor(1,4+(i%4));
					cout << playerSide->getMember(i)->name;	
				}
			}
			for(int i=0;i<enemySide->getSize();i++)
			{
				if(i==0)
				{
					setCursor(40,2);
					cout << "Front:";
					
					setCursor(56,2);
					cout << "Back:";
				}
				if(i<4)
				{
					setCursor(40,4+i);
					cout << enemySide->getMember(i)->name;
				}
				else
				{
					setCursor(56,4+(i%4));
					cout << enemySide->getMember(i)->name;	
				}
			}
		}
		void sortOrder() // sorts the turn order for every character in combat
		{
			vector<character *> oldOrder;
			int minAt;
			int minval;
			character * y;
			for(int i=0;i<playerSide->getSize();i++)
			{
				y = playerSide->getMember(i);
				oldOrder.push_back(y);
				if(i==0 || minval > oldOrder.at(i)->getAgility())
				{
					minAt = i;
					minval = oldOrder.at(i)->getAgility();
				}
			}	
			for(int i=0;i<enemySide->getSize();i++)
			{
				y = enemySide->getMember(i);
				oldOrder.push_back(y);
				if(i==0 || minval > oldOrder.at(i)->getAgility())
				{
					minAt = i;
					minval = oldOrder.at(i)->getAgility();
				}
			}
			for(int i=0;i<size;i++)
			{
				minval = oldOrder.at(0)->getAgility();
				minAt = 0;
				for(int j=0;j<oldOrder.size();j++)
				{
					if(minval > oldOrder.at(j)->getAgility())
					{
						minAt = j;
						minval = oldOrder.at(j)->getAgility();
					}
				}
				turnOrder[i] = oldOrder.at(minAt);
				y = oldOrder.at(oldOrder.size()-1);
				oldOrder.at(minAt) = y;
				oldOrder.pop_back();
			}
		}
		void printTurnOrder() // prints order of character actions in the game
		{
			setCursor(100,0);
			cout << "Turn Order";	
			for(int i=0;i<size;i++)
			{
				
				setCursor(88,i+1);
				if(turnCounter == i)	
					cout << ">";
				else
					cout << " ";
				setCursor(90,i+1);
				cout << i << " ";
				if(turnOrder[i]->withPlayer)
					setColor(1);
				else 
					setColor(4);
				turnOrder[i]->printInfo();
				setColor(15);
				cout << "\n";
						
			}
		}
	private:
		int turns = 0; // keeps track of the number of turns in the combat in total
		int turnCounter = 0; // keeps track of which character in the turn order is going 
		party * playerSide; // player team, keeps track of characters on the player side
		party * enemySide; // keeps track of characters on the enemy side
		character * turnOrder[16]; // general array of pointers to characters in the turn order
		int size = 0; // the number of characters in combat
};