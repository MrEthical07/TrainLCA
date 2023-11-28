// write a C program to reserve train tickets with following functionality:
// 1)User System: Login and signup       DONE
// 2)Tickets reservation:	
// 	i)With option to select preferred class, compartment,seat type (seater or sleeper) and number of seats        DONE
// 	ii)With payment system.(with payment failure handling case).       DONE
// 	iii)Once reserved, get PNR.       DONE
// 3)Get Information of reservations (By entering PNR):
// 	i)To know in which compartment, class seat is reserved.          DONE
// 	ii)To know if ticket is confirmed or in waiting list.       DONE
// 	iii)Get reservation id or referrence number of the reservation.        DONE
// 	iv)To know all about train (max speed, total dist to be travelled )     EHHHHHHHHHHHHHH WE GOTTA DO THAT??????
// 4)Checking Train Status:
// 	i)To know if a train is running late.           DONE
// 	ii)To know time till arrival of a train.           EHHHHHHHHHHHHHH WE GOTTA DO THAT??????
// 5)Alert System:
// 	i)get different alerts in the case of train delays or change in schedules. EHHHHHHHHHHHHHH WE GOTTA DO THAT??????
// 6)Option to cancel tickets:
// 	i)option to cancel ticket.                          DONE
// 	ii)Proper refund to the original payment method.                DONE


#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include "qrcodegen.h"
#include <ctype.h>
// #include <unistd.h>

#define MAX_PASSENGERS 100


typedef struct {
    char userId[10];
    char email[100];
    char password[100];
    char name[100];
} User;

typedef struct {
    char trainId[10];
    char startingPoint[100];
    char destination[100];
    char departureTime[100];
    int cost;
    int compartments;
    char seatType[100];
} Train;

typedef struct {
    long long pnr;
    char userId[10];
    char name[10];
    char trainId[10];
    int compartment;
    char seatType[10];
    char status[10];
    float cost;
    int seats;
    int noOfSeats;
} Reservation;

typedef struct {
    char trainId[20];
    char startingPoint[20];
    char destination[20];
    char departureTime[15];
    int cost;
    int compartments;
    char seatType[20];
    int isDelayed;  
    int delayTime;  
} DelayedTrain;

int delayMilliseconds = 5000;
int logged = 0;
int *loggedPtr = &logged;
char userId[10];
char *userIdPtr = userId;
 
void signup();
void login();
void resetPass();
int userExists(const char* email);
void sortByCost(int sortOrder);
void displayAllTrains();
void findTrainsByStartingPoint(const char* startingPoint);
void findTrainsByDestination(const char* destination);
void findTrain(const char* trainId);
void mainMenu();
void trainListandBook();
void showCompartment( int compartment,  const char* trainId);
void showTickets( int choosenCompartment,  const char* trainId);
void sleepProgram( int seconds);
void payNow( const char* trainId,  int ticketsToBuy,  int choosenCompartment,  int* ticketsNums);
void paymentGateway(const char* trainId, int ticketsToBuy, int choosenCompartment, float totalPrice, int ticketsNums[MAX_PASSENGERS]);
void confirmTickets( const char* trainId,  int ticketsToBuy,  int choosenCompartment,  int* ticketsNums,  float totalPrice);
void cancelTickets( const char* trainId,  int ticketsToBuy,  int choosenCompartment,  int* ticketsNums);
void addDelayedTrain();
void getTrainInformation( const char* trainId,  int ticketsToBuy,  int choosenCompartment,  int* ticketsNums);
void checkAllReservations();
void deleteReservationByPnr(long long pnr);
void showUserInfo();

void sleepProgram(int seconds) 
{
  Sleep(seconds * 1000);
}

void clearTerminal(){
    system("cls");
}

void greenColor() 
{
  printf("\033[1;32m");
}

void redColor() 
{
  printf("\033[1;31m");
}

void resetColor() 
{
  printf("\033[0m");
}

void yellowColor() {
  printf("\033[1;33m");
}

void signup() {
    clearTerminal();
    int SignedUp, AlrExists;

    FILE *file = fopen("users.txt", "a");
    if (file == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }
    
    User newUser;
    printf("\nPlease enter your name: ");
    scanf("%s", newUser.name);

    printf("Please enter your email: ");
    scanf("%s", newUser.email);

    // Check if the user with the same email already exists
    if (userExists(newUser.email)) {
        printf("\nUser with the same email already exists. Press 1 to log in. Press 2 to try again\n");
          scanf("%d", &AlrExists);
          if (AlrExists == 1) {
            login();
          } else if (AlrExists == 2) {
            signup();
          }
        return;
    }

    printf("Please enter your password: ");
    scanf("%s", newUser.password);

    // Auto-generate userId (you can implement your logic for generating unique userId)
    sprintf(newUser.userId, "%08d", rand() % 100000000);

    // Write user information to file
    fprintf(file, "%s %s %s %s\n", newUser.userId, newUser.email, newUser.password, newUser.name);
    printf("\nNew user created successfully!\n");
    printf("\nHere are user Details:\n");
    printf("User ID: %s\n", newUser.userId);
    printf("Email: %s\n", newUser.email);
    printf("Name: %s\n", newUser.name);
    fclose(file);

    printf("\nAccount created successfully! Please log in.\n");
    printf("Press 1 to login or press 2 to create another account:  ");
    scanf("%d", &SignedUp);
    if (SignedUp == 1) {
      login();
    } else if (SignedUp == 2) {
      signup();
    }
}

