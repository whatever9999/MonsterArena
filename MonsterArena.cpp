#include "pch.h"
#include <iostream>
#include <string>

using namespace std;

//The number of options available to the player
int numberOfOptions = 6;

int main() {
	//Introduction of game to user
    std::cout << "Welcome to Monster Arena! The turn based strategy game where you get the chance to pit one of your monsters against an AI controlled monster! Enjoy, and good luck...\n"; 

	//Loop for while the player has not exited the game
	bool playing = true;
	while (playing) {
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

		switch (choice) {
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			break;
		default:
			std::cout << "Error - choice not covered in switch statement made it through vaidation.";
		}
		playing = false;
	}

	return 0;
}

//The Monster object (used to create any and all monsters as a base)
class Monster {
	public: 
		int healthPoints; //Monster health
		int energyPoints; //Monster energy
};
