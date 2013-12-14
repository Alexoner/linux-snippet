#include <stdio.h>
#include <stdlib.h>

char *menu[]=
{
    "a - add new recore",
    "d = delete recore",
    "q - quit",
    NULL,
};

int getchoice(char *greet, char *choices[]);
int main()
{
    FILE *input;
    FILE *output;
    int choice = 0;

    if(!isatty(fileno(stdout)))
    {
        fprintf(stderr,"It's not a terminal\n");
    }
    input=fopen("/dev/tty","r");
    output=fopen("/dev/tty","w");
    if(!input||!output)
    {
        fprintf(stderr,"Unable to open /dev/ttyn");
        exit(1);
    }
    do
    {
        choice = getchoice("Please select an action", menu);
        fprintf(output,"You have chosen: %c\n", choice);
    } while(choice != 'q');
    exit(0);
}
int getchoice(char *greet, char *choices[])
{
    int chosen = 0;
    int selected;
    char **option;
    do {
        printf("Choice: %s\n",greet);
        option = choices;
        while(*option) {
        printf("%s\n",*option);
        option++;
        }
    selected = getchar();
    option = choices;
    while(*option) {
        if(selected == *option[0]) {
        chosen = 1;
        break;
    }
    option++;
    }
    if(!chosen) {
        printf("Incorrect choice, select again\n");
                }
    } while(!chosen);
    return selected;
}

