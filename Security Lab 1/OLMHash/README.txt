To compile the program simply use the following command:
"gcc -o q1 OLMHash.c"
This will produce an executable "q1".
Once you run the program, it will ask you to provide the ID.
When you provide the ID, the program will check if the ID is in the "table" file (which is created automatically if it doesn't exist yet, or simply updates it if it does).
If the ID was not found in the table, the program will ask for NEW password and will update the table with the new entry. After it, it will quit.
If the ID was found, the program will ask for the password.
If you provide the wrong password, the program will keep asking for a password, until you either enter correct one or the number of trials will be 0. If you use all trials, program will notify you and quit.
When you enter correct password, the program will ask you for a NEW password and will update the table with the entry with the updated password. After that the program quits.

The functionality is shown on the screenshots in the report.