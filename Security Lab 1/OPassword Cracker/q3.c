#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#define true 1
#define false 0


/********************* E function *************************/
// DES replacement cipher
// The function E takes 4 bytes from *in as input and
// writes 4 bytes to *out
void E(char *in, char *out) {
out[0]=(in[0]&0x80)^(((in[0]>>1)&0x7F)^((in[0])&0x7F));
out[1]=((in[1]&0x80)^((in[0]<<7)&0x80))^((( in[1]>>1)&0x7F)^((in[1])&0x7F));
out[2]=((in[2]&0x80)^((in[1]<<7)&0x80))^((( in[2]>>1)&0x7F)^((in[2])&0x7F));
out[3]=((in[3]&0x80)^((in[2]<<7)&0x80))^((( in[3]>>1)&0x7F)^((in[3])&0x7F));
}

//hash password using E function above for all 3 4-byte chunks of password

void hash_pass(char* pass){

   char* firstin = (char*)malloc(sizeof(char) * 4);

   char* firstout = (char*)malloc(sizeof(char) * 4);

   int i;
   for(i=0; i<strlen(pass);i++){

        pass[i]= toupper(pass[i]);

   }

   memcpy(firstin,pass,4);


   E(firstin,firstout);


   memcpy(pass,firstout,4);

}

//make a table of all possible 4-digit combinations consisting of Aa-Zz

void make_table(FILE* table){


	int i, j, k, l;
        char* entry = (char*)malloc(sizeof(char)*10);
        char* pass = (char*)malloc(sizeof(char)*4);

	for (i = 65; i<=90;i++){

		for (j = 65; j<=90;j++){

			for (k = 65; k<=90;k++){

				for (l = 65; l<=90; l++){

                    //go through ASCII codes for all possible combinations, hash them and put into a separate table

					sprintf(pass,"%c%c%c%c",i,j,k,l);

 					hash_pass(pass);

					fprintf(table,"%c%c%c%c|%s\n",i,j,k,l,pass);

				}
			}
		}
	}
	free(entry);
	free(pass);

}

//find the password corresponding to given hash (if such one exists in the table)

void find_pass(FILE* table, char* pass){

	char* entry = (char*)malloc(sizeof(char)* 256);

	char* table_pass = (char*)malloc(sizeof(char)* 5);

	char* hash = (char*)malloc(sizeof(char)* 5);

	rewind(table);

	//read file line by line

	while(fgets(entry, sizeof(char)*256, table)){

		char* separator = strchr(entry,'|');

		if (separator!= NULL){

			//for each each line read from table, extract password and it's hash

			memcpy(table_pass, entry, 4);
			memcpy(hash, separator+1, 4);

            //if provided hash and hash from the table match, display the password corresponding to hash

			if(memcmp(hash, pass, 4)==0){

				printf("Password: %s\n", table_pass);
				return;

			}

		}

	}


	printf("Password not found\n");


}

//main function

int main(int args, char* argv[]){

FILE* table;
table = fopen("hash_table","w+");

// make table of all possible 4-digit combinations

make_table(table);

char* str = (char*)malloc(sizeof(char)*256);

char* pass = (char*)malloc(sizeof(char)*6);

//while can read from stdin

while(fgets(str,sizeof(char)*256, stdin)){

//ask for hash

printf("Enter hash: %s", str);

memcpy(pass, str, 6);

//try to find it in the table
//the function will display if the password was found or not

find_pass(table, pass);

memset(str,'\0', 256);

}

close(table);

return 1;
}
