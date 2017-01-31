To compile the program, use the following command (without quotes):
"gcc -o q3 q3.c"
This will produce an executable "q3".
In order to run the program, make sure that files "hashes" and "hash_table" are present in the same folder with executable. 
To run the program, use the following command (without quotes): 
"./q3 < hashes"
The reason of passing the "hashes" file to the program is that the user can not give the program an actual hash using a keyboard. Therefore file "hashes" contains some of the real hashes of the passwords generated to "hash_table", as well as a couple of entries that are made up.
When you run the program in a way described above, you will see how the program takes input from "hashes" line by line and either displays the original password if it's found or informs user that the password could not be found.
After that the program quits.

The functionality is shown on the screenshots in the report.