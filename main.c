//MORANO, JANAH PATRICIA
//DALUMPINES, SOPHIA
//BSCS

#include <stdio.h>
#include <stdlib.h>
#include <string.h> //string functions
#include <windows.h>//for gotoxy
#include <time.h>   //for loading animation
#include <conio.h>  //for getch()
#include <ctype.h>  //for isdigit()
#define MAX 100

//struct for voters
typedef struct data{
    char name[50];
    char pin[7];
    char yrCourse[31];
    int idNumber;
}DATA;

// struct for votes
typedef struct rec {
    int stID;
    int pr,vp, sec;
}REC;

typedef struct list{
    DATA stud [MAX];
    REC candidates [MAX];
    int rlast;
    int vlast;
}LIST;

LIST l;

//GLOBAL VARIABLES
char* checkDrives[2] = {"G:\\checkFd.dbf", "E:\\checkFd.dbf"};     //to check if usb is inserted
char* drives[2] = {"G:\\votersList.dbf", "E:\\votersList.dbf"};    //store voter's information
int p,IdNum, choice=0, back=0, ctr = -1, vcount[7], success;
char pinCode [7], c;

//CANDIDATES
char president [2][10]= {"BING", "GELLER, M"};
char vpres [2][10]= {"BUFFAY", "TRIBBIANI"};
char secretary [2][10] = {"GELLER, R","GREENE"};

//FUNCTIONS
int menu();
void makenull();
int insertCard();
void retrieve();
void retrieveSummary();
void menuControls (int ch) ;
void registration ();
void accValidation();
void voting ();
void votingUI ();
int checkFd();
int isFull();
void insert(DATA vtrs);
void insertVote (REC vt);
int locateName (char nameloc[31]);
int locateId (int Idloc);
int locateVtr (int voter);
void saveToCard();
void saveSummary();
void voteCount(int x);
void voteSummary();
void viewVotes ();

//UI FUNCTIONS
void delay(int ms);
void formLayout();
void usbAnimation();
void box(int x, int y, int len, char *text);
void borders();
void clear(char *label);
void dontRemove(int input);
void gotoxy(int x, int y);
void printToxy(int x, int y, char* text);
COORD coord = {0,0};

int main(){
    system("color B");
    makenull();
    borders();
   if(insertCard()){
        retrieve ();        //retrieve voters' info
        retrieveSummary();  //retrieve the votes

        while (1) {

            system ("cls");
            borders();
            back=0;
            printToxy (50, 6, "MAIN MENU");
            box (42, 7, 47," 1");
            box (48, 7, 67,"REGISTER AS VOTER");
            box (42, 11, 47," 2");
            box (48, 11, 67,"      VOTE ");
            box (42, 15, 47," 3");
            box (48, 15, 67,"      EXIT");
            printToxy (46, 21,"Select 1-3: ");
            gotoxy (58,21);
            scanf ("%d", &choice);
            menuControls (choice);
        }

    }
    return 0;
}

void menuControls (int ch)
{

    switch (ch){
    case 1:                       // for registration
        registration ();
        if (success==0){
            printToxy(37,21,"R E G I S T R A T I O N   S U C C E S S F U L");
            printToxy(44,22,"[ You can now proceed to Voting ]");
        }
        getch ();
        break;
    case 2:                       // for voting
        accValidation ();
        if (back==0){
            votingUI();
            viewVotes ();
        } else
        break;
    case 3:                       // to exit program
        remove(checkDrives[ctr]); // remove the file from usb (metaphor for removing card) so that if another user will register/ vote,  checking drive connectivity will function correctly
        printf("\n\n");
        exit (0);
        break;
    default:
        printToxy(46,21,"INVALID CHOICE!");
        getch();
        break;
    }
}

