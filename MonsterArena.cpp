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
	int rechargeRate = 10; //Amount of recharge per turn
	int chanceToHit = 0; //Increase on chance to hit

	void attack(Monster otherMonster, int chance, int damageMin, int damageMax, int energyCost) {
		//Work out if successfully attacks
		if (rand() % 100 < chance) {
			//Do damage to the other monster
			int damage = rand() % damageMax + damageMin;
			otherMonster.healthPoints -= damage;
		}
		//Subtract cost of attack and recharge energy points
		energyPoints -= energyCost;
		charge();
	}

	void reCharge(Monster otherMonster) {
		//Amp up recharge rate
		rechargeRate = 4 * rechargeRate;
		
		//Since monster is standing still, increase other monster's chance to hit on next turn then charge up
		otherMonster.chanceToHit += 10;
		charge();
	}

	void dodge(Monster otherMonster) {
		//Since monster is using energy next turn, reduce recharge rate
		rechargeRate = rechargeRate * 0.5;

		//Decrease the other monster's chance to hit due to dodge
		otherMonster.chanceToHit -= 30;

		//Take away cost and charge up
		energyPoints -= 20;
		charge();
	}

	void heal(Monster otherMonster) {
		//Find amount to be healed by then move this amount from energy to health points
		int amount = rand() % (energyPoints / 2);
		healthPoints += amount;
		energyPoints -= amount;
	}

	void charge() {
		//Increase energy points by the amount in the recharge
		energyPoints += rechargeRate;
	}
};

int exit() {
	return 0;
}

void checkGameOver() {

}

//Direct the action number given to the correct procedure
void processChoice(Monster monster, Monster otherMonster, int choice) {
	switch (choice) {
	case 1:
		//normalAttack
		monster.attack(otherMonster, 80, 1, 10, 20);
		break;
	case 2:
		//specialAttack
		monster.attack(otherMonster, 50, 5, 20, 50);
		break;
	case 3:
		monster.reCharge(otherMonster);
		break;
	case 4:
		monster.dodge(otherMonster);
		break;
	case 5:
		monster.heal(otherMonster);
		break;
	case 6:
		exit();
		break;
	default:
		std::cout << "Error - choice not covered in switch statement made it through vaidation.";
	}
}

//AI makes a random action choice
void computerTurn(Monster monster, Monster otherMonster) {
	int random = rand() % (numberOfOptions - 1) + 1;
	processChoice(monster, otherMonster, random);
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

			//Check if has already healed/has just healed
			if (choice == 5 && hasHealed!) hasHealed = true;
			if (choice == 5 && hashealed) std::cout << "You've already healed this turn! Please try another move.\n";
		}

		//Player's turn processing
		processChoice(playerMonster, computerMonster, choice);

		//Check to see if last turn resulted in a win then process computer's choice and check again
		checkGameOver();
		computerTurn(computerMonster, playerMonster);
		checkGameOver();
	}
	exit();
}
