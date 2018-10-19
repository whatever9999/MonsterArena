#include "pch.h"
#include <iostream>
#include <string>
#include <conio.h>
#include <windows.h>
#include <limits>

#undef max

using namespace std;

//The number of options available to the player
CONST int numberOfOptions = 6;
CONST int healNumber = 5;

//The amount of energy points gained through recharging
CONST int rechargePoints = 10;

//Amount of time the program pauses for the user to process what's going on
CONST int timeToPause = 2;

//Basic program requirements: 
//Pausing for player to process info
//Exiting the program
void clearBuffer() {
	fflush(stdout);
}
void wait(long seconds) {
	seconds = seconds * 1000;
	Sleep(seconds);
}
void waitForUser() {
	clearBuffer();
	bool waiting = true;
	std::cout << "Enter 'c' to continue...\n";
	while (waiting) {
		char c = getchar();
		if (c == 'c') waiting = false;
	}
	std::cout << "\n";
}
void exit() {
	clearBuffer();
	bool waiting = true;
	std::cout << "Enter 'e' to exit...\n";
	while (waiting) {
		char e = getchar();
		if (e == 'e') waiting = false;
	}
	exit(0);
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
	void changeHealthDamage(int damage) {
		healthPoints -= damage;
	}
	void changeHealthHeal(int heal) {
		int maxHealth = 100;
		healthPoints += heal;
		if (healthPoints > maxHealth) healthPoints = maxHealth;
	}
	void changeEnergyUp(int up) {
		int maxEnergy = 50;
		energyPoints += up;
		if (energyPoints > maxEnergy) energyPoints = maxEnergy;
	}
	void changeEnergyDown(int down) {
		energyPoints -= down;
	}
	void changeRechargeRate(int newRechargeRate) {
		int maxRechargeRate;
		rechargeRate = newRechargeRate;
		if (rechargeRate > maxRechargeRate) rechargeRate = maxRechargeRate;
	}
	void changeChanceToHit(int difference) {
		int chanceToHitBoundary;
		chanceToHit += difference;
		if (chanceToHit > chanceToHitBoundary) chanceToHit = chanceToHitBoundary;
		else if (chanceToHit < -chanceToHitBoundary) chanceToHit = -chanceToHitBoundary;
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
	void attack(Monster &otherMonster, int chance, int damageMin, int damageMax, int energyCost) {
		//Give feedback to player as to what is happening
		std::cout << name << "is attacking!\n";
		wait(timeToPause);

		//Work out if successfully attacks
		if (rand() % 100 + 1 < chance + getChanceToHit()) {
			//Do damage to the other monster
			int damage = rand() % damageMax + damageMin;
			otherMonster.changeHealthDamage(damage);
			std::cout << "Successful hit!\n";
		} else std::cout << "Unsuccessful attack!\n";
		wait(timeToPause);

		//Subtract cost of attack and recharge energy points
		changeEnergyDown(energyCost);
		charge();
	}

	void reCharge(Monster &otherMonster) {
		int reChargeRateChange = 4;
		int chanceToHitChange = 10;

		//Give feedback to player as to what is happening
		std::cout << name << "is recharging!\n";
		wait(timeToPause);

		//Amp up recharge rate
		changeRechargeRate(reChargeRateChange * getRechargeRate());
		charge();
		changeRechargeRate(getRechargeRate() / reChargeRateChange);
		
		//Since monster is standing still, increase other monster's chance to hit on next turn then charge up
		otherMonster.changeChanceToHit(chanceToHitChange);
	}

	void dodge(Monster  &otherMonster) {
		int reChargeRateChange = 0.5;
		int chanceToHitChange = -30;
		int originalRate = getRechargeRate();

		//Give feedback to player as to what is happening
		std::cout << name << "is dodging!\n";
		wait(timeToPause);

		//Since monster is using energy next turn, reduce recharge rate, ensuring that the result is an integer and never reaches 0
		bool odd = false;
		if (getRechargeRate() % 2 == 0) {
			changeRechargeRate((int)(originalRate * reChargeRateChange));
		}
		else {
			changeRechargeRate((int)((originalRate + 1) * reChargeRateChange));
			odd = true;
		}
		charge();
		if(!odd) changeRechargeRate(originalRate);
		else changeRechargeRate(originalRate);

		//Decrease the other monster's chance to hit due to dodge
		otherMonster.changeChanceToHit(chanceToHitChange);
	}

	void heal() {
		int healingPercentage = 0.25;

		//Give feedback to player as to what is happening
		std::cout << name << "is healing!\n";
		wait(timeToPause);

		//Find amount to be healed by then move this amount from energy to health points
		int amount = rand() % (energyPoints * healingPercentage) + 1;
		changeHealthHeal(amount);
		changeEnergyDown(amount);
		changeHasHealed(true);
	}

	void charge() {
		//Increase energy points by the amount in the recharge
		changeEnergyUp(getRechargeRate());
	}
};

void checkGameOver(Monster playerMonster, Monster computerMonster) {
	//Check to see if either monster's health is 0
	bool gameOver = false;
	if (playerMonster.getHealth() <= 0 || computerMonster.getHealth() <= 0) {
		gameOver = true;
		std::cout << "Game Over!\n";
		if (computerMonster.getHealth() == 0) std::cout << "Well done!\n";
		else std::cout << "Sorry, you lost. Try again maybe!\n";
		exit();
	}
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

	waitForUser();
}

//Direct the action number given to the correct procedure
boolean processChoice(Monster &monster, Monster &otherMonster, int choice) {
	switch (choice) {
	case 1:
		//normalAttack
		if (monster.getEnergy() < 20) return false;
		monster.attack(otherMonster, 80, 1, 10, 20);
		break;
	case 2:
		//specialAttack
		if (monster.getEnergy() < 50) return false;
		monster.attack(otherMonster, 50, 5, 20, 50);
		break;
	case 3:
		monster.reCharge(otherMonster);
		break;
	case 4:
		monster.dodge(otherMonster);
		break;
	case 0:
		exit(0);
		break;
	default:
		std::cout << "Error - choice not covered in switch statement made it through vaidation.\n";
	}
	return true;
}

//AI makes a random action choice
int computerChoice(Monster &playerMonster, Monster &computerMonster) {
	//Ensures that AI monster doesn't heal twice in a row
	bool gotRandom = false;
	int random = 0;

	while (!gotRandom) {
		random = rand() % (numberOfOptions - 1) + 1;

		//AI's turn processing (checks to see if monster heals as this is a special case where two moves may be made in one turn)
		if (random == healNumber && !computerMonster.getHasHealed()) {
			gotRandom = true;
			computerMonster.heal();
		}
		else if (random != healNumber) {
			gotRandom = true;
			bool canAfford = processChoice(computerMonster, playerMonster, random);
			if (canAfford = false) std::cout << "You didn't have enough energy points to make that move! You miss your turn!";
		}
	}

	return random;
}

int playerChoice(Monster playerMonster) {
	//Offer options to player
	std::cout << "Enter the number that matches your action:\n";
	std::cout << "1 - Normal Attack (Costs 20 energy points)\n";
	std::cout << "2 - Special Attack (Costs 50 energy points)\n";
	std::cout << "3 - Re-Charge\n";
	std::cout << "4 - Dodge\n";
	std::cout << healNumber << " - Heal\n";
	std::cout << "0 - Exit\n\n";

	//Get choice from player with validation
	bool makingChoice = true;
	int choice;
	while (makingChoice) {
		clearBuffer();
		cin >> choice;

		if (choice >= 0 && choice <= numberOfOptions) { 
			makingChoice = false;
		}
		else {
			std::cout << "Sorry, that's not one of the options available. Please try again.\n";
		}
	}
	return choice;
}


void playerTurn(Monster &playerMonster, Monster &computerMonster, int choice) {
	//Player's turn processing (checks to see if monster heals as this is a special case where two moves may be made in one turn)
	//Check that player can afford choice with current energy points
	bool canAfford = true;

	if (choice == healNumber && !playerMonster.getHasHealed()) {
		playerMonster.heal();
	}
	else if (choice == healNumber) {
		std::cout << "You've already healed! You miss your second move!";
		playerMonster.changeHasHealed(false);
	}
	else {
		canAfford = processChoice(playerMonster, computerMonster, choice);
		if (canAfford = false) std::cout << "You didn't have enough energy points to make that move! You miss your turn!";
		playerMonster.changeHasHealed(false);
	}

	//Checks to see if either monster is dead
	checkGameOver(playerMonster, computerMonster);
}

void computerTurn(Monster playerMonster, Monster computerMonster, int choice) {
	//Inform player that it is the computer's turn
	std::cout << computerMonster.getName() << "is taking their turn!\n";
	wait(timeToPause);

	//Computer's turn processing
	if (choice == healNumber) {
		computerMonster.heal();
	}
	else {
		processChoice(playerMonster, computerMonster, choice);
		computerMonster.changeHasHealed(false);
	}
	
	//Checks if either monster is dead
	checkGameOver(playerMonster, computerMonster);
}

void round(Monster &playerMonster, Monster &computerMonster) {
	//Loop for while the player has not exited the game
	while (true) {
		//Get player's choice and go through their move
		int choice = playerChoice(playerMonster);
		playerTurn(playerMonster, computerMonster, choice);

		//Updates user on state of both monsters
		giveFeedback(playerMonster, computerMonster);

		//If player healed first they get another move
		if (choice == healNumber) {
			std::cout << "You get another move since you healed for your first one this turn!\n";
			choice = playerChoice(playerMonster);
			playerTurn(playerMonster, computerMonster, choice);

			//Updates user on state of both monsters
			giveFeedback(playerMonster, computerMonster);
		}

		//Get computer's choice and go through the move
		choice = computerChoice(playerMonster, computerMonster);
		computerTurn(playerMonster, computerMonster, choice);

		//Updates user on state of both monsters
		giveFeedback(playerMonster, computerMonster);

		//The computer gets another move since it healed first
		if (choice == healNumber) {
			std::cout << "The AI gets another move since it healed for its first one this turn!\n";
			choice = computerChoice(playerMonster, computerMonster);
			computerTurn(playerMonster, computerMonster, choice);

			//Updates user on state of both monsters
			giveFeedback(playerMonster, computerMonster);
		}
	}
}

int main() {
	//Introduction of game to user
    std::cout << "Welcome to Monster Arena! The turn based strategy game where you get the chance to pit one of your monsters against an AI controlled monster! Enjoy, and good luck...\n"; 

	//Create monsters
	Monster playerMonster;
	playerMonster.initialise("Your Monster ", 100, 50, 10, 0);
	Monster computerMonster;
	computerMonster.initialise("The AI ", 100, 50, 10, 0);

	round(playerMonster, computerMonster);

	return 0;
}
