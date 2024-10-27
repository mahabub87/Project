#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct currentValidID {
    int year;
    char branch[6];
    int totalVoters;
};

typedef struct candidate {
    int cid;
    char cname[20];
    int votes;
} CANDIDATE;

// GLOBALS
struct currentValidID currentValidID;
CANDIDATE candidateArray[20];
int numberOfCandidates;
char studentVotes[200];

// Utility functions for extracting information from user ID
int extractYear(const char userID[15]) {
    int year = 0;
    for (int i = 0; i < 4; i++) {
        year = (year * 10) + (userID[i] - '0');
    }
    return year;
}

int extractRollNo(const char userID[15]) {
    int rollno = 0;
    for (int i = 9; i < 14; i++) {
        rollno = (rollno * 10) + (userID[i] - '0');
    }
    return rollno;
}

int checkBranchCode(const char userID[15]) {
    char branchCode[6];
    strncpy(branchCode, &userID[4], 5);
    branchCode[5] = '\0';
    return strcmp(branchCode, currentValidID.branch) == 0;
}

// Admin authentication
int authenticateAdmin() {
    char username[15], password[6];
    printf("\nEnter username: ");
    scanf("%s", username);

    if (strcmp(username, "Admin") != 0) {
        return 0;
    }

    printf("Enter Password: ");
    for (int i = 0; i < 5; i++) {
        password[i] = getchar();
        printf("*");
    }
    password[5] = '\0';

    return strcmp(password, "admiN") == 0;
}

// Function to ban specific user IDs
void banID() {
    FILE *fp = fopen("Banned.txt", "w");
    if (!fp) {
        printf("Error: Unable to create Banned.txt\n");
        return;
    }

    printf("Enter roll numbers to ban (Press 0 to stop): ");
    int input;
    while (1) {
        printf("\nEnter Roll No: ");
        scanf("%d", &input);
        if (input == 0) break;
        studentVotes[input - 1] = '$';
        fprintf(fp, "%d\n", input);
    }
    fclose(fp);
    printf("Banned IDs saved to Banned.txt successfully\n");
}

// Initialize new election
void initiateNewElection() {
    printf("\nNew Election Initiation:\n");

    printf("\nElection Year: ");
    scanf("%d", &currentValidID.year);
    printf("Enter branch code: ");
    scanf("%s", currentValidID.branch);
    printf("Enter max roll number: ");
    scanf("%d", &currentValidID.totalVoters);
    printf("Enter number of candidates: ");
    scanf("%d", &numberOfCandidates);

    for (int i = 0; i < currentValidID.totalVoters; i++) {
        studentVotes[i] = '0';
    }

    for (int i = 0; i < numberOfCandidates; i++) {
        candidateArray[i].cid = i + 1;
        printf("Enter name of candidate %d: ", i + 1);
        scanf(" %s", candidateArray[i].cname);
        candidateArray[i].votes = 0;
    }
}

// Save current election info to file
void saveElectionInfoInFile() {
    FILE *fp = fopen("ElectionInfo.txt", "w");
    if (!fp) {
        printf("Error creating ElectionInfo.txt\n");
        return;
    }
    fprintf(fp, "%d\n%s\n%d\n%d", currentValidID.year, currentValidID.branch, currentValidID.totalVoters, numberOfCandidates);
    fclose(fp);
    printf("Election info saved successfully\n");
}

// Student panel to allow voting
void studentPanel() {
    char userID[15];
    char voteInput;

    while (1) {
        printf("\nEnter user ID (or 0 to exit): ");
        scanf("%s", userID);
        if (strcmp(userID, "0") == 0) return;

        if (isValid(userID) != 1) {
            printf("Invalid User ID\n");
            continue;
        }

        if (isBanned(userID) != 0) {
            printf("This User ID is banned\n");
            continue;
        }

        if (isVoted(userID) != 0) {
            printf("Your ID has already voted\n");
            continue;
        }

        printf("\nCandidates for election:\n");
        for (int i = 0; i < numberOfCandidates; i++) {
            printf("%d. %s\n", i + 1, candidateArray[i].cname);
        }

        printf("Your Vote (Enter Candidate Number): ");
        voteInput = getchar();

        if (voteInput < '1' || voteInput > '0' + numberOfCandidates) {
            printf("Invalid vote. Try again.\n");
            continue;
        }

        saveVote(userID, voteInput);
        printf("\nThanks for voting!\n");
    }
}

// Main function
int main() {
    while (1) {
        printf("\n1. Student Panel\n2. Admin Panel\n3. Exit\nOption: ");
        char input;
        scanf(" %c", &input);

        switch (input) {
            case '1':
                studentPanel();
                break;
            case '2':
                if (authenticateAdmin()) adminPanel();
                break;
            case '3':
                return 0;
            default:
                printf("Invalid option\n");
        }
    }
    return 0;
}
