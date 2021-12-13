#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX 5
typedef struct{
    char name[31];
    char pass[31];
}userInfo;

void generatePass(int idx, userInfo users[], char *filename); 
void getPass(char *filename);
int main(void) {
	userInfo userArr[MAX]; //Structure array to hold up to 5 user's information
    char filename[50] = "data.txt";
	char retry[5], choice[5]; //Using char array to handle faulty inputs such as a character or multiple characters instead of the expected integer input.
	int running=1, valid=0, idx=0; //idx is a counter to keep track of the current user in the userArr structure array
	int ch_int; //To hold the integer version of choice

    printf("\t\tPASSWORD GENERATOR\n");
    printf("\n1. Generate new password\n2. Retrieve password");
    //Keep looping until the user enters a valid integer
    while(!valid){
        printf("\n\nEnter choice> ");
        scanf("%s", &choice);
        ch_int = atoi(choice); //Convert the string to an integer
        /*In case of a successful conversion to integer 1 or 2, make valid=1, so, stop looping for the choice input.
        Otherwise, keep looping in case of faulty numeric or non numeric inputs*/
        if(ch_int==1){
            valid=1;
            generatePass(idx, userArr, filename); //Call the generatePass() function to generate a password for the first user, that is, idx=0

            //Continue to loop and generate passwords until the user presses N or the maximum capacity of 5 users is reached
            while(running){
                printf("\n\nPress Y to generate again. Press N to retrieve password > ");
                scanf("%s", retry); //Using a char array with 5 capacity instead of a char so that the next input prompts are not skipped in case of multiple character inputs by the user
                /*Check if the input string holds only a single character.
                Then check if that character is case insensitive Y or N. If both are true, then generate password for the current user.
                Ask for input again in case of faulty inputs like wrong character, multiple characters or an integer*/
                if((strlen(retry)==1)&&(*retry == 'Y' || *retry == 'y')){
                    idx++; //Increment idx to change the value for the current user every time before calling the password generator function.
                    generatePass(idx, userArr, filename);
                    //If current user's index was 4 then the 5 user capacity has been reached. So stop generating passwords and go to the retrieve function.
                    if(idx==4){
                        printf("\nMaximum password generation limit reached.\n");
                        break;
                    }
                }else if((strlen(retry)==1)&&(*retry == 'N' || *retry == 'n'))
                    running = 0;
                else
                    printf("Invalid input. Try again.");
            }
            getPass(filename); //Retrieving the passwords after generating is finished.
        }else if(ch_int==2){
            valid = 1;
            getPass(filename); //Retrieving previously generated passwords from text file.
        }else
            printf("Invalid input. Try again");
    }
    return 0;
}
void generatePass(int idx, userInfo users[], char *filename){
	char a[100]; //The password will be generated taking random characters from this char array
	int i=0; //Counter for array a

    //Fill character array a with all uppercase and lowercase alphabets
    for(char alph = 'A'; alph<='Z'; alph++){
        a[i] = alph;
        i++;
    }
    for(char alph = 'a'; alph<='z'; alph++){
        a[i] = alph;
        i++;
    }

	char n[3]; //Number of characters. Using string instead of int for the aforementioned reasons.
    char num_flag[5], sym_flag[5]; //To check if user wants to include numbers or symbols in the password
	char sym_list[10] = {'!', '?', '*', '@', '$', '^', '&', '#', '%', '~'};
	int valid=0;
    int n_int; //To hold the converted integer version of n.
    printf("\nEnter your first name> ");
    scanf("%s", users[idx].name);
    users[idx].name[0] = toupper(users[idx].name[0] ); //Capitalize the current user's first name and store it in their name component

	while(!valid){
		printf("\nEnter number of characters (1-30)> ");
    	scanf("%s", &n);
    	n_int = atoi(n); //Convert the number of characters from string to integer
		printf("\nInclude numbers (Y/N)> ");
    	scanf("%s", num_flag);
    	printf("\nInclude symbols (Y/N)> ");
    	scanf("%s", sym_flag);
        
        //Conditions in order to be considered valid inputs
		int cond1 = n_int>0 && n_int<=30;
		int cond2 = *num_flag=='Y' || *num_flag== 'y' || *num_flag=='N' || *num_flag=='n';
		int cond3 = *sym_flag=='Y' || *sym_flag== 'y' || *sym_flag=='N' || *sym_flag=='n';
		int cond4 = strlen(num_flag)==1 && strlen(sym_flag)==1;
		if(cond1 && cond2 && cond3 && cond4)
			valid=1;
		else
			printf("Invalid input(s). Try again.\n\n");
	}
    //If the user decides to include numbers, loop through the digits, convert them to characters and append to the character array a
    if(*num_flag=='Y' || *num_flag=='y'){
        for(int j=0; j<=9; j++){
            a[i] = j+'0';
        	i++;
        }
    }
    //If the user decides to include symbols, loop through the array of symbols and append to the character array a
	if(*sym_flag == 'Y' || *sym_flag == 'y'){
		for(int x=0; x<10; x++){
			a[i] = sym_list[x];
			i++;
		}
	}

	int counter=0, r;
	//Generate n number of random integers where n is the number of characters given by the user.
	while(counter<n_int){
		r = rand()%i; //Generate a random integer between 0 and i (which is the total length of array a) and store it in r
		
		/*In each iteration, take the character at the randomly generated index r from char array a. 
		Put that character in the current user's pass char array component's current index (That is, value of counter for each iteration)*/
		users[idx].pass[counter] = a[r];
		counter++;
	}
	users[idx].pass[n_int] = '\0'; //Terminate the pass char array with a null character at nth index to convert into string 
	printf("\nPassword generated and saved to text file\n");
	printf("%s's password:  %s", users[idx].name, users[idx].pass); //Print the current user's name and generated password to the terminal

	FILE *fp;
	fp = fopen(filename, "w"); 
	if (fp != NULL){
        //Store each user's first name and their password in the text file.
        fprintf(fp, "%s\n", users[idx].name);
        fprintf(fp, "%s\n", users[idx].pass);
        fclose(fp);
	}else
        printf("File not found");
}
void getPass(char *filename){
    printf("\nPRESS Q TO QUIT!");
    userInfo temp[5]; //To hold the previously generated users' information 
    int found=0;
    FILE *fp;
    fp = fopen(filename, "r");
    if(fp != NULL){
        //Keep looping until the user enters Q/q
        while(1){
            char user_name[30];
            printf("\n\nEnter your first name to retrieve password> ");
            scanf("%s", user_name);
            user_name[0] = toupper(user_name[0]); //Capitalize the user's first name

            if(*user_name == 'Q' || *user_name == 'q') break;

            for(int z=0; z<10; z++){
                //For each iteration, get a user's first name and password
                fgets(temp[z].name, 31, fp); 
                fgets(temp[z].pass, 31, fp);
                //Compare the first name from the text file with the one entered by the user
                if(!strncmp(temp[z].name, user_name, strlen(user_name))){
                    //In case of a match, print the user's password and set found=1 to stop the loop. 
                    printf("Your password is:  %s", temp[z].pass);
                    found=1;
                }
            }
            //If found=0 after the loop completes then the user's information is not in the text file. 
            if(!found)
                printf("User not found. Try again.");
            found=0; //Reset value of found for the next iteration
        }
    }else
        printf("File not found");
}