void registration (){
DATA voters;
int i=0, id, count=0, pinMatch= 0, pinChar = 0;
char nomPin [7];

        system("cls");
        formLayout();
        gotoxy(42, 6);
        scanf(" %[^\n]s",voters.name);
        strlwr(voters.name);

        do {                        // to check if the input student ID is a 4-digit number
            count=0;
            id= 0;
            gotoxy(53, 10);
            scanf("%d",&voters.idNumber);
            id = voters.idNumber;
            while(id!=0)  {
                id =id/10;
                count++;
            }
            if (count!=4){
                printToxy(40,22,"I N V A L I D  S T U D E N T   N U M B E R !");
                clear("IdBox");
                }
        } while (count!=4);
        clear("result");

        gotoxy(75, 10);
        scanf(" %[^\n]s",voters.yrCourse);


        do{
            pinMatch = 0;
            pinChar = 0;
            gotoxy(60, 14);
             for(int i = 0; i < 6; i++){            // nominating pin
                c = _getch();
                if(c==8){                           // for user to delete character in PIN
                    i--;
                    putch('\b');
                    putch(' ');
                    putch('\b');
                    i--;
                    continue;
                } else if (c==13|| c==32 || c== 9){ // enter, tab and space key not to be included in PIN
                    continue;
                } else {
                    nomPin[i]= c + 9;               // +9 for encryption
                    putch('*');
                }
            }
            nomPin[6]='\0';

            gotoxy(60, 18);
            for(int i = 0; i < 6; i++){            // confirming pin
                c = _getch();
                if(c==8)
                {
                    i--;
                    putch('\b');
                    putch(' ');
                    putch('\b');
                    i--;
                    continue;
                } else if (c==13|| c==32 || c== 9){
                    continue;
                } else {
                    voters.pin[i]= c + 9;
                    putch('*');
                }
            }
            voters.pin[6]='\0';


              for(int i = 0; i < 6; i++){          // check if there's a char input in PIN
                if(isdigit(voters.pin[i]-9)==0){
                    pinChar = 1;
                    break;
                }
            }

            if( strcmp(nomPin, voters.pin)!= 0){   // check if the nominated and confirmation pin are same
                printToxy(42,22,"P I N   D I D   N O T   M A T C H !");
                pinMatch = 1;
                _getch();
                clear("result");
                clear("pinBoxes");
            }
             else if(pinChar == 1){
                printToxy(36,22,"P I N   C O N T A I N S   A   C H A R A C T E R");
                _getch();
                clear("result");
                clear("pinBoxes");
            }

        }while(pinMatch || pinChar);

    insert (voters);      // call insert function to add records in database

}

void accValidation (){
int i, p=0, attempts=2;

    system("cls");
    printToxy (38, 3,": : : WELCOME TO TUP 2021 USG ELECTIONS : : :");
    box(45,6,75, "Enter Student Id:" );
    box(45,10,75, "Enter PIN:" );
    box(35,15,85, "");
    printToxy(57,17,"- - -");

    gotoxy(65,8);
    scanf(" %d",&IdNum);
    p= locateId (IdNum);                // check if the student is registered or has a record in database
     if (p < 0 ){
         printToxy(47,17,"STUDENT ID NOT REGISTERED");
         getch();
         printToxy(49,17,"                      ");
         printToxy(41,17,"[1] Try Again  [2] Register  [3] Exit ");
         printToxy (52, 20, "Select option: ");
         scanf(" %d", &choice);
         if (choice==1){                 // retry entering ID
                menuControls(2);
            }else if (choice ==2) {      // if not registered, go to registration
                back=1;
                menuControls(1);
            } else{
                exit (0);                // exit the program
            }
        } else if (locateVtr (IdNum) >= 0 ){             // check if the student has voted already
             clear ("vMssgBox");
             printToxy(48,17,"A L R E A D Y   V O T E D");
             getch();
             clear ("vMssgBox");
             printToxy(52,17," E X I T I N G . . .");    // if yes, automatically exits the program
             delay(2000);
             gotoxy(0,20);
             exit(0);
    }else {
            do{                                         // input password
                if(i == 3){                             // user has three attempts
                    clear ("vMssgBox");
                    printToxy(52,17," E X I T I N G . . .");
                    delay(2000);
                    gotoxy(0,20);
                    exit(0);                            // exits if attempt=3
                  }
                    gotoxy(58,12);
                    for(int i = 0; i < 6; i++){         // input pin
                        c = _getch();
                        if(c==8){
                            i--;
                            putch('\b');
                            putch(' ');
                            putch('\b');
                            i--;
                            continue;
                        } else if (c==13|| c==32 || c== 9){
                            continue;
                        } else {
                            pinCode[i]= c + 9;
                            putch('*');
                        }
                    }
                    pinCode[6]='\0';

                    if(strcmp(pinCode,l.stud [p]. pin) != 0){  // check if the pin is similar to the pin in the database
                        printToxy(38,17,"I N V A L I D   P I N !");
                        gotoxy(62,17);
                        printf("[ attempts left: %d ]", attempts);
                        attempts--;
                        _getch();
                        clear ("vMssgBox");
                        clear("vPinBox");
                    }
                    i++;
            } while(strcmp(pinCode,l.stud [p]. pin) != 0);    // make the user input the pin again while it pins are not similar
    printToxy(52, 17, " W E L C O M E ! ");
    getch ();
  }

}

