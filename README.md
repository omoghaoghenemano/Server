CMPE/ISYE/ITEC/MISY 431 Computer Network Design and Application Term Project Details (20211215)
Objectives:
In this assignment, you will learn about programming with sockets by implementing a modified version of the File Transfer Protocol (FTP) for transferring files between two machines.
The purpose of this assignment is to introduce you to the socket programming API as well as a variety of operating system concepts that are widely used in the construction of networked applications.
Recommendations & Rules:
• The application must be written in C/C++ in Linux Platform.
• Application should operate in concurrent mode. (So, application serve more than one
user at a time)
• Don’t use any special library for protocols when coding. Write your own functions if
it’s necessary.
• Unless otherwise explicitly specified, all written assignments or code that is submitted
is to be entirely the student’s own work. Using any code or copying any assignment
from others is strictly prohibited without advance prior permission from the instructor.
• All students’ work is their own. Students who do share their work with others are as
responsible for academic dishonesty as the student receiving the material.
• Students who encourage others to cheat or plagiarize, or students who are aware of plagiarism or cheating and do not report it are also participating in academically
dishonest behaviour.
• In this case (academically dishonest behaviour), students to be punished with no
grade.
Project Overview:
You will first write a server program that allows users (clients) to upload and download files to and from a specified directory. The server should be run as following arguments:
server -d directory -p port -u password
-d: Specify running directory which files to be accessed/modified/erased
-p: Define server port number
-u: Password file that uses delimiter separated format which is delimiter is “:” (https://en.wikipedia.org/wiki/Delimiter-separated_values)
For example,
After the server has started, it simply spins in an infinite loop waiting for incoming connections until you decide to stop it.
To connect to the server, you can use putty or nc as client for testing.
     $ server -d /home/student/files -p 1888 -u password File server listening on localhost port 1888
  
 Once the client has connected to the server, it should allow the user to interactively get a directory listing, download and upload files, and close the connection.
For example:
     $ nc localhost 1888
Welcome to Bob's file server.
USER test test123
200 User test granted to access.
LIST
a.out 5876
client 9130
client.c 580
.
GET client.c
#include<stdlib.h>
#include<stdio.h>
main() { }
.
GET foobar
404 File foobar not found.
PUT test
This is text message that put as example
.
200 44 Byte test file retrieved by server and was saved. LIST
a.out 5876
client 9130
client.c 580
test 44
.
DEL a.out
200 File a.out deleted.
DEL nofile
404 File nofile is not on the server.
QUIT
Goodbye!
$
  First, the server must be able to handle multiple client connections (all of which might be active at any given time).
Protocol
This version of FTP will be slight variation of the FTP protocol implemented over a TCP connection which is guaranteed to be reliable.
Commands
The client and server will communicate with each other by sending ASCII Commands as it given above. You should implement following commands:

 USER <Username> <PASS>
Server should handle simple user and password authentication from password file. Password file should look like as it given below:
If user name and password match server should give “200” response and allow following commands otherwise should give Error code like “400 User not found”. If user not found client only should use command USER or QUIT.
If user and password is Okay server should accept following commands:
LIST
If client gives this command in this case server should list filenames and file sizes to client. And after files listed list should terminated with “.”.
GET <filename>
If client gives this command in this case server should read file content of filename and send content to client side. Also, after file content dumped it should terminate with “\r\n.\r\n”. If file is not found server should give 404 Error.
PUT <filename>
If client gives this command in this case server should read file content from client and save to server side. Client should terminate file with “\r\n.\r\n”. After server take file should give message 200 and report how much Byte transferred and saved on server side. If server can not save file should give 400 Error.
   alice:qwerty bob:2021.sockets
       USER test test100
200 User test granted to access.
USER nobody xxx
400 User not found. Please try with another user.
       Filename1 X Filename2 Y Filename3 Z .
       GET file
This is file content. .
GET foobar
404 File foobar not found.
  
PUT test
This is text message that put as example
.
200 44 Byte test file retrieved by server and was saved.
PUT test
400 File cannot save on server side.
DEL <filename>
If client gives this command in this case server should delete file from server side. If its succeeded server should give 200 messages otherwise should give 404.
QUIT
If client gives this command in this case server should give goodbye message and terminates the client connection.
DEL a.out
200 File a.out deleted.
DEL nofile
404 File nofile is not on the server.
   QUIT
Goodbye!
  
