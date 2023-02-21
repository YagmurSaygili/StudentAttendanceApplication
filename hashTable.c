#include<stdio.h>
#include<string.h>
#include <stdlib.h>

/*
Creator: Yağmur Saygılı / School Assigment : Algorithms

    Intermediate C approach with using hash tables: open addressing
    to design Student Attendance System for schools

    Avaliable operations:
        1) Add student
        2) Search Student
        3) Print Student

    There is also an option for quadratic probing or double hashing before the
    system starts to design hash mapping. System asks user which way of hashing
    wanted then calculate accordingly.

*/
 struct StudentInfo{
    char studentId[4];
    char departmentCode[4];
    char name[40];

};

int tableSize = 11;

float loadfactor(int );
struct StudentInfo* rehash(struct StudentInfo *,int ,int );

int main(){

    struct StudentInfo *hashTable;

    hashTable=(struct StudentInfo *)calloc(tableSize,sizeof(struct StudentInfo));




    for(int i =0;i<tableSize;i++)
    {
        strcpy((hashTable+i)->name,"");
        strcpy((hashTable+i)->departmentCode,"");
        strcpy((hashTable+i)->studentId,"");

    }
    int hashChoice;
    printf("Choose open addressing technique to be used:(1) double hashing (2) quadratic probing\n");
    scanf(" %d", &hashChoice);


    ///menu
    int option,uniquenessCheck=0,index;
    int elementsAdded=0;//This variable counts the added structures to calculate load factor


    do{
            printf("1. Add a student.\n2. Search for a student using ID\n3. Print table");
            printf("\nPlease select an option: ");
            scanf(" %d", &option);
            if (option == 1) // Adding a student
            {
                uniquenessCheck = addingStudent(hashTable,hashChoice);//Here I basically check if the Id is already added to the table or not with returnin value

                if (uniquenessCheck == -1)
                    printf("Id should be unique !\n");
                else
                    elementsAdded++;


                //printf("\nThe load factor is %.2f",loadfactor(elementsAdded));
                if ( loadfactor(elementsAdded)>0.5 )
                {
                    hashTable=rehash(hashTable,hashChoice,elementsAdded);
                    printf("\nTable is rehashed!\n");
                }
            }
            else if (option == 2)
            {
                char uniqueId[4];
                printf("Enter unique identifier: ");
                scanf(" %s", &uniqueId);


                index = search(uniqueId,hashChoice,hashTable);

                if (index == -1)
                    printf("Student is not found\n\n");
                else
                    printf("Name: %s\nDepartment: %s\n\n",(hashTable+index)->name,(hashTable+index)->departmentCode);
            }
            else if (option == 3)
            {
                printHashTable(hashTable);
            }
            else if (option != 4)
            {
                printf("Invalid option!\n");
            }




    }while(  option!= 4 );


    free(hashTable);

return 0;
}

struct StudentInfo* rehash(struct StudentInfo *hashTable,int hashChoice,int numberOfElements)
{
    tableSize = tableSize * 2;
    int index;

    struct StudentInfo *rehashedTable = (struct StudentInfo *)calloc(tableSize,sizeof(struct StudentInfo));

    for(int i =0;i<tableSize;i++)//Setting initial values to the rehashed table
    {
        strcpy((rehashedTable+i)->name,"");
        strcpy((rehashedTable+i)->departmentCode,"");
        strcpy((rehashedTable+i)->studentId,"");

    }

    for (int i =0; i<tableSize/2;i++)//I am dividing tableSize by 2 here because I already multiplied by two
    {
        if (hashChoice == 1)
        {
            if ( strcmp( (hashTable+i)->name,"") != 0 )
            {
                index=doubleHashing(*(hashTable+i),rehashedTable);//Getting the index for new table
                *(rehashedTable+index) = *(hashTable+i);//Adding to the new table
            }

        }
        else
            if ( strcmp( (hashTable+i)->name,"") != 0 )
            {
                index=quadraticProbing(*(hashTable+i),rehashedTable);
                *(rehashedTable+index) = *(hashTable+i);
            }
    }

    return rehashedTable;
}

float  loadfactor(int numberOfElements)//This function calculates the load factor and returns it
{

    return (numberOfElements*1.0)/(tableSize*1.0);

}

void printHashTable(struct StudentInfo *array)
{
    printf("\nIndex\tID\tName\t\tDepartment\n");
    for(int i=0;i<tableSize;i++)
    {
       printf("%d\t%s\t%s\t  %s\n",i,(array+i)->studentId,(array+i)->name,(array+i)->departmentCode);
    }
}