void makenull(){
    l.rlast = -1;   // for the index of voters who registered
    l.vlast = -1;   // for the index of vote summary
}

int insertCard(){
    FILE *fp;
    box(40, 7, 72, "");
    printToxy(48,6,"INSERT YOUR CARD");
    usbAnimation();


    do{
        ctr = (ctr + 1) % 2;              // to open drive with different drive letter
        fp = fopen(checkDrives[ctr],"w"); // create file for drive checking
        usbAnimation;
    }while(fp == NULL);
    fclose(fp);

        dontRemove(0);
        printToxy(42,9,"                             ");
        printToxy(43,9,"P R E S S   A N Y   K E Y");
        getch();
        return 1;
}

int checkFd(){  // check if usb is connected
    FILE *fp;
    fp = fopen(checkDrives[ctr],"r");
    fclose(fp);
    if(fp == NULL)
        return 1;
    return 0;
}

void retrieve(){  // retrieve voter's information
    FILE *fp;
    DATA vtrs;
    if ((fp=fopen(drives[ctr],"r+"))!=NULL){
    while(!feof(fp)){
            fscanf(fp,"%d %[^\t] %[^\t] %s\n",&vtrs.idNumber,vtrs.name,vtrs.yrCourse,vtrs.pin);
            insert (vtrs);
        }
    }
    fclose(fp);
}

int isFull(){
    return(l.rlast == MAX);
}

void insert(DATA vtrs){                      // insert voters' info to database
success=0;
    if(checkFd()){                           // check if usb is connected before saving
        clear("result");
        printToxy(43,22,"F L A S H   D R I V E   E R R O R");
        dontRemove(1);
        gotoxy(0,25);
        exit(0);
    } else if (isFull()) {                   // check if database is full
           clear("result");
            printToxy(43,22,"D A T A B A S E   I S   F U L L");
            exit (0);
    } else if (locateName(vtrs. name)>= 0){  // check for a duplicate name
            clear("result");
            gotoxy (45, 22);
            printf("[ Duplicate record for %s . ]", vtrs.name);
            getch();
            success=1;
    } else if (locateId(vtrs. idNumber)>= 0){ // check for a duplicate student Id
            clear("result");
            gotoxy (31, 22);
            printf("[ STUDENT ID: %d WAS ALREADY IN USE. Recheck your ID ]", vtrs.idNumber);
            getch();
            success=1;
    } else {
        l.rlast++;
        l.stud[l.rlast] = vtrs;               // insert data
        saveToCard();
    }
}

int locateName (char nameloc[31]){
     for (int i = 0; i<= l.rlast; i++)
         if (strcmp(l.stud[i].name, nameloc)==0)
                return (i);
    return(-1);
}

int locateId (int Idloc){
    for(int i = 0; i <=l.rlast; i++){
            if(l.stud[i].idNumber == Idloc)
                return (i);
    }
    return(-1);
}

void saveToCard(){                       // save voters' info to file
    FILE *fp;
    fp = fopen(drives[ctr],"w");
    for(int i=0;i<=l.rlast;i++)
    {
        fprintf(fp,"%d\t%s\t%s\t%s\n",l.stud[i].idNumber,l.stud[i].name,l.stud[i].yrCourse,l.stud[i].pin);
    }
    fclose(fp);
}

void formLayout(){
    printToxy(45, 1,": : : REGISTRATION FORM : : :");
    printToxy(33, 3,"NOTE: Kindly remember your Student ID and PIN number");
    box(30, 4, 90, "Full Name:");
    box(30, 8, 59, "Student ID (4-digit):");
    box(61, 8, 90, "Year/Course:");
    box(40, 12, 80, "Nominate PIN:");
    box(40, 16, 80, "Confirm PIN:");
    box(30, 20, 90, " ");
    printToxy(57,22,"- - -");
}

