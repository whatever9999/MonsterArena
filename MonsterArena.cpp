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

//Basic program requirements: 
//Pausing for player to process info
//Exiting the program
void wait(long seconds) {
	seconds = seconds * 1000;
	Sleep(seconds);
}
int exit() {
	return 0;
}

//The Monster object (used to create any and all monsters as a base)
class Monster {
private:
	string name; //Monster name
	int healthPoints; //Monster health
	int energyPoints; //Monster energy
	int rechargeRate; //Amount of recharge per turn
	int chanceToHit; //Increase on chance to hit
	bool hasHealed; //The monster has already healed this turn

public:
	//Access to private variables
	string getName() {
		return name;
	}
	int getHealth() {
		return healthPoints;
	}
	int getEnergy() {
		return energyPoints;
	}
	int getRechargeRate() {
		return rechargeRate;
	}
	int getChanceToHit() {
		return chanceToHit;
	}
	bool getHasHealed() {
		return hasHealed;
	}

	//Changing private variables
	void changeName(string newName) {
		name = newName;
	}
	void changeHealth(int damage) {
		healthPoints -= damage;
	}
	void changeEnergy(int newEnergy) {
		energyPoints = newEnergy;
	}
	void changeRechargeRate(int newRechargeRate) {
		rechargeRate = newRechargeRate;
	}
	void changeChanceToHit(int difference) {
		chanceToHit += difference;
	}
	void changeHasHealed(bool newHasHealed) {
		hasHealed = newHasHealed;
	}

	//Set start values for monster's variables
	void initialise(string startName, int startHealth, int startEnergy, int startRechargeRate, int startChanceToHit) {
		name = startName; //Monster name
		healthPoints = startHealth; //Monster health
		energyPoints = startEnergy; //Monster energy
		rechargeRate = startRechargeRate; //Amount of recharge per turn
		chanceToHit = startChanceToHit; //Increase on chance to hit
		hasHealed = false; //If the monster has already healed this turn
	}

	//Monster actions
	void attack(Monster & otherMonster, int chance, int damageMin, int damageMax, int energyCost) {
		//Give feedback to player as to what is happening
		std::cout << name << "is attacking!\n";
		wait(2);

		//Work out if successfully attacks
		if (rand() % chance < chance) {
			//Do damage to the other monster
			int damage = rand() % damageMax + damageMin;
			otherMonster.changeHealth(damage);
			std::cout << "Successful hit!\n";
		} else std::cout << "Unsuccessful attack!\n";
		//Subtract cost of attack and recharge energy points
		energyPoints -= energyCost;
		charge();
	}

	void reCharge(Monster & otherMonster) {
		//Give feedback to player as to what is happening
		std::cout << name << "is recharging!\n";
		wait(2);

		//Amp up recharge rate
		rechargeRate = 4 * rechargeRate;
		
		//Since monster is standing still, increase other monster's chance to hit on next turn then charge up
		otherMonster.changeChanceToHit(10);
		charge();
	}

	void dodge(Monster & otherMonster) {
		//Give feedback to player as to what is happening
		std::cout << name << "is dodging!\n";
		wait(2);

		//Since monster is using energy next turn, reduce recharge rate, ensuring that the result is an integer and never reaches 0
		if (rechargeRate % 2 == 0) rechargeRate = (int)(rechargeRate * 0.5);
		else rechargeRate = (int)((rechargeRate + 1) * 0.5);

		//Decrease the other monster's chance to hit due to dodge
		otherMonster.changeChanceToHit(-30);

		//Take away cost and charge up
		energyPoints -= 20;
		charge();
	}

	void heal(Monster otherMonster) {
		//Give feedback to player as to what is happening
		std::cout << name << "is healing!\n";
		wait(2);

		//Find amount to be healed by then move this amount from energy to health points
		int amount = rand() % (energyPoints / 2) + 1;
		healthPoints += amount;
		energyPoints -= amount;
		hasHealed = true;
	}

	void charge() {
		//Increase energy points by the amount in the recharge
		energyPoints += rechargeRate;
	}
};

