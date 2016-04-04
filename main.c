#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> //open,create
#include <sys/types.h> //open
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>

const int NUMBER_OF_STRINGS = 30;
const int MAX_STRING_LENGTH = 30;
const int NEWLINE_I = 10;
char* NEWLINE_C = "\n";

int get_command(const char* command){
    if(strcmp(command, "adddest") == 0){
        return 0;
    } else if(strcmp(command, "rmdest") == 0){
        return 1;
    } else if(strcmp(command, "lsdest") == 0) {
        return 2;
    } else if(strcmp(command, "signup") == 0) {
        return 3;
    } else if(strcmp(command, "details") == 0){
        return 4;
    } else {
        return -1;
    }
}

void remove_destination(char* dest){
    printf("remove_destination\n");
    int f = open("destination",O_RDWR);
    if (f < 0){
        perror("Error at opening the file\n");
        exit(1);
    }
    char* buffer[NUMBER_OF_STRINGS];
    int x;
    for(x = 0; x < NUMBER_OF_STRINGS; x++){
        buffer[x] = (char*)malloc(MAX_STRING_LENGTH*sizeof(char));
    }
    int i = 0;
    char c;
    while (read(f, &c, sizeof(c))){
        strncat(buffer[i],&c,1);
        if(c == NEWLINE_I){
            i++;
        }
    }
    close(f);
    int tmp = open("tmp",O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR);
    if (tmp < 0){
        perror("Error at opening the file\n");
        exit(1);
    }
    int j;
    strncat(dest,NEWLINE_C,1);
    for(j = 0; j < i; j++){
        if(strcmp(buffer[j],dest) != 0){
            write(tmp, buffer[j], sizeof(buffer[j]));
        }
    }
    system("rm destination");
    system("cp tmp destination");
    close(tmp);
    system("rm tmp");
}

void add_destination(char* dest){
    printf("add_destination\n");
    int f = open("destination",O_RDWR);
    if (f < 0){
        perror("Error at opening the file\n");
        exit(1);
    }
    char c;
    while(true){
        if(read(f, &c, sizeof(c)) == 0){
            strncat(dest,NEWLINE_C,1);
            write(f, dest,sizeof(dest));
            break;
        }
    }
    close(f);

}

void list_destinations(){
    printf("list_destinations\n");
    int f = open("destination",O_RDONLY);
    if (f < 0){
        perror("Error at opening the file\n");
        exit(1);
    }
    char c;
    while (read(f, &c, sizeof(c))){
        printf("%c",c);
    }
    close(f);
}

bool validateDest(char* dest){
    printf("validating...\n");
    bool l = false;
    int f = open("destination",O_RDONLY);
    if (f < 0){
        perror("Error at opening the file\n");
        exit(1);
    }
    char* d2 = malloc(1);
    sprintf(d2, "%s\n",dest);
    char* buffer = malloc(1);
    char c;
    while (read(f, &c, sizeof(c))){
        sprintf(buffer, "%s%c",buffer,c);
        if(c == NEWLINE_I){
            if(strcmp(buffer, d2) == 0){
                l = true;
            }
            strcpy(buffer,"");
        }
    }
    free(buffer);
    free(d2);
    close(f);
    return l;
}

void appendToFile(char* file,char* text){
    printf("appendToFile\n");
    int f = open(file,O_RDWR|O_CREAT,S_IRUSR|S_IWUSR);
    if (f < 0){
        perror("Error at opening the file\n");
        exit(1);
    }
    char c;
    while(true){
        if(read(f, &c, sizeof(c)) == 0){
            write(f, text,strlen(text));
            break;
        }
    }
    close(f);
}

void signup(char ** data){
    printf("signup\n");
    char* fullTime = malloc(sizeof(char));
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(fullTime, "%d-%d-%d %d:%d:%d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    /*
     data[0]: ./a.out
     data[1]: kapcsoló
     data[2]: Vezetéknév
     data[3]: Keresztnév
     data[4]: Célállomás
     data[5]: Telefon
     */
    char* lastName = data[2];
    char* firstName = data[3];
    char* dest = data[4];
    char* phone = data[5];
    if(validateDest(dest)){
        printf("CELALLOMAS OK\n");
        char* p = "passengers_";
        int size = (int)strlen(p) + (int)strlen(dest);
        char* def = (char*)malloc(size);
        sprintf(def,"%s%s",p,dest);
        char fullString[sizeof(lastName)+sizeof(firstName)+sizeof(dest)+sizeof(phone)+sizeof(fullTime)+1];
        sprintf(fullString,"%s %s %s %s %s\n",lastName, firstName, dest, phone, fullTime);
        printf("%s",fullString);
        appendToFile(def, fullString);
        free(def);
    }else{
        printf("Nincs ilyen celallomas: %s\n",dest);
    }
    free(fullTime);
}

void details(char *dest) {
    char* p = "passengers_";
    int size = (int)strlen(p) + (int)strlen(dest);
    char* def = (char*)malloc(size);
    sprintf(def,"%s%s",p,dest);

    int f = open(def, O_RDONLY);
    if (f < 0){
        //perror("Error at opening the file\n");
        printf("Nincs adat ehhez a jarathoz\n");
        exit(1);
    }
    char c;
    while (read(f, &c, sizeof(c))){
        printf("%c",c);
    }
    close(f);


}

int main(int argc,char ** argv)
{
    //printf("Number of command line arguments are: %i\n",argc);
    switch (get_command(argv[1])){
        case 0:
            if(argc != 3){
                printf("Too few arguments!\n");
            } else {
                add_destination(argv[2]);
            }
            break;
        case 1:
            if(argc != 3){
                printf("Too few arguments!\n");
            } else {
                remove_destination(argv[2]);
            }
            break;
        case 2:
            if(argc != 2){
                printf("Too few arguments!\n");
            } else {
                list_destinations();
            }
            break;
        case 3:
            if(argc != 6){
                printf("Too few arguments!\n");
            } else {
                signup(argv);
            }
            break;
        case 4:
            if(argc != 3){
                printf("Too few arguments!\n");
            } else {
                details(argv[2]);
            }
            break;
        case -1:
            printf("Invalid argument!\n");
            break;
    }
    return 0;
}