void votingUI (){
    p= locateId(IdNum);
    system ("cls");
    printToxy (45, 4, ": : : C A S T   A   V O T E : : : ");
    box (30,5,49, "Student ID: ");
    gotoxy (43,7);
    printf(" %d", IdNum);
    box (50, 5, 90, "Full Name: ");
    gotoxy(62,7);
    printf(" %s", l.stud[p].name);
    box(20,9,100, "                              C A N D I D A T E S ");
     for(int i = 20; i < 100; i++){
        printToxy(i,12,"_");
        printToxy(i,19,"_");
    }
    for(int i = 13; i < 20; i++){
        printToxy(19,i,"|");
        printToxy(45,i,"|");
        printToxy(74,i,"|");
        printToxy(100,i,"|");
    }
    printToxy(28, 14, "PRESIDENT");
    printToxy(53, 14, "VICE PRESIDENT");
    printToxy(83, 14, "SECRETARY");

    printToxy(24, 16, "[1] BING, CHANDLER \n\t\t\t[2] GELLER, MONICA");
    printToxy(51, 16, "[1] BUFFAY, PHOEBE \n\t\t\t\t\t\t   [2] TRIBBIANI, JOEY");
    printToxy(80, 16, "[1] GELLER, ROSS   \n\t\t\t\t\t\t\t\t\t\t[2] GREENE, RACHEL");

    printToxy(24,18,"Enter vote: ");
    printToxy(51,18,"Enter vote: ");
    printToxy(80,18,"Enter vote: ");

    box (20,20, 100, " ");
    printToxy( 21,22, "Choose only 1 candidate per position. Enter their corresponding number [1 OR 2]");

    voting ();

}

void voting (){
REC vr;
vr.stID= IdNum;
voteCount(2);        // read voteCount file and save the value to vote counter (vcount)

    do {
        printToxy(37,18,"  ");       // vote for president
        gotoxy (37, 18);
        scanf ("%d",&vr.pr );
        if (vr.pr==1) {
             vcount[0]++;
        } else {
             vcount[1]++;
        }
    } while (vr.pr <=0 || vr.pr >2);  // do while number picked is not 1 or 2

    do {
        printToxy(64,18,"  ");         // vote for vice president
        gotoxy (64, 18);
        scanf ("%d",&vr.vp );
        if (vr.vp==1) {
            vcount[2]++;
        } else {
            vcount[3]++;
        }
    } while (vr.vp <=0 || vr.vp>2);

    do {
        printToxy(93,18,"  ");         // vote for vice secretary
        gotoxy (93, 18);
        scanf ("%d",&vr.sec );
        if (vr.sec==1) {
            vcount[4]++;
        } else {
            vcount[5]++;
        }
    } while (vr.sec <=0 || vr.sec>2);


    do {
         clear("VtMssgBox");
         printToxy(35,22, " [1] SUBMIT    [2] REDO VOTING     CHOICE: ");   // user will have a choice to submit vote or redo for instance it made a mistake
         scanf (" %d",&choice);
    } while (choice <= 0 || choice > 2 );

    if ( choice == 1){        // call votingUi to vote again
         voteCount(1);        // update vote count
        insertVote (vr);      // add the votes to the database
        clear("VtMssgBox");
        printToxy(35,22, "    VOTE SUBMITTED! THANK YOU FOR VOTING. ");
    } else if (choice ==2) {
        votingUI ();
    }
}

void voteCount(int x){
FILE *fp;
    if (x==1){                     // write the vote counts to file
     fp = fopen("Vcounts.dbf","w");
    for( int i= 0; i < 6; i++){
        fprintf (fp, "%d ", vcount[i]);
    }
    }else if ( x==2){
      fp = fopen("Vcounts.dbf","r+");   // read vote counts
       for( int i= 0; i < 6; i++)
            fscanf(fp, " %d", &vcount[i]);

    }
}

int locateVtr (int voter){         // check the recorded votes to confirm if a user has voted or not
for(int i = 0; i <=l.rlast; i++){
            if(l.candidates[i].stID == voter)
                return (i);
    }
    return(-1);
}