int hash1(int key)//Calculates the hash1 function
{
    return (2*key)%tableSize;
}

int keyCalculator(struct StudentInfo student)//Calculates the key of the content
{
    int key,firstDigit,secondDigit,thirdDigit;

    firstDigit=student.studentId[0] - 'A';
    secondDigit = student.studentId[1] - '0';
    thirdDigit = student.studentId[2] - '0';

    key = (  firstDigit+ secondDigit + thirdDigit );

    return key;
}


int quadraticProbing(struct StudentInfo student,struct StudentInfo *array)//Doing the quadraticProbing and return the index if the value is not already there
{
    int key,f,resultingIndex;

    char studentId[4];

    int firstDigit,secondDigit,thirdDigit;//To convert characters to numbers

    firstDigit=student.studentId[0] - 'A';
    secondDigit = student.studentId[1] - '0';
    thirdDigit = student.studentId[2] - '0';

    int i=-1;//Will be incremented with starting of loop so it will start from 0
    key = firstDigit + secondDigit + thirdDigit;

    do {
        i++;

        f = i*i;

        resultingIndex = (hash1(key)+f)%tableSize;


        if ( strcmp((array+resultingIndex)->studentId,student.studentId) == 0 )// checking if the ID is unique , if it is not it will return -1 and user goes back to functionalities menu
        {
            return -1;
        }

    }while( ( strcmp((array+resultingIndex)->name,"") != 0 ) );

    return resultingIndex;

}


int doubleHashing(struct StudentInfo student,struct StudentInfo *array)///Doing the doubleHashing and return the index if the value is not already there
{
    int key,f,resultingIndex,i=-1; //to count the number of tryings

    key = keyCalculator(student);

    do{

    i++;

    f = i*(7- key%7);
    resultingIndex= (hash1(key)+f)%tableSize;

    if ( strcmp((array+resultingIndex)->studentId,student.studentId) == 0 )// checking if the ID is unique , if it is not it will return -1 and user goes back to functionalities menu
        {
            return -1;
        }
    }while ( strcmp((array+resultingIndex)->name,"") != 0 );

    //printf("Resulting index is %d",resultingIndex);
    return resultingIndex;


}

int addingStudent(struct StudentInfo *array,int hashChoice)///Adding student  with the return value of either DoubleHashing or QuadraticProbing function
{
    char Id[4],departmentCode[4],name[40];

    printf("Enter unique identifier: ");
    scanf(" %[^\n]s",&Id);
    printf("Enter name: ");
    scanf(" %[^\n]s",&name);
    printf("Enter department: ");
    scanf(" %s",&departmentCode);

    struct StudentInfo student;

    strcpy(student.name,name);
    strcpy(student.departmentCode,departmentCode);
    strcpy(student.studentId,Id);



    int indexToAdd;

    if(hashChoice==1)
        indexToAdd=doubleHashing(student,array);
    else
        indexToAdd=quadraticProbing(student,array);

    if (indexToAdd == -1)
        return -1;

    *(array+indexToAdd) = student;
    printf("%s has been registered\n",student.name);

    return 0;
}

int search(char *studentId,int hashChoice,struct StudentInfo *hashTable)
//This function basically checks if the student Id is in already in the list or not
{
    int key,f,searchIndex,i=-1;
    int firstDigit,secondDigit,thirdDigit;

    firstDigit = studentId[0] - 'A';
    secondDigit = studentId[1] - '0';
    thirdDigit = studentId[2] - '0';

    key = ( firstDigit + secondDigit + thirdDigit);


    if (hashChoice == 1) // That means we implement searching with double hashing
    {
        do{
            i++;

            f = i*(7- key%7);
           searchIndex= (hash1(key)+f)%tableSize;

            if ( strcmp( (hashTable+searchIndex)->studentId,studentId ) == 0 )
            {
                return searchIndex;
            }

        }while( strcmp((hashTable+searchIndex)->name,"") != 0 ); // To check if we reach an empty slot
    }
    else
    {
        do{
            i++;

             f = i*i;

            searchIndex = (hash1(key)+f)%tableSize;

            if ( strcmp( (hashTable+searchIndex)->studentId,studentId ) == 0 )
                return searchIndex;

        }while( strcmp((hashTable+searchIndex)->name,"") != 0 ); // To check if we reach an empty slot

    }

    return -1;// means not found

}
