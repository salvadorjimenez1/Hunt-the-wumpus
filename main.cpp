/* prog5Wumpus.cpp
 
 Description: Navigate a maze to kill the Wumpus.
 Implementation of a 1972 game by Gregory Yob.
 
 Results of a program run are shown below:
 
 
 */
#include <iostream>
#include <iomanip>   // for setw()
#include <cctype>    // for toupper()
#include <cstdlib>   // for rand() and srand()
#include <ctime>     // to seed random number generator with time(0)
#include <vector>
using namespace std;

// global constants
#define MAX_LINE_LENGTH 81
#define NUMBER_OF_ROOMS 20

//--------------------------------------------------------------------------------
void displayCave()
{
    cout<< "       ______18______             \n"
    << "      /      |       \\           \n"
    << "     /      _9__      \\          \n"
    << "    /      /    \\      \\        \n"
    << "   /      /      \\      \\       \n"
    << "  17     8        10     19       \n"
    << "  | \\   / \\      /  \\   / |    \n"
    << "  |  \\ /   \\    /    \\ /  |    \n"
    << "  |   7     1---2     11  |       \n"
    << "  |   |    /     \\    |   |      \n"
    << "  |   6----5     3---12   |       \n"
    << "  |   |     \\   /     |   |      \n"
    << "  |   \\       4      /    |      \n"
    << "  |    \\      |     /     |      \n"
    << "  \\     15---14---13     /       \n"
    << "   \\   /            \\   /       \n"
    << "    \\ /              \\ /        \n"
    << "    16---------------20           \n"
    << endl;
}//end displayCave()


//--------------------------------------------------------------------------------
void displayInstructions()
{
    cout<< "Hunt the Wumpus:                                             \n"
    << "The Wumpus lives in a completely dark cave of 20 rooms. Each \n"
    << "room has 3 tunnels leading to other rooms.                   \n"
    << "                                                             \n"
    << "Hazards:                                                     \n"
    << "1. Two rooms have bottomless pits in them.  If you go there you fall and die.  \n"
    << "2. Two other rooms have super-bats.  If you go there, the bats grab you and    \n"
    << "   fly you to some random room, which could be troublesome.  Then those bats go\n"
    << "   to a new room different from where they came from and from the other bats.  \n"
    << "3. The Wumpus is not bothered by the pits or bats, as he has sucker feet and   \n"
    << "   is too heavy for bats to lift.  Usually he is asleep.  Two things wake      \n"
    << "   him up: Anytime you shoot an arrow, or you entering his room.  When he      \n"
    << "   wakes he moves if he is in an odd-numbered room, but stays still otherwise. \n"
    << "   After that, if he is in your room, he eats you and you die!                 \n"
    << "                                                                               \n"
    << "Moves:                                                                         \n"
    << "On each move you can do the following, where input can be upper or lower-case: \n"
    << "1. Move into an adjacent room.  To move enter 'M' followed by a space and      \n"
    << "   then a room number.                                                         \n"
    << "2. Shoot your guided arrow through a list of up to three adjacent rooms, which \n"
    << "   you specify.  Your arrow ends up in the final room.                         \n"
    << "   To shoot enter 'S' followed by the number of rooms (1..3), and then the     \n"
    << "   list of the desired number (up to 3) of adjacent room numbers, separated    \n"
    << "   by spaces. If an arrow can't go a direction because there is no connecting  \n"
    << "   tunnel, it ricochets and moves to the lowest-numbered adjacent room and     \n"
    << "   continues doing this until it has traveled the designated number of rooms.  \n"
    << "   If the arrow hits the Wumpus, you win! If the arrow hits you, you lose. You \n"
    << "   automatically pick up the arrow if you go through a room with the arrow in  \n"
    << "   it.                                                                         \n"
    << "3. Enter 'R' to reset the person and hazard locations, useful for testing.     \n"
    << "4. Enter 'C' to cheat and display current board positions.                     \n"
    << "5. Enter 'D' to display this set of instructions.                              \n"
    << "6. Enter 'P' to print the maze room layout.                                    \n"
    << "7. Enter 'X' to exit the game.                                                 \n"
    << "                                                                               \n"
    << "Good luck!                                                                     \n"
    << " \n"
    << endl;
}//end displayInstructions()