void viewVotes () {               // view the your vote summary, tally of votes and check the leading candidates after voting
    getch();
    system("cls");
         for(int i = 30; i < 90; i++){
        printToxy(i,4,"_");
        printToxy(i,19,"_");
    }
    for(int i = 5; i < 20; i++){
        printToxy(29,i,"|");
        printToxy(90,i,"|");
    }
        printToxy(48, 9, " [1] VIEW YOUR VOTE SUMMARY\n\n\t\t\t\t\t\t [2] DISPLAY VOTE COUNT\n\n\t\t\t\t\t\t [3] EXIT ");
        box(40,20,80," ");
        printToxy(50,22,"Select choice (1-3): ");
        scanf( "%d", &choice);

    switch (choice){
      case 1:
          system("cls");
          borders();
          printToxy(37,7," Y O U R   V O T E   S U M M A R Y" );
          gotoxy(45,10);
          printf("PRESIDENT: %s", president[l.candidates[l.vlast].pr-1]);
          gotoxy(40,12);
          printf("VICE PRESIDENT: %s", vpres[l.candidates[l.vlast].vp-1]);
          gotoxy(45,14);
          printf("SECRETARY: %s", secretary[l.candidates[l.vlast].sec-1]);
          viewVotes ();
          break;
     case 2:
          system("cls");
          borders();
          printToxy(47,7,"V O T E   C O U N T " );
          printToxy(16,10,"           PRESIDENT            VICE PRESIDENT              SECRETARY" );
          gotoxy(19,12);
          printf("    BING, CHANDLER = %d       BUFFAY, PHOEBE  = %d       GELLER, ROSS   = %d\n", vcount[0], vcount[2], vcount[4]);
          gotoxy(19,13);
          printf("    GELLER, MONICA = %d       TRIBBIANI, JOEY = %d       GREENE, RACHEL = %d\n", vcount[1], vcount[3], vcount[5]);
          printToxy(44,17,"===== L E A D I N G =====" );
          if (vcount[0] > vcount[1]){
              printToxy (23,19,"BING, CHANDLER");
              } else if (vcount[0] == vcount[1]) {
                   printToxy (25,19,"TIED");
                } else {
                    printToxy (23,19,"GELLER, MONICA");
                    }
          if (vcount[2]> vcount[3]){
              printToxy (50,19,"BUFFAY, PHOEBE ");
              } else if (vcount[2] == vcount[3]) {
                   printToxy (53,19,"TIED");
                } else {
                    printToxy (50,19,"TRIBBIANI, JOEY");
                    }
          if (vcount[4]> vcount[5]){
              printToxy (75,19,"GELLER, ROSS ");
              } else if (vcount[4] == vcount[5]) {
                   printToxy (79,19,"TIED");
                } else {
                    printToxy (75,19,"GREENE, RACHEL");
                    }
          viewVotes ();
          break;

     case 3:
          printToxy(0, 25,"VOTING SYSTEM PROJECT\nSubmitted by: MORANO, Janah Patricia and DALUMPINES, Sophia\nSubmitted  to: Prof. Fernando Renagado\nBSCS 2AB ");
          remove(checkDrives[ctr]);
          exit (0);
          break;

    }

}

void insertVote (REC vt){   // insert your votes to database
int i;
    if(checkFd()){
        clear("VtMssgBox");
        printToxy(43,22,"F L A S H   D R I V E   E R R O R");
        dontRemove(1);
        gotoxy(0,25);
        exit(0);
    } else {
        l.vlast++;
        l.candidates[l.vlast]= vt;
    }

    saveSummary ();

}

void saveSummary() {       // save your vote summary to file
    FILE *fp;
     fp = fopen("voteSummary.dbf","w");
    for(int i=0; i<=l.vlast; i++)
    {
        fprintf(fp,"%d\t%d\t%d\t%d\n",l.candidates[i].stID,l.candidates[i].pr,l.candidates[i].vp,l.candidates[i].sec);
    }
    fclose(fp);
}

void retrieveSummary(){   // retrieve votes from the file
    FILE *fp;
    REC vr;
    if ((fp=fopen("voteSummary.dbf","r+"))!=NULL){
    while(!feof(fp)){
            fscanf(fp,"%d %d %d %d \n",&vr.stID,&vr.pr,&vr.vp,&vr.sec);
            insertVote(vr);
        }
    }
    fclose(fp);
}


void delay(int ms){
    clock_t timeDelay = ms + clock();
    while(timeDelay > clock());
}