bool checkGameOver(Monster playerMonster, Monster computerMonster) {
	//Check to see if either monster's health is 0
	bool gameOver = false;
	if (playerMonster.getHealth() == 0 || computerMonster.getHealth() == 0) gameOver = true;

	if (gameOver == true) {
		std::cout << "Game Over!\n";
		if (playerMonster.getHealth() == 0) std::cout << "Well done!\n";
		else std::cout << "Sorry, you lost. Try again maybe!\n";
	}

	return gameOver;
}

void giveFeedback(Monster playerMonster, Monster computerMonster) {
	//Feedback on player monster's health points, energy points, recharge rate, chance to hit and weather or not can heal next move
	std::cout << "\n\n" << playerMonster.getName() << ":\n\n";
	std::cout << "Health Points: " << playerMonster.getHealth() << "\n";
	std::cout << "Energy Points: " << playerMonster.getEnergy() << "\n";
	std::cout << "Recharge Rate: " << playerMonster.getRechargeRate() << "\n";
	std::cout << "Amount chance to hit has been affected by: " << playerMonster.getChanceToHit() << "%\n";
	string canHeal = "";
	if (playerMonster.getHasHealed()) canHeal = "No";
	else canHeal = "Yes";
	std::cout << "Can heal next move: " << canHeal << "\n";

	//Feedback for computer's monster
	std::cout << "\n" << computerMonster.getName() << ":\n\n";
	std::cout << "Health Points: " << computerMonster.getHealth() << "\n";
	std::cout << "Energy Points: " << computerMonster.getEnergy() << "\n";
	std::cout << "Recharge Rate: " << computerMonster.getRechargeRate() << "\n";
	std::cout << "Amount chance to hit has been affected by: " << computerMonster.getChanceToHit() << "%\n";
	if (computerMonster.getHasHealed()) canHeal = "No";
	else canHeal = "Yes";
	std::cout << "Can heal next move: " << canHeal << "\n\n\n";
}

//Direct the action number given to the correct procedure
void processChoice(Monster & monster, Monster & otherMonster, int choice) {
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
int computerTurn(Monster & monster, Monster & otherMonster) {
	//Ensures that AI monster doesn't heal twice in a row
	bool gotRandom = false;
	int random = 0;

	while (!gotRandom) {
		random = rand() % (numberOfOptions - 1) + 1;

		//AI's turn processing (checks to see if monster heals as this is a special case where two moves may be made in one turn)
		if (random == 5 && !monster.getHasHealed()) {
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

int playerTurn(Monster & playerMonster) {
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
		cin.sync();

		if (choice > 0 && choice <= numberOfOptions) {
			makingChoice = false;
		}
		else {
			std::cout << "Sorry, that's not one of the options available. Please try again.\n";
		}

		//Check if has already healed/has just healed
		if (playerMonster.getHasHealed() && choice == 5) {
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
		playerMonster.initialise("Your Monster ", 100, 50, 10, 0);
		Monster computerMonster;
		computerMonster.initialise("The AI ", 100, 50, 10, 0);

		int choice = playerTurn(playerMonster);

		//Player's turn processing (checks to see if monster heals as this is a special case where two moves may be made in one turn)
		if(choice == 5) playerMonster.heal(computerMonster);
		else processChoice(playerMonster, computerMonster, choice);

		//Give the player feedback on the state of both monsters
		giveFeedback(playerMonster, computerMonster);
		wait(2);

		//Check to see if last turn resulted in a win then process computer's choice and check again
		//If either player heals they are given a second move
		playing = !checkGameOver(playerMonster, computerMonster);

		if (choice == 5) {
			std::cout << "You get another move since you healed for your first one this turn!\n";
			choice = playerTurn(playerMonster);
			processChoice(playerMonster, computerMonster, choice);
			playerMonster.changeHasHealed(false);
			giveFeedback(playerMonster, computerMonster);
			wait(2);
		}
		
		std::cout << computerMonster.getName() << "is taking their turn!\n";
		wait(2);
		int computerChoice = computerTurn(computerMonster, playerMonster);

		giveFeedback(playerMonster, computerMonster);
		wait(2);

		if (computerChoice == 5) {
			computerTurn(computerMonster, playerMonster);
			computerMonster.changeHasHealed(false);

			giveFeedback(playerMonster, computerMonster);
			wait(2);
		}

		playing = !checkGameOver(playerMonster, computerMonster);
	}
	exit();
}
