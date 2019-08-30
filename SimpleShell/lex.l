%{
#define YY_DECL char **yylex()
int _numargs = 100;
char *_args[100];
int _argcount = 0;
%}

WORD [a-zA-Z0-9\/\.-]+
SPECIAL [()><|&;*]+

%%

_argcount=0; _args[0]=NULL;

{WORD}|{SPECIAL} {
   if(_argcount < _numargs) {
      _args[_argcount++] = (char *)strdup(yytext);
      _args[_argcount] = NULL; 
   }
   else {
      printf("Error: Overloading arguments\n");
      printf("%d arguments reached\nExiting...", _argcount);
      exit(1);
   }
}
\n  {
   if(_argcount == 0) {
      _args[0] = NULL;
   }
   _argcount = 0;
   //printf("received a new line returning args\n");
   return _args;
}
[ \t]+
.
%%
char **getLine() { return yylex(); }
