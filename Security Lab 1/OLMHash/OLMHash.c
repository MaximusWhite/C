#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#define true 1
#define false 0

//checks if the password contains restricted characters

int valid_password(char* pass){

   int i;
   for (i=0;i<strlen(pass)-1;i++){

     if (pass[i]<48 || (pass[i]>57 && pass[i]<65) || (pass[i]>90 && pass[i]<97) || pass[i]>122) return false;

   }

   return true;

}

// checks if the id was previously saved in the table

int savedID(FILE* tbl, char* id, char* tbl_entry){

   char* entry = (char*)malloc(sizeof(char) * 48);
   char* tmp_id = (char*)malloc(sizeof(char)*34);
   memset(entry,'\0',48);

   //reading table line by line

   while(fgets(entry, sizeof(char)*48, tbl)){

      //extract id from entry

      char* separator = strchr(entry,'|');
      memset(tmp_id,'\0',34);
      strncpy(tmp_id, entry, separator-entry);

      //if entered id matches the one read from table, return true

      if (strcmp(id, tmp_id)==0) {

        strncpy(tbl_entry, entry, strlen(entry));
 	return true;
      }

   }

   //else - false
   return false;
}


//updating an entry with a new password

FILE* update_entry(FILE* table,char* id, char* entry){

     char* tmp_id = (char*)malloc(sizeof(char)*34);

     //creating another file to store the table with updated entry

     FILE* table2 = fopen("table2","w+");
     char* str = (char*)malloc(sizeof(char)*48);
     rewind(table);

     //read table line by line

     while(fgets(str, sizeof(char)*48, table)){

         //extract id

         memset(tmp_id,'\0',34);
         char* separator = strchr(str,'|');
         if (separator==NULL) break;
         memcpy(tmp_id, str, separator-str+1);

    //if give id and id from the table entry don't match, copy entry into another table

	if(strcmp(tmp_id,id)!=0) {

            fprintf(table2,"%s",str);

        }

     }

     //close the original table and delete it

     fclose(table);
     remove("table");

     //put updated entry into new table

     fprintf(table2,"%s\n",entry);

     //close new table and rename it

     fclose(table2);
     rename("table2", "table");

     //reopen new table as if it was still older one

     table2 = fopen("table","a+");

	return table2;

}

// putting new entry into table

void new_pass(FILE* table, char* str,char* id, char* password){


            memcpy(password,str,12);
            password[strlen(password)]='\n';
            fprintf(table, "%s%s", id, password);


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

   //transferring password to upper case

   int i;
   for(i=0; i<strlen(pass);i++){

        pass[i]= toupper(pass[i]);

   }

   //change newline character to null-character

   char* nl_ptr = strchr(pass,'\n');
   nl_ptr[0]='\0';

   //copying three chunks of password

   memcpy(firstin,pass,4);
   memcpy(secondin,pass+4,4);
   memcpy(thirdin,pass+8,4);

   //applying hash function to all chunks

   E(firstin,firstout);
   E(secondin,secondout);
   E(thirdin,thirdout);

   //copying result back into password

   memcpy(pass,firstout,4);
   memcpy(pass+4,secondout, 4);
   memcpy(pass+8,thirdout, 4);


}


int main(int args, char* argv[]){

  FILE* table;
  table = fopen("table","a+");

  int saved_id;

  int trials=3;

  char* id = (char*)malloc(sizeof(char) * 34);

  char* tmp = (char*)malloc(sizeof(char) * 34);

  char* password = (char*)malloc(sizeof(char) * 13);
  char* pass_table = (char*)malloc(sizeof(char) * 13);

  char* str = (char*)malloc(sizeof(char)* 256);

  char* table_entry = (char*)malloc(sizeof(char) * 48);

  memset(id,'\0',34);
  memset(password,'\0',13);
  memset(pass_table,'\0',13);
  memset(table_entry,'\0',48);

  //asking for ID

  while(1){
  printf("Enter your ID: ");
  fgets(str, sizeof(char) * 256 , stdin);

  	if (strlen(str)-1>3 && strlen(str)-1<33){


            break;

  	}
       printf("\nERROR: ID length must be 4-32 characters long. Try again\n\n");

  }

  //copying id and checking if it was saved in the table before

  memcpy(id,str,strlen(str)-1);

  saved_id = savedID(table,id,table_entry);

  id[strlen(str)-1]='|';

  //if id wasn't saved before, create new entry

  if(saved_id == false){
  	while(1){
   	    printf("Enter NEW password: ");
	    memset(str,'\0',256);
            fgets(str, sizeof(char) * 256 , stdin);
            if (strlen(str)-1<=12){

            //if password is properly formatted, hash it and write new entry to table

	        if(valid_password(str)==true) {
                hash_pass(str);   // HASHING
                new_pass(table,str,id,password);
 	        break;
 	        } else {
            	   printf("\nERROR: No special characters allowed. Try again\n\n");
                   continue;
	          }
             }
        printf("\nERROR: Password length must be not more than 12 characters long. Try again\n\n");
  	}

  	// if id found in the table, check password

  }else{

      //if trial not used completely

      while(trials>0){

        printf("Enter your PASSWORD: ");
        memset(str,'\0',256);
        fgets(str, sizeof(char) * 256 , stdin);

        if (strlen(str)-1<=12){

            //if password formatted properly, hash it

            if(valid_password(str)==true) {

            memset(password,'\0',13);

            hash_pass(str);   // HASHING

            memcpy(password,str,strlen(str));

            //extract password from the entry according to given id

	        char* separator = strchr(table_entry,'|');
            memcpy(pass_table,separator+1,strlen(table_entry)-1-(separator+1-table_entry));

              //if hashes of given password and password from the table match, inform user and ask for new password

              if(strcmp(password,pass_table)==0){  // password match

              printf("CORRECT PASSWORD\n");

              //handle new password as with the new user addition

	          while(1){
                      printf("Enter NEW password: ");
                      memset(str,'\0',256);
                      fgets(str, sizeof(char) * 256 , stdin);
                      if (strlen(str)-1<=12){

                         if(valid_password(str)==true) {

                           hash_pass(str);  // HASHING

                           memset(separator+1,'\0',12);
                           memcpy(separator+1,str, 12);

                          break;
                         } else {
                            printf("\nERROR: No special characters allowed. Try again\n\n");

                            continue;
                           }
                     }
                   printf("\nERROR: Password length must be not more than 12 characters long. Try again\n\n");
                   }

                //update table with new entry

                table = update_entry(table,id,table_entry);

             	break;

        //if passwords do not match, inform user and ask for password again; update trials

		}else{
			trials--;
			if(trials==0) {
				printf("\nIncorrect password. You have no more trials. System is locked.\n\n");
				exit(0);
			}
			printf("\nIncorrect password. You have %d more trial(s)\n\n", trials);

			continue;
		     }

            }else {
			printf("\nERROR: No special characters allowed. Try again\n\n"); continue;
		  }
        }else{
        	printf("\nERROR: Password length must not more than be 12 characters long. Try again\n\n");
	     }

	}
  }

  fclose(table);
  return 1;
}
