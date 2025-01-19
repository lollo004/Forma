%{
#include "static_analysis.h"
#include "forma.tab.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

void yyerror(const char *msg);
extern StaticScopeStack *_staticstack;
%}

/* Modalità */
%x STRING_MODE COMMENT_MODE

/* Token Regex */
INTEGER      		[0-9]+
FLOATING        	[0-9]+\.[0-9]+
EQ			==
NEQ			!=
MINE			<=
GRTE			>=
ARROW			=>
COMMENT			%
STRING_DELIMITER     	\"
STRING_ESCAPE        	\\\"
BINARY       		0b[01]+
OCTAL        		0o[0-7]+
HEXADECIMAL  		0x[0-9a-fA-F]+
IL			i\[\]
FL			f\[\]
CL			c\[\]
SL			s\[\]
IF           		if
ELSE         		else
RETURN			return
READ			read
WRITE			write
RANDOM			random
LET			let
DEF			def
IMGR			I
SET			N|Z|R|C|S
ID           		[A-Za-z_][A-Za-z0-9_]*
NL			\n
NEWLINE              	\\n
SPACE			" "
ANY_STR			[^\n^\t]{1}
ANY_CHARACTER        	[^ ^\n^\t]{1}

%option noyywrap noinput nounput yylineno
%%

{SPACE}	{ }
{NL} { /* For yylineno debug */ }
{IL}	{ return _il; }
{FL}	{ return _fl; }
{CL}	{ return _cl; }
{SL}	{ return _sl; }
{RANDOM} { return _random; }
{IF} 	{ return _if; }
{ELSE} 	{ return _else; }
{RETURN}	{ return _return; }
{IMGR}	{ return imgr; }
{SET}	{ yylval.ch = *yytext; return set; }
{LET}	{ return let; }
{DEF}	{ enter_static_scope(_staticstack); return def; }
"}" { exit_static_scope(_staticstack); return *yytext; }

{EQ}	{ return eq; }
{NEQ}	{ return neq; }
{MINE}	{ return mine; }
{GRTE}	{ return grte; }
{ARROW}	{ return arrow; }

{INTEGER} { /* Numeric Literals */
	errno = 0;
	long long value = strtoll(yytext, NULL, 10);
	if (errno == ERANGE) {
        	fprintf(stderr, "Error: Invalid integer literal '%s'.\n", yytext);
        exit(EXIT_FAILURE);
    	}
    	yylval.integer = value;
    	return integer;
}
{FLOATING} {
	errno = 0;
	long double value = strtold(yytext, NULL);
    	if (errno == ERANGE) {
        	fprintf(stderr, "Error: Invalid float literal '%s'.\n", yytext);
        exit(EXIT_FAILURE);
    	}
    	yylval.real = value;
    	return real;
}
{BINARY} {
    	errno = 0;
    	long long value = strtoll(yytext + 2, NULL, 2);
    	if (errno == ERANGE) {
        	fprintf(stderr, "Error: Invalid binary literal '%s'.\n", yytext);
        exit(EXIT_FAILURE);
    	}
    	yylval.integer = value;
    	return integer;
}
{OCTAL} {
    	errno = 0;
    	long long value = strtoll(yytext + 2, NULL, 8);
    	if (errno == ERANGE) {
        	fprintf(stderr, "Error: Invalid octal literal '%s'.\n", yytext);
        exit(EXIT_FAILURE);
    	}
    	yylval.integer = value;
    	return integer;
}
{HEXADECIMAL} {
    	errno = 0;
    	long long value = strtoll(yytext + 2, NULL, 16);  /* Salta il prefisso '0x' */
    	if (errno == ERANGE) {
        	fprintf(stderr, "Error: Invalid hexadecimal literal '%s'.\n", yytext);
        exit(EXIT_FAILURE);
    	}
    	yylval.integer = value;
    	return integer;
}

{READ}	{ return _read; }
{WRITE} { return _write; }

{ID} {
    	// Check if the ID exists in the current scope or above
	Symbol *id = find_static_symbol(_staticstack, yytext);	
	yylval.id = strdup(yytext);
	if (!id)return new_id;
	if (id->nature == NATURE_VARIABLE) {
		switch(id->type){
		case TYPE_INT: return int_var;
		case TYPE_FLOAT: return float_var;
		case TYPE_STRING: return str_var;
		case TYPE_COMPLEX: return cmpx_var;
		case TYPE_INTLIST: return intlist_var;
		case TYPE_FLOATLIST: return floatlist_var;
		case TYPE_STRINGLIST: return strlist_var;
		case TYPE_COMPLEXLIST: return cmpxlist_var;
	}}
	else {
		switch(id->type){
		case TYPE_INT: return int_fun;
		case TYPE_FLOAT: return float_fun;
		case TYPE_STRING: return str_fun;
		case TYPE_COMPLEX: return cmpx_fun;
		case TYPE_INTLIST: return intlist_fun;
		case TYPE_FLOATLIST: return floatlist_fun;
		case TYPE_STRINGLIST: return strlist_fun;
		case TYPE_COMPLEXLIST: return cmpxlist_fun;
	}
	}
}

{COMMENT} {
    BEGIN(COMMENT_MODE);
}
<COMMENT_MODE>{NL} {
    BEGIN(INITIAL); // Exit COMMENT_MODE on newline
}
<COMMENT_MODE>{ANY_CHARACTER} {
    // Ignore all characters within a comment
}

{STRING_DELIMITER} {/* Strings */
    	BEGIN(STRING_MODE);
    	yylval.str = strdup("");
}
<STRING_MODE>{STRING_ESCAPE} {
    	yylval.str = realloc(yylval.str, strlen(yylval.str) + 2);
    	strcat(yylval.str, yytext);
}
<STRING_MODE>{STRING_DELIMITER} {
    	BEGIN(INITIAL);
    	return str;
}
<STRING_MODE>{NEWLINE} {
    	yylval.str = realloc(yylval.str, strlen(yylval.str) + 2);
    	strcat(yylval.str, "\n");
}
<STRING_MODE>{ANY_STR} {
    	yylval.str = realloc(yylval.str, strlen(yylval.str) + 2);
    	strncat(yylval.str, yytext, 1);
}

{ANY_CHARACTER} { return *yytext; }

%%