int userExists(const char* email) {
    FILE *file = fopen("users.txt", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

    char line[500];
    while (fgets(line, sizeof(line), file) != NULL) {
        User user;
        sscanf(line, "%s %s %s %s", user.userId, user.email, user.password, user.name);

        if (strcmp(user.email, email) == 0) {
            fclose(file);
            return 1; 
        }
    }

    fclose(file);
    return 0;  
}

void login() {
    int wrongPass, wrongUser;
    char email[100];
    char password[100];
    char line[500];

    clearTerminal();
    printf("\n\n\n\n\n==============================================================================================\n\n");
    printf("\t\t\t\033[1;31mPlease Login:\033[0m\t\t");
    printf("\n\n==============================================================================================\n");
    printf("\nPlease enter your email:");
    scanf("%s", email);
    getchar();

    FILE *file = fopen("users.txt", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

    int userFound = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        User user;
        sscanf(line, "%s %s %s %s", user.userId, user.email, user.password, user.name);

        if (strcmp(user.email, email) == 0) {
            userFound = 1;
            printf("\nUser Found\n");
            printf("Please enter your password: ");
            scanf("%s", password);
            getchar();
            if (strcmp(user.password, password) == 0) {
                printf("\nLogin successful!\n");
                *loggedPtr = 1;
                strcpy(userIdPtr, user.userId);
                yellowColor();
                printf("\n\t\t\tYou are logged in and can proceed to book tickets!!");
                resetColor();
                mainMenu();
                break;
            } else {
                printf("\nIncorrect password. Please try again.\n");
                printf("\nPress 1 to try again or press 2 to reset password!\n");
                scanf("%d", &wrongPass);
                if (wrongPass == 1) {
                    login();
                } else if (wrongPass == 2) {
                    resetPass();
                }
            }
            break;
        }
    }
    fclose(file);
    if (!userFound) {
        printf("\nUser not found. Please create an account.\n");
        printf("\nPress 1 to create an account or press 2 to try again!\n");
        scanf("%d", &wrongUser);
        if (wrongUser == 1) {
            signup();
        } else if (wrongUser == 2) {
            login();
        }
    }
}

void copyFile(const char *source, const char *destination) {
    FILE *src = fopen(source, "rb");
    FILE *dest = fopen(destination, "wb");

    if (src == NULL || dest == NULL) {
        printf("Error opening files for copying.\n");
        return;
    }

    char ch;
    while ((ch = fgetc(src)) != EOF) {
        fputc(ch, dest);
    }

    fclose(src);
    fclose(dest);
}

void resetPass() {
    int resetNoUser, resetDoneUser;
    char email[100];
    char newPassword[100];
    char line[500];
    clearTerminal();
    printf("\nPlease enter your email: ");
    scanf("%s", email);
    getchar();
    FILE *file = fopen("users.txt", "r");
    
    if (file == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

    // Create a temporary file
    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        printf("Error creating temporary file.\n");
        fclose(file);
        exit(1);
    }

    int userFound = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        User user;
        sscanf(line, "%s %s %s %s", user.userId, user.email, user.password, user.name);
        
        if (strcmp(user.email, email) == 0) {
            userFound = 1;
            printf("\nUser Found\n");
            printf("Please enter your new password: ");
            scanf("%s", newPassword);
            getchar();

            // Modify the password in memory
            strcpy(user.password, newPassword);
        }

        // Write the line to the temporary file
        fprintf(tempFile, "%s %s %s %s\n", user.userId, user.email, user.password, user.name);
    }

    fclose(file);
    fclose(tempFile);
    remove("users.txt");
    fclose(fopen("users.txt", "w"));  
    copyFile("temp.txt", "users.txt");
    remove("temp.txt");

    if (!userFound) {
        printf("\nUser not found. Please create an account.\n");
        printf("\nPress 1 to create an account or press 2 to try again!\n");
        scanf("%d", &resetNoUser);

        if (resetNoUser == 1) {
            signup();
        } else if (resetNoUser == 2) {
            resetPass();
        }
    } else {
        printf("\nPassword Reset Successful!!");
        printf("\n\n==============================================================================================\n");
        printf("\n Do you want to continue to login? or signup? or exit?");
        printf("\nPress 1 to login, Press 2 to signup, Press any other key to exit!\n");
        scanf("%d", &resetDoneUser);

        if (resetDoneUser == 1) {
            login();
        } else if (resetDoneUser == 2) {
            signup();
        } else {
            exit(0);
        }
    }
}

void LogOrSign(){
    int LogOrSign;
    clearTerminal();
    printf("\n\n\n\n\n==============================================================================================\n\n");
    printf("Please Login or Signup to continue booking tickets");
    printf("\n\n==============================================================================================\n\n");
    printf("  Select from the following options:\n\n");
    printf("   \033[1;31m[1]\033[0m Login \n\n");
    printf("   \033[1;31m[2]\033[0m Signup\n\n");
    printf("   \033[1;31m[3]\033[0m Reset Password\n\n");
    scanf("%d", &LogOrSign);
    do
    {
        switch (LogOrSign)
    {
    case 1:
        login();
        break;
    
    case 2:
        signup();
        break;
    case 3:
        resetPass();
        break;
    default:
        printf("Please Choose Correct Option");
        break;
    }
    } while (LogOrSign != 1 && LogOrSign != 2 && LogOrSign != 3);
}