//--------------------------------------------------------------------------------
int roomNumbersArray[21][4] = { {0,0,0,0},
                                {1,2,5,8},
                                {2,1,3,10},
                                {3,2,4,12},
                                {4,3,5,14},
                                {5,1,4,6},
                                {6,5,7,15},
                                {7,6,8,17},
                                {8,1,7,9},
                                {9,8,10,18},
                                {10,2,9,11},
                                {11,10,12,19},
                                {12,3,11,13},
                                {13,12,14,20},
                                {14,4,13,15},
                                {15,6,14,16},
                                {16,15,17,20},
                                {17,7,16,18},
                                {18,9,17,19},
                                {19,11,18,20},
                                {20,13,16,19},
};

struct theGame {
    int roomNumber;
    bool isTrue;
};

//****************************************************************
//* This function playerMove is called when the player wants     *
//* to make a move into an adjacent room. Adjacency is also      *
//* checked in this function to make sure that the move the      *
//* player wants to make is valid.                               *
//****************************************************************
void playerMove(int userRoomChoice, int &playerRoomNum, int &theArrowRoomNum, int &moveCounter){
    int moveValidation = 1;
    for(int i = 1; i < 4; i++){
        if(userRoomChoice == roomNumbersArray[playerRoomNum][i]){
            moveValidation = i;
        }
    }
    if(userRoomChoice != roomNumbersArray[playerRoomNum][moveValidation]){
        cout << "Invalid move. Please retry." << endl;
        moveCounter--;
    }
    if(userRoomChoice == roomNumbersArray[playerRoomNum][moveValidation]){
        playerRoomNum = userRoomChoice;
    }
    if(playerRoomNum == theArrowRoomNum){
        cout << "Congratulations, you found the arrow and can once again shoot." << endl;
        theArrowRoomNum = -1;
    }
    
    
} // end playerMove


