#include "pch.h"
#include <iostream>
#include <string>
#include <conio.h>
#include <windows.h>

using namespace std;

//The number of options available to the player
CONST int numberOfOptions = 6;

//The amount of energy points gained through recharging
CONST int rechargePoints = 10;

//The Monster object (used to create any and all monsters as a base)
class Monster {
public:
	int healthPoints = 100; //Monster health
	int energyPoints = 50; //Monster energy
	int rechargeRate = 10; //Amount of recharge per turn
	int chanceToHit = 0; //Increase on chance to hit
	bool hasHealed = false; //If the monster has already healed this turn

	void attack(Monster otherMonster, int chance, int damageMin, int damageMax, int energyCost) {
		//Work out if successfully attacks
		if (rand() % 100 < chance) {
			//Do damage to the other monster
			int damage = rand() % damageMax + damageMin;
			otherMonster.healthPoints -= damage;
			std::cout << "Successful hit!\n";
		} else std::cout << "Unsuccessful attack!\n";
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
		//Since monster is using energy next turn, reduce recharge rate, ensuring that the result is an integer and never reaches 0
		if (rechargeRate % 2 == 0) rechargeRate = (int)(rechargeRate * 0.5);
		else rechargeRate = (int)((rechargeRate + 1) * 0.5);

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
		hasHealed = true;
	}

	void charge() {
		//Increase energy points by the amount in the recharge
		energyPoints += rechargeRate;
	}
};

void wait(long seconds) {
	seconds = seconds * 1000;
	Sleep(seconds);
}

int exit() {
	return 0;
}

bool checkGameOver(Monster playerMonster, Monster computerMonster) {
	//Check to see if either monster's health is 0
	bool gameOver = false;
	if (playerMonster.healthPoints == 0 || computerMonster.healthPoints == 0) gameOver = true;

	if (gameOver == true) {
		std::cout << "Game Over!\n";
		if (playerMonster.healthPoints == 0) std::cout << "Well done!\n";
		else std::cout << "Sorry, you lost. Try again maybe!\n";
	}

	return gameOver;
}

void giveFeedback(Monster playerMonster, Monster computerMonster) {
	//Feedback on player monster's health points, energy points, recharge rate, chance to hit and weather or not can heal next move
	std::cout << "\n\nPlayer Monster:\n\n";
	std::cout << "Health Points: " << playerMonster.healthPoints << "\n";
	std::cout << "Energy Points: " << playerMonster.rechargeRate << "\n";
	std::cout << "Amount chance to hit has been affected by: " << playerMonster.chanceToHit << "%\n";
	string canHeal = "";
	if (playerMonster.hasHealed) canHeal = "No";
	else canHeal = "Yes";
	std::cout << "Can heal next move: " << canHeal << "\n";

	//Feedback for computer's monster
	std::cout << "\nOther Monster:\n";
	std::cout << "Health Points: " << computerMonster.healthPoints << "\n";
	std::cout << "Energy Points: " << computerMonster.rechargeRate << "\n";
	std::cout << "Amount chance to hit has been affected by: " << computerMonster.chanceToHit << "%\n";
	if (computerMonster.hasHealed) canHeal = "No";
	else canHeal = "Yes";
	std::cout << "Can heal next move: " << canHeal << "\n\n\n";
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
	case 6:
		exit();
		break;
	default:
		std::cout << "Error - choice not covered in switch statement made it through vaidation.\n";
	}
}

//AI makes a random action choice
int computerTurn(Monster monster, Monster otherMonster) {
	//Ensures that AI monster doesn't heal twice in a row
	bool gotRandom = false;
	int random = 0;

	while (!gotRandom) {
		random = rand() % (numberOfOptions - 1) + 1;

		//AI's turn processing (checks to see if monster heals as this is a special case where two moves may be made in one turn)
		if (random == 5 && !monster.hasHealed) {
			gotRandom = true;
			monster.heal(otherMonster);
		}
		else if (random != 5) {
			gotRandom = true;
			processChoice(monster, otherMonster, random);
		}
	}

	return random;
}

int playerTurn(Monster playerMonster) {
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
		cin.clear();
		fflush(stdin);

		if (choice > 0 && choice <= numberOfOptions) {
			makingChoice = false;
		}
		else {
			std::cout << "Sorry, that's not one of the options available. Please try again.\n";
		}

		//Check if has already healed/has just healed
		if (playerMonster.hasHealed) {
			std::cout << "Sorry, you have already healed this turn. Please make another move.\n";
			makingChoice = true;
		}
	}
	return choice;
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

		int choice = playerTurn(playerMonster);

		//Player's turn processing (checks to see if monster heals as this is a special case where two moves may be made in one turn)
		if(choice == 5) playerMonster.heal(computerMonster);
		else processChoice(playerMonster, computerMonster, choice);

		//Give the player feedback on the state of both monsters
		giveFeedback(playerMonster, computerMonster);

		//Check to see if last turn resulted in a win then process computer's choice and check again
		//If either player heals they are given a second move
		playing = !checkGameOver(playerMonster, computerMonster);

		if (choice == 5) {
			playerTurn(playerMonster);
			playerMonster.hasHealed = false;
			giveFeedback(playerMonster, computerMonster);
		}
		
		std::cout << "The other player is taking their turn!\n";
		wait(2);
		int computerChoice = computerTurn(computerMonster, playerMonster);
		giveFeedback(playerMonster, computerMonster);
		giveFeedback(playerMonster, computerMonster);
		if (computerChoice == 5) {
			computerTurn(computerMonster, playerMonster);
			computerMonster.hasHealed = false;
			giveFeedback(playerMonster, computerMonster);
		}
		playing = !checkGameOver(playerMonster, computerMonster);
	}
	exit();
}
