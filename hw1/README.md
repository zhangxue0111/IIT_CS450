# Overview

Code for Lab 1.

## Build and execute

To compile, just type:

```
prompt> gcc -o myshell myshell.c
```

Then, run:

```
prompt> ./myshell
```

## Screen shots

see the attachment testcase1.png, testcase2.png and testcase3.png

### Comments and explain

The main purpose of this assignment is to implement the sequence command execution operator ";" and parallel execution operator "&" based on the skeleton code.  Here I explain modifications  I made to implement those functions.

1. defines two  data structures `struct listcmd` and `struct paracmd`:

   `listcmd`: defines a list command which will be executed in sequence

   ```c
   struct listcmd {
     int type;           // ;
     struct cmd *left;   // left side of list
     struct cmd *right;  // right side of list
   };
   ```

   `paracmd`: defines a parallel command which will be executed in parallel

   ```c
   struct paracmd {
     int type;		// &
     struct cmd *left;
     struct cmd *right;
   };
   ```

2. In function `runcmd`, add two cases. The first case is ';', the other one is '&'. The main idea to deal with sequence commands is that the parent process delays its execution until the child finishes executing, while the parallel commands don't need to do that. 

   ```c
   case ';':
       lcmd = (struct listcmd*)cmd;     // get a list command
       if(fork1()==0) {                 // fork a child process
         runcmd(lcmd->left);            // the child process executes the left command
       }
       wait(&r);          				// parent delays its execution
       runcmd(lcmd->right);             // executes the right command
       break;
   
   case '&':
       pacmd = (struct paracmd*)cmd;
       if(fork1() == 0) {
         runcmd(pacmd->left);
       }
       if(fork1() == 0) {
         runcmd(pacmd->right);
       }
       wait(&r);
       break;
   ```

   

3. Defines two functions `listcmd` and `paracmd` to dynamically allocated space every time a new command is encountered.

   ```c
   struct cmd*
   listcmd(struct cmd *left, struct cmd *right)
   {
     struct listcmd *cmd;
     cmd = malloc(sizeof(*cmd));
     memset(cmd, 0, sizeof(*cmd));
     cmd->type = ';';
     cmd->left = left;
     cmd->right = right;
     return (struct cmd*)cmd;
   }
   
   struct cmd*
   paracmd(struct cmd *left, struct cmd *right)
   {
     struct listcmd *cmd;
     cmd = malloc(sizeof(*cmd));
     memset(cmd, 0, sizeof(*cmd));
     cmd->type = '&';
     cmd->left = left;
     cmd->right = right;
     return (struct cmd*)cmd;
   }
   ```
   
4. Defines function to parse list commands and parallel commands.

   The list commands parse is executed in pretty much the way as an `execcmd`. The logic for parse a list command is that when checking symbol ';', the program get the left command. The child process will execute this left command. After the child process finishes the left command, the parent process goes ahead to execute the right command. In this way, we could make sure that left command will always execute before the right command. 

   ```c
   struct cmd*
   parseline(char **ps, char *es)
   {
     struct cmd *cmd;
     cmd = parsepipe(ps, es);
   
     // parse list command
     if(peek(ps, es, ";")){
       gettoken(ps, es, 0, 0);
       cmd = listcmd(cmd, parseline(ps, es));
     }
       
     if (peek(ps, es, "&")) {
       gettoken(ps, es, 0, 0);
       cmd = paracmd(cmd, parseline(ps, es));
     }
       
     return cmd;
   }
   ```
   


5. The last changes is about the function of parse execution. The main purpose of this function is to parse execute commands. I separate list command from parallel command. When peeking the symbol ";", the program parse commands to be executed just like the  pipe "|", which parses commands first and then executes, but when it comes to the symbol "&", the program will parse commands and executes them at the same time.  

   ```c
   struct cmd*
   parseexec(char **ps, char *es)
   {
     char *q, *eq;
     int tok, argc;
     struct execcmd *cmd;
     struct cmd *ret;
     
     ret = execcmd();
     cmd = (struct execcmd*)ret;
   
     argc = 0;
     ret = parseredirs(ret, ps, es);
     while(!peek(ps, es, "|;&")){     // add symble "; and &"                
       if((tok=gettoken(ps, es, &q, &eq)) == 0)
         break;
       if(tok != 'a') {
         fprintf(stderr, "syntax error\n");
         exit(-1);
       }
       cmd->argv[argc] = mkcopy(q, eq);
       argc++;
       if(argc >= MAXARGS) {
         fprintf(stderr, "too many args\n");
         exit(-1);
       }
       ret = parseredirs(ret, ps, es);
       ret = parseback(ret, ps, es); // parse back commands here
     }
     cmd->argv[argc] = 0;
     return ret;
   }
   
   ```

   

## Test cases

According to given conditions, I analysis the potential test cases and related expected results. 

| Scenario                                                     | commands                            | expected results                     |
| ------------------------------------------------------------ | ----------------------------------- | ------------------------------------ |
| case 1: no operator                                          | echo hello                          | hello                                |
| case 2: one operator                                         | echo hello;                         | hello                                |
|                                                              | echo hello &                        | A string terminated by & is illegal. |
| (*) see comments                                             | ; echo hello                        | hello                                |
|                                                              | & echo hello                        | syntax error                         |
|                                                              | echo hello;echo world               | hello<br />world                     |
|                                                              | echo hello & echo world             | hello world                          |
| case 3: two operator                                         | echo hello;echo world;              | hello<br />world                     |
|                                                              | echo hello & echo world&            | A string terminated by & is illegal. |
|                                                              | echo hello;echo world &             | A string terminated by & is illegal. |
|                                                              | echo hello & echo world;            | hello world                          |
|                                                              | echo hello &; echo world            | A string terminated by & is illegal  |
|                                                              | echo hello; & echo world            | syntax error                         |
| case 4 : three operator                                      | echo hello; echo world;echo 123;    | hello<br />world<br />123            |
|                                                              | echo hello;echo world;echo 123 &    | A string terminated by & is illegal  |
|                                                              | echo hello;echo world & echo 123    | hello<br />world 123                 |
|                                                              | echo hello;echo world & echo 123 &  | A string terminated by & is illegal  |
|                                                              | echo hello & echo world;echo 123;   | hello world<br />123                 |
|                                                              | echo hello & echo world & echo 123; | hello world 123                      |
| case 5: exception<br />illegal input<br />unsupported commands | hello                               | Command not found                    |

(*) comments: In this code, I assume that list commands are independent. When see the command at the beginning of ";"  just like ";echo hello",  I assume the left command of semicolon is none, and then the program will go ahead to parse the right command. 
