#pragma once

//-------------------------------------------------------------------
//PRAGMA TODO
//The following code handles creating FIXME, TODO, and NOTE methods.
//Code created in refrence to: http://www.flipcode.com/archives/FIXME_TODO_Notes_As_Warnings_In_Compiler_Output.shtml

#define _QUOTE(x) # x
#define QUOTE(x) _QUOTE(x)
#define __FILE__LINE__ __FILE__ "(" QUOTE(__LINE__) ") : "

#define PRAGMA(p) __pragma(p)
#define NOTE( x ) PRAGMA( message(x) )
#define FILE_LINE NOTE( __FILE__LINE__)

#define TODO( x ) NOTE( __FILE__LINE__ "\n" )
#define FIXME( x ) message( __FILE__LINE__"\n"        \
         " ---------------------------------------\n" \
         "| FIXME : " #x "\n" \
         " ---------------------------------------\n" )
#define todo( x ) message( __FILE__LINE__" TODO :   " #x "\n" )
#define fixme( x ) message( __FILE__LINE__" FIXME:   " #x "\n")


/*
//example code
int main(int argc, char* argv[])
{
	#pragma TODO(  We have still to do some work here... )
	#pragma FIXME( Limits are not controlled in that function or things like that )

	#pragma todo(  Have a look to flipcode daily ! )
	#pragma todo(  Sleep... )

	#pragma fixme( It seems that there is some leaks in that object )

	#pragma FILE_LINE
	#pragma NOTE( " \n\
	A free format multiline, comment............\n\
	So I can put a whole text here              \n\
	-------------------------------------------------")
	return 0;
}
//---------------------------------------------------------------------------------------------
*/