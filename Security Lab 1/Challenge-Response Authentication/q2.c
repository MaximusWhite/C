#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#define true 1
#define false 0

//checks if the password contains restricted characters

int valid_password(char* pass){

   int i;
   //for each character check if it belongs to a-z or A-Z range
   for (i=0;i<strlen(pass)-1;i++){

     if (pass[i]<48 || (pass[i]>57 && pass[i]<65) || (pass[i]>90 && pass[i]<97) || pass[i]>122) return false;

   }

   return true;

}

//returns true if userid exists in the table

int savedID(FILE* tbl, char* id, char* tbl_entry){

   char* entry = (char*)malloc(sizeof(char) * 48);
   char* tmp_id = (char*)malloc(sizeof(char)*34);
   memset(entry,'\0',48);
   rewind(tbl);

   //read file line by line

   while(fgets(entry, sizeof(char)*48, tbl)){

      char* separator = strchr(entry,'|');
      if(separator!=NULL){
      memset(tmp_id,'\0',34);
      strncpy(tmp_id, entry, separator-entry);

      //if extracted id matches the one given by user
      //save the entire entry to table_entry and return true

      memset(tbl_entry,48, '\0');
        if (strcmp(id, tmp_id)==0) {
            memcpy(tbl_entry, entry, strlen(entry));

            return true;

        }
      }

   }

   //otherwise not found

   return false;
}

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
   char* secondin = (char*)malloc(sizeof(char) * 4);
   char* thirdin = (char*)malloc(sizeof(char) * 4);

   char* firstout = (char*)malloc(sizeof(char) * 4);
   char* secondout = (char*)malloc(sizeof(char) * 4);
   char* thirdout = (char*)malloc(sizeof(char) * 4);


   int i;
   for(i=0; i<strlen(pass);i++){

        pass[i]= toupper(pass[i]);

   }
   int len = strlen(pass);

   char* nl_ptr = strchr(pass,'\n');
   nl_ptr[0]='\0';

   memcpy(firstin,pass,4);
   memcpy(secondin,pass+4,4);
   memcpy(thirdin,pass+8,4);

   E(firstin,firstout);
   E(secondin,secondout);
   E(thirdin,thirdout);

   memcpy(pass,firstout,4);
   memcpy(pass+4,secondout, 4);
   memcpy(pass+8,thirdout, 4);

}

//xor function for each 4-byte chunk of password
void R(char* in, char* out, int r)
{
    //xor the first byte with r's first byte

	out[3] = in[3] ^ (r & 255);

	//shift r by one byte

	r = r >> 8;
	out[2] = in[2] ^ (r & 255);
	r = r >> 8;
	out[1] = in[1] ^ (r & 255);
	r = r >> 8;
	out[0] = in[0] ^ (r & 255);
}

//returns the result of xor between r and hashed password

char * get_xor(char* hash, int r)
{
	char * out = (char*)calloc(12, sizeof(char));
	R(hash, out, r);
	R(&hash[4], &out[4], r);
	R(&hash[8], &out[8], r);
	return out;
}

// "client" view
//

char* provide_r(int r, char* id, char* pass){

    //hash password given by user

    hash_pass(pass);
    printf("\n**********\nUser hashed password to **%s**\n", pass);
    printf("User applied function, giving result to server\n**********\n");

    //return result of xor

    return get_xor(pass, r);

}


// main acts as a server

int main()
{

    time_t t;
    srand((unsigned) time(&t));

    char* response;

    char* id = (char*)malloc(sizeof(char) * 34);
    char* tmp = (char*)malloc(sizeof(char) * 34);

    char* password = (char*)malloc(sizeof(char) * 13);
    char* pass_table = (char*)malloc(sizeof(char) * 13);

    char* str = (char*)malloc(sizeof(char)* 256);

    char* table_entry = (char*)malloc(sizeof(char) * 48);

    int saved_id=false;

    FILE* table;
    table = fopen("table", "r");



getid:
  memset(id,'\0',34);
  memset(password,'\0',13);
  memset(pass_table,'\0',13);
  memset(table_entry,'\0',48);

  //asking for id

  while(1){

  printf("Enter your ID: ");
  memset(str, '\0', 256);
  fgets(str, sizeof(char) * 256 , stdin);

  	if (strlen(str)-1>3 && strlen(str)-1<33){


            break;

  	}
       printf("\nERROR: ID length must be 4-32 characters long. Try again\n\n");

  }

    //copy entered id to memory

    memset(id,'\0',34);

     memcpy(id,str,strlen(str)-1);

    char* separator;

    //ask for password, read it and save it

  	while(1){
   	    printf("Enter your password: ");
	    memset(str,'\0',256);
            fgets(str, sizeof(char) * 256 , stdin);
            if (strlen(str)-1<=12){

	        if(valid_password(str)==true) {
                    memcpy(password,str,12);

 	        break;
 	        } else {
            	   printf("\nERROR: No special characters allowed. Try again\n\n");
                   continue;
	          }
             }
        printf("\nERROR: Password length must be not more than 12 characters long. Try again\n\n");
  	}

    //check if id was saved in the table before

    saved_id = savedID(table,id,table_entry);


    //if id was saved before, extract password from the according table

    if(saved_id==true){
    separator = strchr(table_entry,'|');
    memcpy(pass_table,separator+1,strlen(table_entry)-1-(separator+1-table_entry));

    //if not - inform user, and go back to asking for id

    }else{
            printf("ID is not found\n");
            goto getid;
     }

    //create random 32-bit challenge

    int r=rand();

    printf("\n**********\nServer recieved id: %s\nGiving challange to user: %d\n**********\n",id, r);

    //provide user with challenge and save result of xor between hash and r

    response = provide_r(r, id, password);

    //calculate "true" xor, on the server's side

    char* server_result = get_xor(pass_table, r);


    printf("\n**********\nServer recieved user response\n**********\n");

    //if results of xor match, grant access, else deny access

    if(memcmp(response,server_result, 12)==0){

            printf("  ==============\n||ACCESS GRANTED||\n  ==============\n");

    }else {

            printf("  =============\n||ACCESS DENIED||\n  =============\n");

    }

    //ask if user wants to try anoter session and handle answer

ask:
    printf("Again?(Y/N): ");

    char answer = fgetc(stdin);
    if(answer == 'Y' || answer == 'y'){
            fgets(str, 256, stdin);
            goto getid;
    }else if(answer == 'N' || answer == 'n'){

         return(0);

    }else{
       printf("Enter 'Y' or 'N'\n");
       goto ask;

    }
    return 0;
}
