#include "pch.h"
#include <iostream>
#include <string>

using namespace std;

//The number of options available to the player
int numberOfOptions = 6;

//The amount of energy points gained through recharging
int rechargePoints = 10;

//The Monster object (used to create any and all monsters as a base)
class Monster {
public:
	int healthPoints = 100; //Monster health
	int energyPoints = 50; //Monster energy
	int rechargeRate = 100;
	int chanceToHit = 100;

	void attack(int chance, int damageMin, int damageMax) {
		//Work out if successfully attacks
		if (rand() % monster.chanceToHit < chance) {
			//Amount of damage done
			damage = rand() % damageMax + damageMin;
			dodgeChance = 0;
		}
	}

	void reCharge() {
		rechargeRate = 4 * rechargeRate;
	}

	void dodge() {

	}

	void heal() {

	}
};

int exit() {
	return 0;
}

void checkGameOver() {

}

//Direct the action number given to the correct procedure
void processChoice(Monster monster, int choice) {
	switch (choice) {
	case 1:
		//normalAttack
		monster.attack(80, 1, 10);
		break;
	case 2:
		//specialAttack
		monster.attack(50, 5, 20);
		break;
	case 3:
		monster.reCharge;
		break;
	case 4:
		monster.dodge;
		break;
	case 5:
		monster.heal;
		break;
	case 6:
		exit();
		break;
	default:
		std::cout << "Error - choice not covered in switch statement made it through vaidation.";
	}
}

//AI makes a random action choice
void computerTurn(Monster monster) {
	int random = rand() % (numberOfOptions - 1) + 1;
	processChoice(monster, random);
}

int main() {
	//Introduction of game to user
    std::cout << "Welcome to Monster Arena! The turn based strategy game where you get the chance to pit one of your monsters against an AI controlled monster! Enjoy, and good luck...\n"; 

	//Loop for while the player has not exited the game
	bool playing = true;
	while (playing) {
		//Create monsters
		Monster playerMonster;
		Monster computerMonster;

		//Offer options to player
		std::cout << "Enter the number that matches your action:\n";
		std::cout << "1 - Normal Attack\n";
		std::cout << "2 - Special Attack\n";
		std::cout << "3 - Re-Charge\n";
		std::cout << "4 - Dodge\n";
		std::cout << "5 - Heal\n";
		std::cout << "6 - Exit\n\n";

		//Get choice from player with validation
		bool makingChoice = true;
		int choice;
		while (makingChoice) {
			cin >> choice;

			if (choice > 0 && choice <= numberOfOptions) {
				makingChoice = false;
			}
			else {
				std::cout << "Sorry, that's not one of the options available. Please try again.\n";
			}
		}

		processChoice(playerMonster, choice);

		checkGameOver();
		computerTurn(computerMonster);
		checkGameOver();
	}
	exit();
}
