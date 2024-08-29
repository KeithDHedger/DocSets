/*
 *
 * ©K. D. Hedger. Mon 12 Aug 11:03:23 BST 2024 keithdhedger@gmail.com

 * This file (builddb.cpp) is part of docsets.

 * docsets is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * docsets is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with docsets.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <string>
#include <filesystem>
#include <sqlite3.h>

#include "LFSTKUtilityClass.h"

sqlite3			*DB; 
char				line[1024];
std::string		gotline;
std::string		thename;
std::string		thetype;
std::string		thepath;
std::string		prefixfolder;
std::string		indexname;
std::string		holdline;

int				verbose=0;
int				recordcnt=0;

void smartenLine()
{
	gotline=LFSTK_UtilityClass::LFSTK_strStrip(std::string(line));
	gotline=LFSTK_UtilityClass::LFSTK_strReplaceAllStr(gotline,"&lt;","",true);
	gotline=LFSTK_UtilityClass::LFSTK_strReplaceAllStr(gotline,"&gt;","",true);
	gotline=LFSTK_UtilityClass::LFSTK_strReplaceAllStr(gotline,"&amp;","",true);
}

void setThePath()
{
	long unsigned int	pos=0;

	thepath=LFSTK_UtilityClass::LFSTK_strStr(holdline,"link=\"",true);	
	pos=0;
	pos=thepath.find("\"",6);
	if(pos!=std::string::npos)
		thepath=thepath.substr(6,pos-6);
	else
		thepath="";
}

void setTheType()
{
	long unsigned int	pos=0;

	thetype=LFSTK_UtilityClass::LFSTK_strStr(holdline,"type=\"",true);
	pos=thetype.find("\"",6);
	if(pos!=std::string::npos)
		{
			thetype=thetype.substr(6,pos-6);
			if(thetype.compare("typedef")==0)
				thetype="Type";
			else if(thetype.length()>0)
				thetype.at(0)=std::toupper(thetype.at(0));
			else
				thetype="Annotation";
		}
}

void setTheName()
{
	long unsigned int	pos=0;

	thename=LFSTK_UtilityClass::LFSTK_strStr(holdline,"name=\"",true);
	pos=thename.find("\"",6);
	if(pos!=std::string::npos)
		{
			thename=thename.substr(6,pos-6);
		}
}

void insertInto(void)
{
	int			exit=0; 
	char			*messaggeError; 
	std::string	sql;

	if((thename.empty()==true) && (thepath.empty()==true))
		return;

	sql="insert into searchindex (name,type,path) values ('"+thename+"','"+thetype+"','"+thepath+"');";
	exit=sqlite3_exec(DB,sql.c_str(),NULL,0,&messaggeError);
	if(exit != SQLITE_OK)
		{ 
			if(verbose==2)
				std::cerr << "Error Insert: " << messaggeError<<std::endl; 
			sqlite3_free(messaggeError); 
			return;
		}
	recordcnt++;
}

void createEntry()
{
	long unsigned int	pos=0;

	holdline=LFSTK_UtilityClass::LFSTK_strStr(gotline,"<keyword ",true);
	if(holdline.length()>0)
		{
			setTheName();
			setTheType();
			setThePath();

			if(verbose==1)
				fprintf(stderr,"Name=\"%s\" Type=\"%s\" Path=\"%s\"\n",thename.c_str(),thetype.c_str(),thepath.c_str());
			insertInto();
		}
}

int main(int argc, char **argv)
{
    FILE			*fp=NULL;
 	std::string	command;
	int			exit=0; 
	char			*messaggeError=NULL; 
	std::string	sql;

	if(argc>3)
		verbose=atoi(argv[3]);

	exit=sqlite3_open(argv[2],&DB); 
  
	if (exit)
		{ 
			if(verbose==2)
				std::cerr << "Error open DB " << sqlite3_errmsg(DB) << std::endl; 
			return(0); 
		} 

	sql="CREATE TABLE searchIndex(id INTEGER PRIMARY KEY, name TEXT, type TEXT, path TEXT);CREATE UNIQUE INDEX anchor ON searchIndex (name, type, path);";
    exit=sqlite3_exec(DB,sql.c_str(),NULL,0,&messaggeError); 
  
	if(exit != SQLITE_OK)
		{ 
			if(verbose==2)
				std::cerr << "Error Creating Table" << messaggeError << std::endl; 
			sqlite3_free(messaggeError); 
		} 

	prefixfolder=std::filesystem::path(argv[1]).parent_path().filename();
	indexname=std::filesystem::path(argv[1]).filename();
 	command="cat "+std::string(argv[1]);

	sqlite3_exec(DB,"BEGIN TRANSACTION",NULL,NULL,&messaggeError);
    fp=popen(command.c_str(), "r");

	printf("Building from %s\n",indexname.c_str());

	if(fp!=NULL)
		{
			while(fgets(line,1024,fp))
				{
					smartenLine();
					createEntry();
				}
			pclose(fp);
		}
	sqlite3_exec(DB,"END TRANSACTION",NULL,NULL,&messaggeError);
	sqlite3_close(DB); 
	printf("\e[2K");
	printf("\e[1GFinished %s, inserted %i records.\n",indexname.c_str(),recordcnt);
	fflush(stdout);
	return 0;
}