int main()
{
    // Seed the random number generator.  Change seed to time(0) to change output each time.
    // srand(time(0));
    srand(1);   // Use this version in what you turn in
    
    char userMove; // this character is the input to determine what the user wants to do.
    
    int userRoomChoice; // this variable represents what room the user wants to go into
    int moveCounter = 1; // counter for each turn the player takes
    int column = 4;
    
    //*********************************************************************
    //* I declared all of the game pieces as objects because they all     *
    //* share rooms. Most of the objects are initialized to rand()%20 + 1 *
    //* to give us a room number between 1 through 20. The arrow is       *
    //* initialize to -1 because in this game to signify that it is       *
    //* with the player.                                                  *
    //*********************************************************************
    
    theGame theBat1;
    theBat1.roomNumber = (rand()%20) + 1;
    
    theGame theBat2;
    theBat2.roomNumber = (rand()%20) + 1;
    
    theGame bottomlessPit1;
    bottomlessPit1.roomNumber = (rand()%20) + 1;
    
    theGame bottomlessPit2;
    bottomlessPit2.roomNumber = (rand()%20) + 1;
    
    theGame theWumpus;
    theWumpus.roomNumber = (rand()%20) + 1;
    
    theGame player;
    player.roomNumber = 13;
    
    theGame theArrow;
    theArrow.roomNumber = -1;
    
    player.isTrue = true;
    
    //******************************************************************
    //* The whole game is in a do while loop. This allows the player   *
    //* to make the amount of moves he wants to make before dying or   *
    //* quitting the game. This is a menu-driven game. Using the char  *
    //* userMove, the player is allowed to select what they choose to  *
    //* do. They can make a move, shoot the arrow, cheat, reset the    *
    //* game values or quit.                                           *
    //******************************************************************
    do{
        
        //**************************************************
        //* We check if the hazards are in the rooms first.*
        //* We check the pits first, then the wumpus and   *
        //* then we check for the bats.                    *
        //**************************************************
        
        if (player.roomNumber == bottomlessPit1.roomNumber ||
            player.roomNumber == bottomlessPit2.roomNumber) {
            cout << " Aaaaaaaaahhhhhh...." << endl;
            cout << "You fall into a pit and die." << endl;
            break;
        }
        
        //*********************************************
        //* If the wumpus is in the room, this checks *
        //* if the wumpus is in an even numbered room.*
        //* If it is then the player dies. If it's in *
        //* an odd numbered room then the wumpus moves*
        //* to lowest numbered adjacent room.         *
        //*********************************************
        
        if (player.roomNumber == theWumpus.roomNumber) {
            if((theWumpus.roomNumber % 2) == 0){
                cout << "You briefly feel a slimy tentacled arm as your neck is snapped." << endl;
                cout << "It is over." << endl;
                break;
            }
            else{
                theWumpus.roomNumber = roomNumbersArray[theWumpus.roomNumber][1];
                cout << "You hear a slithering sound, as the Wumpus slips away. "
                << "Whew, that was close!" << endl;
            }
        }
        
        //**********************************************************
        //* If the player walks into the room                      *
        //* with bats in it, then he will be transported           *
        //* into a different room and the bats will also           *
        //* move into a different room from their original room.   *
        //**********************************************************
        
        if (player.roomNumber == theBat1.roomNumber ||
            player.roomNumber == theBat2.roomNumber) {
            
            if(player.roomNumber == theBat1.roomNumber){
                theBat1.roomNumber = (rand()%20) - 6;
            }
            if(player.roomNumber == theBat2.roomNumber){
                theBat2.roomNumber = (rand()%20) + 1;
            }
            cout << "Woah... you're flying!" << endl;
            player.roomNumber = (rand() % 20) + 4;
            
            cout << "You've just been transported by bats to room "
                << player.roomNumber << endl;
        }
        
        //***********************************************
        //* If the player is alive, then                *
        //* it will display what room the player is in. *
        //***********************************************
        if(player.isTrue == true){
            cout << "\nYou are in room " << player.roomNumber << ". ";
        }
        
        //*******************************************
        //* This section of the code will sense if  *
        //* the hazards of the game are nearby.     *
        //*******************************************
        for (int i = 1; i < column; i++ ) {
            
            if (bottomlessPit1.roomNumber == roomNumbersArray[player.roomNumber][i] ||
                bottomlessPit2.roomNumber == roomNumbersArray[player.roomNumber][i]) {
                player.isTrue = true;
                cout << "You feel a draft." << endl;
            }
        }
        
        for(int i = 1; i< column; i++){
            
            if(theWumpus.roomNumber == roomNumbersArray[player.roomNumber][i]){
                player.isTrue = true;
                cout << "You smell a stench." << endl;
            }
        }
        for(int i= 1; i < column; i++){
            
            if (theBat1.roomNumber == roomNumbersArray[player.roomNumber][i] ||
                theBat2.roomNumber == roomNumbersArray[player.roomNumber][i]) {
                player.isTrue = true;
                cout << "You hear rustling of batwings." << endl;
            }
        }
        
        
        //************************************
        //* Prompt player to make their move *
        //************************************
        cout << '\n' << moveCounter << ". ";
        cout << "Enter your move (or 'D' for directions): ";
        cin >> userMove;
        
        //******************************************
        //* Calls playerMove function to allow the *
        //* player to make a valid move.           *
        //******************************************
        if(userMove == 'm' || userMove == 'M'){
            cin >> userRoomChoice;

            playerMove(userRoomChoice, player.roomNumber, theArrow.roomNumber, moveCounter);
        }
        
        //***************************************************
        //* This part of the code will allow the            *
        //* user to shoot the arrow if it is with them      *
        //* and be able to kill the wumpus if they guess    *
        //* the right room. If the player puts an invalid   *
        //* room number, then the arrow will ricochet into  *
        //* the lowest numbered adjacent room.              *
        //***************************************************
        if((userMove == 'S' || userMove == 's') && theArrow.roomNumber == -1){
            
            bool ricochet = false;
            int shotLength = 0;
            int temp = 0; // temporary variable only used to keep track of the counters
            int currentArrowRoom; // this will store the arrows current room when it shoots more than one room
            int room1, room2, room3 = 0; // the rooms the player wants to shoot in

            //***************************
            //* Prompt player to shoot  *
            //***************************
            cout << "Enter the number of rooms (1..3) into which "
            << "you want to shoot, followed by the rooms themselves: ";
            cin >> shotLength;
            
            currentArrowRoom = player.roomNumber;
            
            //***********************************************************************
            //* These next few blocks of code check if the rooms                    *
            //* the player wants to shoot in. If they are valid,                    *
            //* then the arrow will be shot into that room and stay there until     *
            //* the player picks it up again. If the room is invalid, then the      *
            //* arrow will ricochet into the lowest adjacent room.                  *
            //***********************************************************************
            if(shotLength == 1){
                cin >> room1;
                for(int i = 1; i < column; i++){
                    if(room1 == roomNumbersArray[player.roomNumber][i]){
                        temp = i;
                        theArrow.roomNumber = room1;
                        currentArrowRoom = theArrow.roomNumber;
                        break;
                    }
                }
                if(theArrow.roomNumber != roomNumbersArray[player.roomNumber][temp]){
                    cout << "Room " << room1 << " is not adjacent. "
                    << "Arrow ricochets..." << endl;
                    theArrow.roomNumber = currentArrowRoom;
                    theArrow.roomNumber = roomNumbersArray[currentArrowRoom][1];
                    ricochet = true;
                }
            }
            //***********************************************
            //* When the player wants to shoot into 2 rooms *
            //***********************************************
            if(shotLength == 2){
                cin >> room1 >> room2;
                for(int i =1; i < column; i++){
                    if(room1 == roomNumbersArray[player.roomNumber][i]){
                        theArrow.roomNumber = room1;
                        currentArrowRoom = room1;
                    }
                }
                for(int i = 1; i < column; i++){
                    if(room2 == roomNumbersArray[currentArrowRoom][i]){
                        temp = i;
                        theArrow.roomNumber = room2;
                        currentArrowRoom = room2;
                    }
                }
                
                if(room2 != roomNumbersArray[room1][temp]){
                    cout << "Room " << room2 << " is not adjacent. "
                         << " Arrow ricochets..." << endl;
                    theArrow.roomNumber = currentArrowRoom;
                    theArrow.roomNumber = roomNumbersArray[currentArrowRoom][1];
                    ricochet = true;
                }
                if(room2 == player.roomNumber){
                    cout << "You just shot yourself. "
                    << "Maybe Darwin was right.  You're dead." << endl;
                    break;
                
                }
            }
            //***********************************************
            //* When the player wants to shoot into 3 rooms *
            //***********************************************
            if(shotLength == 3){
                cin >> room1 >> room2 >> room3;
                for(int i =1; i < column; i++){
                    if(room1 == roomNumbersArray[player.roomNumber][i]){
                        theArrow.roomNumber = room1;
                        currentArrowRoom = room1;
                    }
                }
                for(int i = 1; i < column; i++){
                    if(room2 == roomNumbersArray[currentArrowRoom][i]){
                        theArrow.roomNumber = room2;
                        currentArrowRoom = room2;
                    }
                }
                
                for(int i = 1; i < column; i++){
                    if(room3 == roomNumbersArray[currentArrowRoom][i]){
                        temp = i;
                        theArrow.roomNumber = room3;
                        currentArrowRoom = room3;
                    }
                }
                
                if(room3 != roomNumbersArray[room2][temp]){
                    cout << "Room " << room3 << " is not adjacent.";
                    cout << " Arrow ricochets..." << endl;
                    theArrow.roomNumber = currentArrowRoom;
                    theArrow.roomNumber = roomNumbersArray[currentArrowRoom][1];
                    ricochet = true;
                }
            }
            
            //***************************************************************
            //* If the arrow ricochets and accidentally kills the wumpus,   *
            //* then this code will execute telling the user they won       *
            //***************************************************************
            if(ricochet == true){
                if(theArrow.roomNumber == theWumpus.roomNumber){
                    cout << "Your arrow ricochet killed the Wumpus, you lucky dog! "
                    << "Accidental victory, but still you win!" << endl;
                    break;
                }
            }
            
            //****************************************************
            //* If the player shoots themselves after a ricochet *
            //* then they will die and it will be over for them  *
            //****************************************************
            if(theArrow.roomNumber == player.roomNumber){
                cout << "You just shot yourself, and are dying. "
                << "It didn't take long, you're dead." << endl;
                break;
            }

            //************************************
            //* When the arrow shoots the wumpus *
            //* and kills it.                    *
            //************************************
            if(theArrow.roomNumber == theWumpus.roomNumber){
                cout << "Wumpus has just been pierced by your deadly arrow!" << endl;
                cout << "Congratulations on your victory, you awesome hunter you." << endl;
                break;
            }
            
            //************************************************************
            //* If the arrow misses the wumpus, then it will             *
            //* wake up and move into the lowest numbered adjacent room. *
            //************************************************************
            if(theArrow.roomNumber != theWumpus.roomNumber) {
                theWumpus.roomNumber = roomNumbersArray[theWumpus.roomNumber][1];
            }
        }
        //**********************************************************
        //* If the user attempts to shoot without having an arrow, *
        //* then he will get an error telling them to go find it.  *
        //**********************************************************
        else if((userMove == 'S' || userMove == 's') && theArrow.roomNumber != -1) {
            cout << "Sorry, you can't shoot an arrow you don't have.  Go find it." << endl;
        
        } // end shooting
        
        //********************************************
        //* This will allow the player to see the    *
        //* location of all the hazards.             *
        //********************************************
        if(userMove == 'C' || userMove == 'c'){
            cout << "Cheating! Game elements are in the following rooms: " << endl;
            cout << "Player Wumpus Bats1 Bats2 Pit1 Pit2 Arrow" << endl;
            cout << setw(4) << player.roomNumber << setw(7)
            << theWumpus.roomNumber << setw(6)
            << theBat1.roomNumber << setw(6)
            << theBat2.roomNumber << setw(5)
            << bottomlessPit1.roomNumber << setw(5)
            << bottomlessPit2.roomNumber << setw(5)
            << theArrow.roomNumber << endl;
            moveCounter--;
        }
        
        //******************************
        //* Display instructions again *
        //******************************
        if (userMove == 'D' || userMove == 'd') {
            displayInstructions();
            moveCounter--;
        }
        
        //*********************
        //* Display map again *
        //*********************
        if(userMove == 'P' || userMove == 'p'){
            displayCave();
            moveCounter--;
        }
        
        //************************************************
        //* This will allow the player to reset          *
        //* the values of all the hazards and the        *
        //* initial room number of the player and arrow  *
        //************************************************
        if(userMove == 'R' || userMove == 'r'){
            cout << "\n(Remember arrow value of -1 means it is with the person.)" << endl;
            cout << "Enter the 7 room locations (1..20) for player,"
            << "wumpus, bats1, bats2, pit1, pit2, and arrow: " << endl;
            cin >> player.roomNumber
            >> theWumpus.roomNumber
            >> theBat1.roomNumber
            >> theBat2.roomNumber
            >> bottomlessPit1.roomNumber
            >> bottomlessPit2.roomNumber
            >> theArrow.roomNumber;
            moveCounter--;
        }
        
        //*****************
        //* Exit the game *
        //*****************
        if(userMove == 'X' || userMove == 'x'){
            cout << "\nExiting Program ... " << endl;
            exit(0);
        }
        
        moveCounter++; // Increments after each move
        
    } while (userMove != 'x' || userMove != 'X');
    
    cout << "\nExiting Program ..." << endl;
}//end main()