void addTrain() {
    FILE *file = fopen("train.txt", "r+");
    if (file == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

    // Read the current number of trains
    int numTrains;
    fscanf(file, "%d", &numTrains);

    // Increment the number of trains
    numTrains++;

    // Move the file pointer to the beginning
    fseek(file, 0, SEEK_SET);

    // Write the updated number of trains back to the file
    fprintf(file, "%d\n", numTrains);

    // Move the file pointer to the end for appending the new train
    fseek(file, 0, SEEK_END);

    Train newTrain;
    printf("\nPlease enter the Train ID: ");
    scanf("%s", newTrain.trainId);

    printf("Please enter the Starting Point: ");
    scanf("%s", newTrain.startingPoint);

    printf("Please enter the Destination: ");
    scanf("%s", newTrain.destination);

    printf("Please enter the Departure Time: ");
    scanf("%s", newTrain.departureTime);

    printf("Please enter the Cost: ");
    scanf("%d", &newTrain.cost);

    printf("Please enter number of Compartments: ");
    scanf("%d", &newTrain.compartments);

    printf("Please enter Seat type:- sleeper or seater: ");
    scanf("%s", newTrain.seatType);

    fprintf(file, "%s %s %s %s %d %d %s\n", newTrain.trainId, newTrain.startingPoint, newTrain.destination, newTrain.departureTime, newTrain.cost, newTrain.compartments, newTrain.seatType);

    fclose(file);

    printf("\nTrain added successfully!\n");
}

void displayAllTrains() {
    FILE *file = fopen("train.txt", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

    int numTrains;
    fscanf(file, "%d", &numTrains);

    printf("\n====================================================================================================================================\n");
    printf("| %-10s | %-20s | %-20s | %-15s | %-5s | %-20s | %-20s |\n", "Train ID", "Starting Point", "Destination", "Departure Time", "Cost", "Compartments", "Seat Type");
    printf("====================================================================================================================================\n");

    for (int i = 0; i < numTrains; i++) {
        Train train;
        fscanf(file, "%s %s %s %s %d %d %s", train.trainId, train.startingPoint, train.destination,  train.departureTime, &train.cost, &train.compartments, train.seatType);

        printf("| %-10s | %-20s | %-20s | %-15s | %-5d | %-20d | %-20s |\n", train.trainId, train.startingPoint, train.destination, train.departureTime, train.cost, train.compartments, train.seatType);
    }

    printf("====================================================================================================================================\n");
    fclose(file);
}

int compareByCostAsc(const void* a, const void* b) {
    return ((Train*)a)->cost - ((Train*)b)->cost;
}

int compareByCostDesc(const void* a, const void* b) {
    return ((Train*)b)->cost - ((Train*)a)->cost;
}

void sortByCost(int sortOrder) {
    FILE* file = fopen("train.txt", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

    int numTrains;
    fscanf(file, "%d", &numTrains);

    Train* trains = (Train*)malloc(numTrains * sizeof(Train));
    if (trains == NULL) {
        printf("Memory allocation failed.\n");
        fclose(file);
        exit(1);
    }

    for (int i = 0; i < numTrains; i++) {
        fscanf(file, "%s %s %s %s %d %d %s", trains[i].trainId, trains[i].startingPoint, trains[i].destination, trains[i].departureTime, &trains[i].cost, &trains[i].compartments, trains[i].seatType);
    }

    fclose(file);

    if (sortOrder == 2) {
        qsort(trains, numTrains, sizeof(Train), compareByCostDesc);
    } else {
        qsort(trains, numTrains, sizeof(Train), compareByCostAsc);
    }

    // Display sorted trains
    printf("\n====================================================================================================================================\n");
    printf("| %-10s | %-20s | %-20s | %-15s | %-5s | %-20s | %-20s |\n", "Train ID", "Starting Point", "Destination", "Departure Time", "Cost", "Compartments", "Seat Type");
    printf("====================================================================================================================================\n");

    for (int i = 0; i < numTrains; i++) {
        printf("| %-10s | %-20s | %-20s | %-15s | %-5d | %-20d | %-20s |\n", trains[i].trainId, trains[i].startingPoint, trains[i].destination, trains[i].departureTime, trains[i].cost, trains[i].compartments, trains[i].seatType);
    }
    printf("====================================================================================================================================\n");

    free(trains);
}

void findTrainsByDestination(const char* destination) {
    FILE* file = fopen("train.txt", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

    // Skip the first line
    int numTrains;
    fscanf(file, "%d", &numTrains);

    printf("\n====================================================================================================================================\n");
    printf("| %-10s | %-20s | %-20s | %-15s | %-5s | %-20s | %-20s |\n", "Train ID", "Starting Point", "Destination", "Departure Time", "Cost", "Compartments", "Seat Type");
    printf("====================================================================================================================================\n");

    Train train;
    int found = 0;

    while (fscanf(file, "%s %s %s %s %d %d %s", train.trainId, train.startingPoint, train.destination, train.departureTime, &train.cost, &train.compartments, train.seatType) != EOF) {
        if (strcmp(train.destination, destination) == 0) {
            found = 1;
            printf("| %-10s | %-20s | %-20s | %-15s | %-5d | %-20d | %-20s |\n", train.trainId, train.startingPoint, train.destination, train.departureTime, train.cost, train.compartments, train.seatType);
        }
    }
    printf("====================================================================================================================================\n");

    fclose(file);

    if (!found) {
        printf("No trains found for the destination: %s\n", destination);
    }
}

void findTrainsByStartingPoint(const char* startingPoint) {
    FILE* file = fopen("train.txt", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

    // Skip the first line
    int numTrains;
    fscanf(file, "%d", &numTrains);

    printf("\n====================================================================================================================================\n");
    printf("| %-10s | %-20s | %-20s | %-15s | %-5s | %-20s | %-20s |\n", "Train ID", "Starting Point", "Destination", "Departure Time", "Cost", "Compartments", "Seat Type");
    printf("====================================================================================================================================\n");

    Train train;
    int found = 0;

    while (fscanf(file, "%s %s %s %s %d %d %s", train.trainId, train.startingPoint, train.destination, train.departureTime, &train.cost, &train.compartments, train.seatType) != EOF) {
        if (strcmp(train.startingPoint, startingPoint) == 0) {
            found = 1;
            printf("| %-10s | %-20s | %-20s | %-15s | %-5d | %-20d | %-20s |\n", train.trainId, train.startingPoint, train.destination, train.departureTime, train.cost, train.compartments, train.seatType);
        }
    }
    printf("====================================================================================================================================\n");

    fclose(file);

    if (!found) {
        printf("No trains found for the Starting Point: %s\n", startingPoint);
    }
}

void adminControls() {
    clearTerminal();
    printf("\n====================================================================================================================================\n");
    printf("Welcome To Admin Controls");
    printf("\n====================================================================================================================================\n");
    printf("Please Select Appropriate Option:\n");
    printf("   \033[1;31m[1]\033[0m VIEW TRAIN LIST \n\n");
    printf("   \033[1;31m[2]\033[0m ADD NEW TRAIN\n\n");
    printf("   \033[1;31m[3]\033[0m Add Delayed Trains\n\n");
    printf("   \033[1;31m[4]\033[0m GO BACK TO MAIN MENU\n\n");
    int adminChoice;
    scanf("%d", &adminChoice);
    switch (adminChoice) {
    case 1:
        displayAllTrains();
        break;
    case 2:
        addTrain();
        break;
    case 3:
        addDelayedTrain();
        break;
    case 4:
        mainMenu();
        break;
    default:
        printf("Oops Wrong Choice");
        break;
    }

}

void addDelayedTrain() {
    FILE *file = fopen("delayed.txt", "a");
    if (file == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

    DelayedTrain delayedTrain;
    printf("\nPlease enter the Train ID: ");
    scanf("%s", delayedTrain.trainId);

    printf("Please enter the Starting Point: ");
    scanf("%s", delayedTrain.startingPoint);

    printf("Please enter the Destination: ");
    scanf("%s", delayedTrain.destination);

    printf("Please enter the Departure Time: ");
    scanf("%s", delayedTrain.departureTime);

    printf("Please enter the Cost: ");
    scanf("%d", &delayedTrain.cost);

    printf("Please enter number of Compartments: ");
    scanf("%d", &delayedTrain.compartments);

    printf("Please enter Seat type: sleeper or seater: ");
    scanf("%s", delayedTrain.seatType);

    printf("Is the train delayed? (1 for Yes, 0 for No): ");
    scanf("%d", &delayedTrain.isDelayed);

    if (delayedTrain.isDelayed) {
        printf("Enter delay time (in minutes): ");
        scanf("%d", &delayedTrain.delayTime);
    } else {
        delayedTrain.delayTime = 0;
    }

    fprintf(file, "%s %s %s %s %d %d %s %d %d\n", delayedTrain.trainId, delayedTrain.startingPoint, delayedTrain.destination, delayedTrain.departureTime, delayedTrain.cost, delayedTrain.compartments, delayedTrain.seatType, delayedTrain.isDelayed, delayedTrain.delayTime);

    fclose(file);

    printf("\nDelayed train added successfully!\n");
    sleepProgram(3);
    adminControls();
}

void displayDelayedTrains() {
    FILE *file = fopen("delayed.txt", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }
    DelayedTrain delayedTrain;
    printf("\n===========================================================================================================================================================\n");
    printf("| %-10s | %-20s | %-20s | %-15s | %-5s | %-20s | %-20s | %-10s | %-15s |\n", "Train ID", "Starting Point", "Destination", "Departure Time", "Cost", "Compartments", "Seat Type", "Status", "Delay (min)");
    printf("===========================================================================================================================================================\n");

    while (fscanf(file, "%s %s %s %s %d %d %s %d %d", delayedTrain.trainId, delayedTrain.startingPoint, delayedTrain.destination, delayedTrain.departureTime, &delayedTrain.cost, &delayedTrain.compartments, delayedTrain.seatType, &delayedTrain.isDelayed, &delayedTrain.delayTime) != EOF) {
        printf("| %-10s | %-20s | %-20s | %-15s | %-5d | %-20d | %-20s | %-10s | %-15d |\n", delayedTrain.trainId, delayedTrain.startingPoint, delayedTrain.destination, delayedTrain.departureTime, delayedTrain.cost, delayedTrain.compartments, delayedTrain.seatType, delayedTrain.isDelayed ? "Delayed" : "On Time", delayedTrain.delayTime);
    }

    printf("===========================================================================================================================================================\n");
    fclose(file);
    sleepProgram(3);
    mainMenu();
}

void trainListandBook(){
  displayAllTrains();
        int ifSort;
        printf("Do you want to sort trains? (1 for yes) Or Press 2 for ticket booking Or Press anu other key to go back to main menu: ");
        scanf("%d", &ifSort);
        if (ifSort == 1) {
            printf("Do you want to sort by cost? (1 for ascending, 2 for descinding) Or Do you want to sort based on destination or starting point? (3 for destination, 4 for starting point) Or press any other key to go back to main menu: ");
            int sortChoice;
            scanf("%d", &sortChoice);
            if (sortChoice == 1) {
                sortByCost(1);
            }
            else if (sortChoice == 2)
            {
              sortByCost(2);
            }
            else if(sortChoice == 3)
            {
                char destination[100];
                printf("Enter the destination: ");
                scanf("%s", destination);
                findTrainsByDestination(destination);
            }
            else if(sortChoice == 4)
            {
                char startingPoint[100];
                printf("Enter the starting point: ");
                scanf("%s", startingPoint);
                findTrainsByStartingPoint(startingPoint);
            }
            else{
              mainMenu();
            }
        }
        else if (ifSort == 2)
        {
          printf("Please enter the train id: ");
          getchar(); // Consume the trailing newline character
          char trainId[10];
          scanf("%s", trainId);
          findTrain(trainId);
        }
        else{
          mainMenu();
        }
}

void findTrain(const char* trainId) {
  int trainNotFound;
    FILE* file = fopen("train.txt", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

  
    // Read and discard the first line (number of trains)
    int numTrains;
    fscanf(file, "%d", &numTrains);

    Train train;
    int trainFound = 0;

    while (fscanf(file, "%s %s %s %s %d %d %s", train.trainId, train.startingPoint, train.destination, train.departureTime, &train.cost, &train.compartments, train.seatType) != EOF) {
        if (strcmp(train.trainId, trainId) == 0) {
            trainFound = 1;
            printf("\n\n==============================================================================================\n");
            printf("Train found!\n");
            printf("Train ID: %s\n", train.trainId);
            printf("Starting Point: %s\n", train.startingPoint);
            printf("Destination: %s\n", train.destination);
            printf("Departure Time: %s\n", train.departureTime);
            printf("Cost: %d\n", train.cost);
            printf("Compartments: %d\n", train.compartments);
            printf("Seat Type: %s\n", train.seatType);
            printf("==============================================================================================\n");
            showCompartment(train.compartments, train.trainId);
            break;
        }
    }

    fclose(file);

    if (!trainFound) {
        printf("\nTrain with ID %s not found. Press 1 to view train list or any other key to go back to main menu.\n", trainId);
        scanf("%d", &trainNotFound);
        if (trainNotFound == 1) {
            displayAllTrains();
        }
        else{
          mainMenu();
        }
    }
}

void showCompartment( int compartment, const char* trainId){
    int choosenCompartment;
    printf("Train Compartments:\n");
    printf("    +----------------");
    for (int i = 0; i < compartment; ++i) {
        printf("----------------");
    }
    printf("+\n");
    printf("  <(     Engine      |");
    for (int i = 0; i < compartment; ++i) {
        printf(" Compartment %2d|", i + 1);
    }
    printf("\n");
    printf("    +----------------");
    for (int i = 0; i < compartment; ++i) {
        printf("----------------");
    }
    printf("+\n");
    printf("\n\n\nPlease Enter Your Preferred Compartment:");
    printf("\n\t1)First compartment is First AC\n\t2)Second compartment is Exectuive Class\n\t3)Third compartment is Third AC\n\t4)Fourth compartment is Sleeper\n\t5)Rest Are  Unreserved General Class\n");
    scanf("%d", &choosenCompartment);
    showTickets(choosenCompartment, trainId);
}

void randomlyBookSeats(int* seats, int numSeats) {
    srand(time(NULL));  // Seed for random number generation
    int seatsToBook = numSeats / 2;

    for (int i = 0; i < seatsToBook; ++i) {
        int randomSeat;
        do {
            randomSeat = rand() % numSeats;
        } while (seats[randomSeat] == 1);  // Keep generating until an available seat is found

        seats[randomSeat] = 1;  // Book the seat
    }
}

void showTickets(int choosenCompartment, const char* trainId) {
  int numSeats = 0;
    if (choosenCompartment == 1 || choosenCompartment == 2 || choosenCompartment == 3)
    {
       numSeats = 54;
    } else if (choosenCompartment == 4)
    {
       numSeats = 18;
    } else
    {
       numSeats = 72;
    }
    int seats[MAX_PASSENGERS];
    int bookedSeats[MAX_PASSENGERS];

    // Initialize seats to available
    for (int i = 0; i < numSeats; ++i) {
        seats[i] = 0;
    }

    // Randomly book seats
    randomlyBookSeats(seats, numSeats);

    for (int i = 0; i < numSeats; ++i) {
        bookedSeats[i] = seats[i];
    }

    // Display the ticket layout
    printf("\nYour chosen compartment is: %d", choosenCompartment);
    if (choosenCompartment == 1 || choosenCompartment == 2 || choosenCompartment == 3)
    {
    printf("\nThe seat type is: %s", "AC Seater");
    } else if (choosenCompartment == 4)
    {
    printf("\nThe seat type is: %s", "Sleeper");
    } else
    {
    printf("\nThe seat type is: %s", "Seater");
    }
    printf("\nThe Tickets are:\n");
    printf("\t1) Red Are Booked Tickets\n");
    printf("\t2) Green Are Available Tickets\n");

    for (int i = 0; i < numSeats; ++i) {
        if (i % 6 == 0) {
            printf("\n");
        }
        if (i % 18 == 0) {
            printf("\n");
        }

        if (seats[i] == 1) {
            printf("\033[1;31m");
        } else {
            printf("\033[1;32m");
        }

        printf("\t| Seat %2d |", i + 1);
        printf("\033[0m");
    }

    printf("\n");
    printf("Enter how many tickets you want to buy: ");
    int ticketsToBuy;
    scanf("%d", &ticketsToBuy);
    printf("\n");

    int TicketsNums[MAX_PASSENGERS];

    for (size_t i = 0; i < ticketsToBuy; i++) {
        do {
            printf("Enter your %d ticket number: ", i + 1);
            scanf("%d", &TicketsNums[i]);
            printf("\n");

            // Check if the entered ticket number is valid and not already booked
            if (TicketsNums[i] < 1 || TicketsNums[i] > numSeats) {
                printf("Invalid ticket number. Please enter a valid ticket number.\n");
            } else if (bookedSeats[TicketsNums[i] - 1] == 1) {
                printf("Ticket %d is already booked. Please choose another ticket.\n", TicketsNums[i]);
            }
        } while (TicketsNums[i] < 1 || TicketsNums[i] > numSeats || bookedSeats[TicketsNums[i] - 1] == 1);

        // Mark the selected ticket as booked
        bookedSeats[TicketsNums[i] - 1] = 1;
    }

    printf("Please Complete the payment to Confirm your bookings!!!\n");
    payNow(trainId, ticketsToBuy, choosenCompartment, TicketsNums);

}

void payNow(const char* trainId, int ticketsToBuy, int choosenCompartment, int ticketsNums[MAX_PASSENGERS]) {
    float totalPrice = 0;
    int wantToPay;
    FILE* file = fopen("train.txt", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

    // Read and discard the first line (number of trains)
    int numTrains;
    fscanf(file, "%d", &numTrains);

    Train train;

    while (fscanf(file, "%s %s %s %s %d %d %s", train.trainId, train.startingPoint, train.destination, train.departureTime, &train.cost, &train.compartments, train.seatType) != EOF) {
        if (strcmp(train.trainId, trainId) == 0) {
            if (choosenCompartment == 1 || choosenCompartment == 2)
            {
              totalPrice = ticketsToBuy * train.cost * 2.5;
            } 
            else if(choosenCompartment == 3){
              totalPrice = ticketsToBuy * train.cost * 1.8;
            }
            else if (choosenCompartment == 4)
            {
              totalPrice = ticketsToBuy * train.cost * 1.4;
            } else
            {
              totalPrice = ticketsToBuy * train.cost;
            }
            break;
        }
    }

    fclose(file);
    printf("\nYour Total Cost Is: %2f", totalPrice);
    printf("\nPress 1 to continue to pay or press 2 to go back to ticket selection!!\n");
    scanf("%d", &wantToPay);
    if (wantToPay == 1)
    {
      paymentGateway(trainId, ticketsToBuy, choosenCompartment, totalPrice, ticketsNums);
    }
    else if (wantToPay == 2)
    {
      findTrain(trainId);
    }

}

int isAllDigits(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) {
            return 0; // Not all characters are digits
        }
    }
    return 1; // All characters are digits
}

void paymentGateway(const char* trainId, int ticketsToBuy, int choosenCompartment, float totalPrice, int ticketsNums[MAX_PASSENGERS])
  {
    int gatewayChoice;
    int wantToPay;
    char cardNumber[100];
    char expiryDate[100];
    char cvv[10];
    clearTerminal();
    printf("\n\n\n\n\n==============================================================================================\n\n");
    printf("\t\t\t\033[1;31mTrain RESERVATION\033[0m\t\t");
    printf("\n\n==============================================================================================\n");
    printf("\n====================");
    redColor();
    printf("  PAYMENT GATEWAY  ");
    resetColor();
    printf("=====================\n\n");
    printf("   \033[1;31m[1]\033[0m CREDIT or DEBIT CARD\n\n");
    printf("   \033[1;31m[2]\033[0m UPI (QR CODE)\n\n");
    printf("   \033[1;31m[3]\033[0m CANCEL BOOKING\n\n");
    printf("   \033[1;31m[4]\033[0m PAYMENT INFO\n\n");
    printf("   \033[1;31m[5]\033[0m GO BACK TO TICKET BOOKING\n\n");
    printf("   \033[1;31m[6]\033[0m EXIT TO MAIN MENU\n\n");
    scanf("%d", &gatewayChoice);
    switch (gatewayChoice)
    {
    case 1:
      clearTerminal();
      yellowColor();
      printf("\nYou have chosen Card payment. Please enter card details to complete the payment:\n");
      printf("\nTo cancel payment please enter '7887' in card number field\n");
      resetColor();
      printf("\n\n");

      do {
          printf("Enter Card Number (16 digits): ");
          scanf("%s", cardNumber);
          if (cardNumber[0] == '7' && cardNumber[1] == '8' && cardNumber[2] == '8' && cardNumber[3] == '7')
          {
              findTrain(trainId);
              break;
          }
          if (strlen(cardNumber) != 16 || !isAllDigits(cardNumber)) {
              printf("Invalid card number. Please enter a valid 16-digit number.\n");
          }
      } while (strlen(cardNumber) != 16 || !isAllDigits(cardNumber));

      printf("\nEnter Expiry Date: in format (mm/yyyy) ");
      scanf("%s", expiryDate);
      do {
          printf("Enter CVV  (3 digits): ");
          scanf("%s", cvv);
          if (strlen(cvv) != 3 || !isAllDigits(cvv)) {
              printf("Invalid card number. Please enter a valid 3-digit number.\n");
          }
      } while (strlen(cvv) != 3 || !isAllDigits(cvv));
      printf("\n\n");
      sleepProgram(5);
      printf("Payment Successful");
      printf("\nYou will be redirected to Tickets Section in 5 seconds\n");
      sleepProgram(5);
      confirmTickets(trainId, ticketsToBuy, choosenCompartment, ticketsNums, totalPrice);
      break;
    
    case 2:
      
      clearTerminal();
      yellowColor();
      printf("\nYou have chosen QR payment. Please scan following QR code to make payment\n");
      printf("\nEnter the code obtained by scanning the QR code. ");
      redColor();
      printf("\nPlease Confirm Amount To Be Paid Before Scanning QR Code");
      printf("\nTo cancel payment please enter '7887'\n");
      resetColor();
      printf("\n\n");
      char* paymentInfo = calloc(1000, sizeof(char));
      sprintf(paymentInfo + strlen(paymentInfo), "\nTotal Cost: %.2f\n", totalPrice);
      int i = verifyByQrWithText("Your Code Is:", paymentInfo);
      free(paymentInfo);
      if (i == 1)
      {
          printf("Payment cancelled");
          printf("\nYou will be redirected to train section in 5 seconds\n");
          sleepProgram(5);
          findTrain(trainId);
      }
      else if(i == 0)
      {
          printf("Payment successful");
          confirmTickets(trainId, ticketsToBuy, choosenCompartment, ticketsNums, totalPrice);
          printf("\nYou will be redirected to show ticket booking in 5 seconds\n");
          sleepProgram(5);
          confirmTickets(trainId, ticketsToBuy, choosenCompartment, ticketsNums, totalPrice);
      }
      else{
          printf("Payment failed");
          printf("\nYou will be redirected to tickets section of this train in 5 seconds\n");
          findTrain(trainId);
      }
      break;
    
    case 3:
      printf("\nYou have cancelled ticket booking.\n");
      printf("\nYou will be redirected to train section in 5 seconds\n");
      sleepProgram(5);
      findTrain(trainId);
      break;
    
    case 4:
      printf("\nYour Payment Information Is As Following:\n");
      printf("Total Cost: %f\n", totalPrice);
      printf("Total Tickets: %d\n", ticketsToBuy);
      printf("Train ID: %s\n", trainId);
      printf("Compartment: %d\n", choosenCompartment);
      if (choosenCompartment == 1 || choosenCompartment == 2 || choosenCompartment == 3)
      {
      printf("\nThe seat type is: %s", "AC Seater");
      } else if (choosenCompartment == 4)
      {
      printf("\nThe seat type is: %s", "Sleeper");
      } else
      {
      printf("\nThe seat type is: %s", "Seater");
      }
      printf("\nPress 1 to continue to payment or press any other key to go back to main menu.\n");
      scanf("%d", &wantToPay);
      if (wantToPay == 1)
      {
          paymentGateway(trainId, ticketsToBuy, choosenCompartment, totalPrice, ticketsNums);
      } else
      {
          mainMenu();
      }
      break;

    case 5:
      printf("You will be redierected to Train Section in 5 seconds.\n");
      sleepProgram(5);
      findTrain(trainId);
      break;
    
    case 6:
      printf("You will be redierected to main menu in 5 seconds.\n");
      sleepProgram(5);
      mainMenu();
      break;
    
    default:
      printf("Invalid option. You will be redierected to main menu in 5 seconds.\n");
      sleepProgram(5);
      mainMenu();
      break;
    }
}

long long generate10DigitRandomNumber() {
    long long randomNumber = 0;

    // Seed the random number generator with the current time
    srand((unsigned int)time(NULL));

    // Generate each digit randomly
    for (int i = 0; i < 10; ++i) {
        randomNumber = randomNumber * 10 + rand() % 10;
    }

    return randomNumber;
}

void writeReservationToFile(const Reservation* passenger) {
    FILE* file = fopen("reservations.txt", "a");
    if (file == NULL) {
        printf("Error opening reservations file.\n");
        exit(1);
    }
    fprintf(file, "%s %s %s %d %.2f %d %d %s %s %lld", passenger->name, passenger->userId, passenger->trainId,
            passenger->compartment, passenger->cost, passenger->noOfSeats, passenger->seats , passenger->seatType, passenger->status, passenger->pnr);
    fclose(file);
}

void confirmTickets(const char* trainId, int ticketsToBuy, int choosenCompartment, int* ticketsNums, float totalPrice) {
    int confirmedNowGoBack;
    clearTerminal();
    printf("\n\n\n\n\n==============================================================================================\n\n");
    printf("\t\t\t\033[1;31mTrain RESERVATION SUCCESSFUL\033[0m\t\t");
    printf("\n\n==============================================================================================\n");
    printf("\nPlease Enter following details to complete the booking:\n");
    long long pnr = generate10DigitRandomNumber();

    Reservation passengers[MAX_PASSENGERS];
    for (int i = 0; i < ticketsToBuy; ++i) {
        printf("Enter passenger name for Seat %2d: ", i + 1);
        scanf("%s", passengers[i].name);
        strcpy(passengers[i].userId, userIdPtr);
        strcpy(passengers[i].trainId, trainId);
        passengers[i].compartment = choosenCompartment;
        passengers[i].cost = totalPrice;
        passengers[i].noOfSeats = 1;
        passengers[i].seats = ticketsNums[i];
        if (choosenCompartment == 1 || choosenCompartment == 2 || choosenCompartment == 3) {
            strcpy(passengers[i].seatType, "(AC)Seater");
        } else if (choosenCompartment == 4) {
            strcpy(passengers[i].seatType, "Sleeper");
        } else {
            strcpy(passengers[i].seatType, "Seater");
        }
        strcpy(passengers[i].status, "CONFIRMED");
        passengers[i].pnr = pnr;
        writeReservationToFile(&passengers[i]);
    }
    printf("\nThank You For Your Details. Your PNR is: %lld\n", pnr);
    printf("\n====================");
    redColor();
    printf("  TICKET BOOKING DETAILS:  ");
    resetColor();
    printf("=====================\n\n");
    printf("\nTrain ID: %s\n", trainId);
    printf("Compartment: %d\n", choosenCompartment);
    printf("Total Tickets: %d\n", ticketsToBuy);
    printf("Total Cost: %.2f\n", totalPrice);
    printf("Seat Numbers: ");
    for (int i = 0; i < ticketsToBuy; i++) {
        printf("%d \t", ticketsNums[i]);
    }
    printf("\nYour PNR is %lld\n", pnr);
    printf("\nPress any key to go back to the main menu.\n");
    scanf("%d", &confirmedNowGoBack);  // Use %d for integer input
    mainMenu();
}

void PrintToBeDeletedReservation(long long pnr) {
    clearTerminal();
    FILE* inputFile = fopen("reservations.txt", "r");
    if (inputFile == NULL) {
        printf("Error opening reservations file for reading.\n");
        exit(1);
    }

    Reservation currentReservation;
    int reservationFound = 0;

    while (fscanf(inputFile, "%s %s %s %d %f %d", currentReservation.name, currentReservation.userId,
              currentReservation.trainId, &currentReservation.compartment, &currentReservation.cost,
              &currentReservation.noOfSeats) != EOF) {
    fscanf(inputFile, " %d", &currentReservation.seats);

    fscanf(inputFile, " %s %s %lld", currentReservation.seatType, currentReservation.status, &currentReservation.pnr);
    
        if (currentReservation.pnr == pnr) {
            reservationFound = 1;

            // Display reservation details
            printf("\nReservation Details:\n");
            printf("Name: %s\n", currentReservation.name);
            printf("Train ID: %s\n", currentReservation.trainId);
            printf("Compartment: %d\n", currentReservation.compartment);
            printf("Cost: %.2f\n", currentReservation.cost);
            printf("Number of Seats: %d\n", currentReservation.noOfSeats);
            printf("Seat Numbers: %d \n", currentReservation.seats);
            printf("Seat Type: %s\n", currentReservation.seatType);
            printf("Status: %s\n", currentReservation.status);
            printf("PNR: %lld\n", currentReservation.pnr);
            printf("\n");
        }
    }

    fclose(inputFile);

    if (reservationFound) {
        printf("Do you want to cancel this reservation? (1 for yes, 0 for no): ");
        int confirmation;
        scanf("%d", &confirmation);

        if (confirmation == 1) {
            deleteReservationByPnr(pnr);
            printf("Reservation cancelled successfully.\n");
            printf("Money will be refunded to original payment source within 3-4 business days.\n");
        } else {
            printf("Reservation not cancelled.\n");
        }
    } else {
        printf("Reservation not found for the given PNR.\n");
    }
    printf("You will be redirected to the main menu in 5 seconds\n");
    sleepProgram(5);
    mainMenu();
}

void deleteReservationByPnr(long long pnr) {
    FILE* inputFile = fopen("reservations.txt", "r");
    FILE* tempFile = fopen("temp.txt", "w");
    if (inputFile == NULL || tempFile == NULL) {
        printf("Error opening files for reading/writing.\n");
        exit(1);
    }

    Reservation currentReservation;

    while (fscanf(inputFile, "%s %s %s %d %f %d", currentReservation.name, currentReservation.userId,
                  currentReservation.trainId, &currentReservation.compartment, &currentReservation.cost,
                  &currentReservation.noOfSeats) != EOF) {
        // Read seat number
        fscanf(inputFile, " %d", &currentReservation.seats);

        fscanf(inputFile, " %s %s %lld", currentReservation.seatType, currentReservation.status, &currentReservation.pnr);

        // Skip the reservation with the specified PNR
        if (currentReservation.pnr == pnr) {
            continue;
        }

        // Write the reservation to the temp file
        fprintf(tempFile, "%s %s %s %d %.2f %d", currentReservation.name, currentReservation.userId,
                currentReservation.trainId, currentReservation.compartment, currentReservation.cost,
                currentReservation.noOfSeats);
        fprintf(tempFile, " %d", currentReservation.seats);
        fprintf(tempFile, " %s %s %lld\n", currentReservation.seatType, currentReservation.status, currentReservation.pnr);
    }

    fclose(inputFile);
    fclose(tempFile);

    // Rename the temp file to the original file
    remove("reservations.txt");
    rename("temp.txt", "reservations.txt");
}

void cancelBooking(){
    long long pnrCancel;
    printf("\nEnter PNR: ");
    scanf("%lld", &pnrCancel);
    PrintToBeDeletedReservation(pnrCancel);
    clearTerminal();
    printf("\n\n\n\n\n==============================================================================================\n\n");
    printf("\t\t\t\033[1;31mTrain RESERVATION CANCELLED\033[0m\t\t");
    printf("\n\n==============================================================================================\n");
    printf("\nYou have cancelled ticket booking.\n");
    printf("\nYou will be redirected to main menu in 5 seconds\n");
    sleepProgram(5);
    mainMenu();
}

void reservationInfo() {
    long long pnrInfo;
    clearTerminal();
    printf("\nEnter PNR: ");
    scanf("%lld", &pnrInfo);

    FILE* file = fopen("reservations.txt", "r");
    if (file == NULL) {
        printf("Error opening reservations file for reading.\n");
        exit(1);
    }

    Reservation currentReservation;
    int reservationFound = 0;

    while (fscanf(file, "%s %s %s %d %f %d", currentReservation.name, currentReservation.userId, currentReservation.trainId, &currentReservation.compartment, &currentReservation.cost, &currentReservation.noOfSeats) != EOF) {
        fscanf(file, " %d", &currentReservation.seats);  
        fscanf(file, " %s %s %lld", currentReservation.seatType, currentReservation.status, &currentReservation.pnr);  

        if (currentReservation.pnr == pnrInfo) {
            reservationFound = 1;
            printf("\n====================================================================\n");
            printf("\t\t\t\033[1;31mRESERVATION INFORMATION\033[0m\t\t");
            printf("\n====================================================================\n");

            printf("\t||\t\tPassenger Name: %s\n", currentReservation.name);
            printf("\t||\t\tUser ID: %s\n", currentReservation.userId);
            printf("\t||\t\tTrain ID: %s\n", currentReservation.trainId);
            printf("\t||\t\tCompartment: %d\n", currentReservation.compartment);
            printf("\t||\t\tCost: %.2f\n", currentReservation.cost);
            printf("\t||\t\tNumber of Seats: %d\n", currentReservation.noOfSeats);
            printf("\t||\t\tSeat Numbers: %d\n ", currentReservation.seats);
            printf("\t||\t\tSeat Type: %s\n", currentReservation.seatType);
            printf("\t||\t\tStatus: %s\n", currentReservation.status);
            printf("\t||\t\tPNR: %lld\n", currentReservation.pnr);

            printf("\n====================================================================\n");
        }
    }

    fclose(file);

    if (!reservationFound) {
        printf("\nReservation with PNR %lld not found.\n", pnrInfo);
        printf("\nPress 1 to try again or any other key to go back to the main menu.\n");
        int reservationPrintChoice;
        scanf("%d", &reservationPrintChoice);
        if (reservationPrintChoice == 1) {
            reservationInfo();
        } else {
            mainMenu();
        }
    }

    printf("\nPress 1 to check another reservation or Press any key to go back to the main menu.\n");
    int reservationFoundPrintChoice;
    scanf("%d", &reservationFoundPrintChoice);
    if (reservationFoundPrintChoice == 1) {
        reservationInfo();
    } else {
        mainMenu();
    }
}

void checkAllReservations() {
    clearTerminal();
    int currentReservationFound = 0;  // Initialize to 0
    printf("\n==============================================================================================\n");
    printf("\t\t\t\033[1;31mALL YOUR RESERVATIONS \033[0m\t\t");
    printf("\n==============================================================================================\n");

    FILE* inputFile = fopen("reservations.txt", "r");
    if (inputFile == NULL) {
        printf("Error opening reservations file for reading.\n");
        exit(1);
    }

    Reservation currentReservation;

    printf("\nYour Reservations:\n");
    while (fscanf(inputFile, "%s %s %s %d %f %d", currentReservation.name, currentReservation.userId,
              currentReservation.trainId, &currentReservation.compartment, &currentReservation.cost,
              &currentReservation.noOfSeats) != EOF) {
            fscanf(inputFile, " %d", &currentReservation.seats);
            fscanf(inputFile, " %s %s %lld", currentReservation.seatType, currentReservation.status, &currentReservation.pnr);

        if (strcmp(currentReservation.userId, userIdPtr) == 0) {
            currentReservationFound = 1;
            printf("\nReservation Details:\n");
            printf("Name: %s\n", currentReservation.name);
            printf("Train ID: %s\n", currentReservation.trainId);
            printf("Compartment: %d\n", currentReservation.compartment);
            printf("Cost: %.2f\n", currentReservation.cost);
            printf("Number of Seats: %d\n", currentReservation.noOfSeats);
            printf("Seat Numbers: %d ", currentReservation.seats);
            printf("\n");
            printf("Seat Type: %s\n", currentReservation.seatType);
            printf("Status: %s\n", currentReservation.status);
            printf("PNR: %lld\n", currentReservation.pnr);
            printf("\n");
        }
    }

    fclose(inputFile);
    if (currentReservationFound == 0) {
        printf("\nYou Don't Have Any Reservations!!\n");
    }

    printf("\nPress any key to go back to Main Menu\n");
    int allResChoice;
    scanf("%d", &allResChoice);
    if (allResChoice == 1) {
        mainMenu();
    }
    else{
        mainMenu();
    }
}

void showUserInfo(){
    FILE *file = fopen("users.txt", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

    char line[500];
    while (fgets(line, sizeof(line), file) != NULL) {
        User user;
        sscanf(line, "%s %s %s %s", user.userId, user.email, user.password, user.name);

        if (strcmp(user.userId, userId) == 0) {
            clearTerminal();
            printf("\n\n\n\n\n==============================================================================================\n\n");
            printf("\t\t\t\033[1;31mYour Profile\033[0m\t\t");
            printf("\n\n==============================================================================================\n");
            printf("User ID: %s\n", user.userId);
            printf("Name: %s\n", user.name);
            printf("Email: %s\n", user.email);
            printf("Password: %s\n", user.password);
            break;
        }
    }

    fclose(file);
    printf("\nPress 1 to change password or Press any key to go back to Main Menu\n");
    int userInfoChoice;
    scanf("%d", &userInfoChoice);
    if (userInfoChoice == 1) {
        resetPass();
    }
    else{
        mainMenu();
    }
}


void mainMenu(){
    int choice;
    printf("\n\n\n\n\n==============================================================================================\n\n");
    printf("\t\t\t\033[1;31mTrain RESERVATION\033[0m\t\t");

    printf("\n\n==============================================================================================\n");
    printf("\n====================");
    redColor();
    printf("  MAIN MENU  ");
    resetColor();
    printf("=====================\n\n");
    printf("   \033[1;31m[1]\033[0m VIEW TRAIN LIST \n\n");
    printf("   \033[1;31m[2]\033[0m BOOK TICKETS\n\n");
    printf("   \033[1;31m[3]\033[0m CANCEL BOOKING\n\n");
    printf("   \033[1;31m[4]\033[0m RESERVATION INFO\n\n");
    printf("   \033[1;31m[5]\033[0m SEE ALL YOUR RESERVATIONS\n\n");
    printf("   \033[1;31m[6]\033[0m LOGOUT\n\n");
    printf("   \033[1;31m[7]\033[0m DELAYED TRAIN STATUS\n\n");
    printf("   \033[1;31m[8]\033[0m Profile\n\n");
    printf("   \033[1;31m[9]\033[0m EXIT\n\n\n");
    printf("   \033[1;31m[11]\033[0m ADMIN CONTROLS\n\n");
    printf("\n========================================================");
    printf("\n==============================================================================================\n");
    scanf("%d", &choice);
    switch (choice)
    {
    case 1:
        trainListandBook();
        break;
    case 2:
        trainListandBook();
        break;
    case 3:
        cancelBooking();
        break;
    case 4:
        reservationInfo();
        break;
    case 5:
        checkAllReservations();
        break;
    case 6:
        *loggedPtr = 0;
        memset(userIdPtr, '0', strlen(userId));
        LogOrSign();
        break;
    case 7:
        displayDelayedTrains();
        break;
    case 8:
        showUserInfo();
        break;
    case 9:
        exit(0);
        break;
    case 11:
        adminControls();
        break;
    }
}

int main(){
    clearTerminal();
    LogOrSign();
    clearTerminal();
    mainMenu();
}