void usbAnimation(){
    char loadingCharacters[] = {'.','.','.','o','.','.','0','o','.','0','0','o','0','0','0','o','0','0','.','o','0','.','.','o','.','.','.','\0'};
    int x = 0, y = 1, z = 2, loop = 0;
    while(loop != 9){
        if(loop > 0){
            x += 3;
            y += 3;
            z += 3;
        }
        delay(130);
        printToxy(43,9,"         ");
        printf("%c   %c   %c",loadingCharacters[x],loadingCharacters[y],loadingCharacters[z]);
        gotoxy(72,10);
        loop++;
    }
}

void gotoxy(int x, int y){
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}

void printToxy(int x, int y, char* text){
    gotoxy(x,y);
    printf("%s",text);
}

void box(int x, int y, int len, char *text){
    int i;
    for(i = x; i < len; i++){
        printToxy(i,y,"_");
        printToxy(i,y + 3,"_");
    }
    for(i = y + 1; i < y + 4; i++){
        printToxy(x - 1,i,"|");
        printToxy(len,i,"|");
    }
    printToxy(x + 1, y + 2,text);
}

void clear(char *text){
    if(strcmp(text,"result") == 0){
        printToxy(32,21,"                                                       ");
        printToxy(32,22,"                                                       ");
        printToxy(57,22,"- - -");
    }
    else if(strcmp(text,"pinBoxes") == 0){
        printToxy(55,18,"                 ");
        printToxy(55, 14,"                 ");
    } else if (strcmp (text, "IdBox")== 0){
        printToxy (53, 10, "      ");
     } else if (strcmp( text, "vMssgBox")== 0){
        printToxy(35,17, "                                                 ");
        printToxy(57,17,"- - -");
      } else if (strcmp( text, "vPinBox")== 0){
          printToxy(58,12, "                ");
       } else if (strcmp( text, "VtMssgBox")== 0){
          printToxy(21,22, "                                                                               ");
        }

}

void dontRemove(int error){
    int i = 0, x, y, counter;
    if(error == 1){
        system("color 04");
        for(int i = 11; i < 19; i++)
            printToxy(44,i,"                                ");
        for(int i = 44; i < 76; i++){
            printToxy(i,10,"_");
            printToxy(i, 18,"_");
        }
        for(int i = 11; i < 19; i++){
            printToxy(43,i,"|");
            printToxy(76,i,"|");
        }
        x = 49;
        y = 11;
        counter = 4;
    }
    else if(error == 0){
        x = 46;
        y = 12;
        counter = 2;
        printToxy(48, 19,"! W A R N I N G !");
        printToxy(39, 20,"[ do not remove card when in use ]");
    }
    do{
        for(int j = y; j < y + 7; j++)
            printToxy(x - 1,j,"                          ");
        printToxy(x,y + 1,"_   ____________");
        printToxy(x,y + 2," |_|    ___ _   |");
        printToxy(x,y + 3," | |   [___|_|  |");
        printToxy(x,y + 4," |_|            |");
        printToxy(x,y + 5,"_| |____________|");
        delay(580);
        for(int j = y; j < y + 1; j++)
            printToxy(x - 1,j,"                           ");
        printToxy(x,y + 1,"_        _____________");
        printToxy(x,y + 2," |   ___|    ___ _    |");
        printToxy(x,y + 3," |  |[] |   [___|_|   |");
        printToxy(x,y + 4," |  |[]_|             |");
        printToxy(x,y + 5,"_|      |_____________|");
        delay(580);
        i++;
    }while(i < counter);
}

void borders(){
     printToxy(33,2,": : : T U P   V O T I N G   S Y S T E M : : : ");
    //edges
    gotoxy(14,0);
    printf("%c",201);
    gotoxy(96,0);
    printf("%c",187);
    gotoxy(14,24);
    printf("%c",200);
    gotoxy(96,24);
    printf("%c",188);

    //TOP LINE
    for(int i = 15; i < 96; i++){
        gotoxy(i, 0);
        printf("%c",205);
    }
    //header
    for(int i = 15; i < 96; i++){
        gotoxy(i, 4);
        printf("%c",205);
    }
    //BOTTOM LINE
    for(int i = 15; i < 96; i++)
    {
        gotoxy(i, 24);
        printf("%c",205);
    }
    //LEFT LINE
    for(int i = 1; i < 24; i++)
    {
        gotoxy(14, i);
        printf("%c",186);
    }
    //RIGHT LINE
    for(int i = 1; i < 24; i++)
    {
        gotoxy(96, i);
        printf("%c",186);
    }
}

