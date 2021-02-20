# Database File

This project basically lets the user make a big file that contains others in it.

It was made to understand better the C sys-call functions that open create read 
and write a file ass well as manipulate it. (open, close, read, write, lseek)  <br />

You just need to compile and run the binary file. <br />

In order to run you need to provide an argument that will be the database's name (
file). Be careful if you provide the name of an already existing file the program
will check if that's an already existing database file if it is it will continue using
that as an input otherwise it will exit giving an error message.
If it's a new file it will continue writing the new file to the filesystem.

Once you run it, you will be presented with the menu : <br />
    1.import(i) <br />
    2.find(f) <br />
    3.export(e) <br />
    4.delete(d) <br />
    5.quit(q) <br />
    Select an option: <br />

It is self-explanatory, you just need to provide the option and then you provide the 
file's name you want to import find export or delete from the database.

In case you want to list the files the database inside you need to write * under the 
find option from the menu.