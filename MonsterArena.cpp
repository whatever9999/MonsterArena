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

//The amount of energy points gained through recharging
CONST int rechargePoints = 10;

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
	void changeHealthDamage(int damage) {
		healthPoints -= damage;
	}
	void changeHealthHeal(int heal) {
		healthPoints += heal;
		if (healthPoints > 100) healthPoints = 100;
	}
	void changeEnergyUp(int up) {
		energyPoints += up;
		if (energyPoints > 50) energyPoints = 50;
	}
	void changeEnergyDown(int down) {
		energyPoints -= down;
	}
	void changeRechargeRate(int newRechargeRate) {
		rechargeRate = newRechargeRate;
		if (rechargeRate > 40) rechargeRate = 40;
	}
	void changeChanceToHit(int difference) {
		chanceToHit += difference;
		if (chanceToHit > 40) chanceToHit = 40;
		else if (chanceToHit < -40) chanceToHit = -40;
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
		wait(2);

		//Work out if successfully attacks
		if (rand() % 100 + 1 < chance + getChanceToHit()) {
			//Do damage to the other monster
			int damage = rand() % damageMax + damageMin;
			otherMonster.changeHealthDamage(damage);
			std::cout << "Successful hit!\n";
		} else std::cout << "Unsuccessful attack!\n";
		wait(2);

		//Subtract cost of attack and recharge energy points
		changeEnergyDown(energyCost);
		charge();
	}

	void reCharge(Monster &otherMonster) {
		//Give feedback to player as to what is happening
		std::cout << name << "is recharging!\n";
		wait(2);

		//Amp up recharge rate
		changeRechargeRate(4 * getRechargeRate());
		charge();
		changeRechargeRate(getRechargeRate() / 4);
		
		//Since monster is standing still, increase other monster's chance to hit on next turn then charge up
		otherMonster.changeChanceToHit(10);
	}

	void dodge(Monster  &otherMonster) {
		//Give feedback to player as to what is happening
		std::cout << name << "is dodging!\n";
		wait(2);

		//Since monster is using energy next turn, reduce recharge rate, ensuring that the result is an integer and never reaches 0
		bool odd = false;
		if (getRechargeRate() % 2 == 0) {
			changeRechargeRate((int)(getRechargeRate() * 0.5));
		}
		else {
			changeRechargeRate((int)((getRechargeRate() + 1) * 0.5));
			odd = true;
		}
		charge();
		if(!odd) changeRechargeRate(getRechargeRate() * 2);
		else changeRechargeRate(getRechargeRate() * 2 - 1);

		//Decrease the other monster's chance to hit due to dodge
		otherMonster.changeChanceToHit(-30);
	}

	void heal() {
		//Give feedback to player as to what is happening
		std::cout << name << "is healing!\n";
		wait(2);

		//Find amount to be healed by then move this amount from energy to health points
		int amount = rand() % (energyPoints / 2) + 1;
		changeHealthHeal(amount);
		changeEnergyDown(amount);
		changeHasHealed(true);
	}

	void charge() {
		//Increase energy points by the amount in the recharge
		changeEnergyUp(getRechargeRate());
	}
};

bool checkGameOver(Monster playerMonster, Monster computerMonster) {
	//Check to see if either monster's health is 0
	bool gameOver = false;
	if (playerMonster.getHealth() <= 0 || computerMonster.getHealth() <= 0) gameOver = true;

	if (gameOver == true) {
		std::cout << "Game Over!\n";
		if (computerMonster.getHealth() == 0) std::cout << "Well done!\n";
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
	case 6:
		exit();
		break;
	default:
		std::cout << "Error - choice not covered in switch statement made it through vaidation.\n";
	}
	return true;
}

//AI makes a random action choice
int computerTurn(Monster &monster, Monster &otherMonster) {
	//Ensures that AI monster doesn't heal twice in a row
	bool gotRandom = false;
	int random = 0;

	while (!gotRandom) {
		random = rand() % (numberOfOptions - 1) + 1;

		//AI's turn processing (checks to see if monster heals as this is a special case where two moves may be made in one turn)
		if (random == 5 && !monster.getHasHealed()) {
			gotRandom = true;
			monster.heal();
		}
		else if (random != 5) {
			gotRandom = true;
			bool canAfford = processChoice(monster, otherMonster, random);
			if (canAfford = false) std::cout << "You didn't have enough energy points to make that move! You miss your turn!";
		}
	}

	return random;
}

int playerTurn(Monster playerMonster) {
	//Offer options to player
	std::cout << "Enter the number that matches your action:\n";
	std::cout << "1 - Normal Attack (Costs 20 energy points)\n";
	std::cout << "2 - Special Attack (Costs 50 energy points)\n";
	std::cout << "3 - Re-Charge\n";
	std::cout << "4 - Dodge\n";
	std::cout << "5 - Heal\n";
	std::cout << "6 - Exit\n\n";

	//Get choice from player with validation
	bool makingChoice = true;
	int choice;
	while (makingChoice) {
		clearBuffer();
		cin >> choice;

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

	//Create monsters
	Monster playerMonster;
	playerMonster.initialise("Your Monster ", 100, 50, 10, 0);
	Monster computerMonster;
	computerMonster.initialise("The AI ", 100, 50, 10, 0);

	//Loop for while the player has not exited the game
	bool playing = true;
	while (playing) {
		int choice = playerTurn(playerMonster);

		//Player's turn processing (checks to see if monster heals as this is a special case where two moves may be made in one turn)
		//Ensures that player can afford the action they take
		bool canAfford = true;
		if(choice == 5) playerMonster.heal();
		else canAfford = processChoice(playerMonster, computerMonster, choice);
		if (canAfford == false) {
			std::cout << "You didn't have enough energy points to make that move! You miss your turn!\n";
		}

		//Check to see if last turn resulted in a win then process computer's choice and check again
		//If either player heals they are given a second move
		playing = !checkGameOver(playerMonster, computerMonster);

		//Give the player feedback on the state of both monsters
		giveFeedback(playerMonster, computerMonster);
		waitForUser();
		
		if (choice == 5) {
			std::cout << "You get another move since you healed for your first one this turn!\n";
			choice = playerTurn(playerMonster);
			canAfford = processChoice(playerMonster, computerMonster, choice);
			if (canAfford = false) std::cout << "You didn't have enough energy points to make that move! You miss your turn!";
			playerMonster.changeHasHealed(false);
			
			playing = !checkGameOver(playerMonster, computerMonster);

			giveFeedback(playerMonster, computerMonster);
			waitForUser();
		}
		
		std::cout << computerMonster.getName() << "is taking their turn!\n";
		wait(2);
		int computerChoice = computerTurn(computerMonster, playerMonster);

		playing = !checkGameOver(playerMonster, computerMonster);

		giveFeedback(playerMonster, computerMonster);
		waitForUser();

		if (computerChoice == 5) {
			computerTurn(computerMonster, playerMonster);
			computerMonster.changeHasHealed(false);

			playing = !checkGameOver(playerMonster, computerMonster);

			giveFeedback(playerMonster, computerMonster);
			waitForUser();
		}
	}
	exit();
}
