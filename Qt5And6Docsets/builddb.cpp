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
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/limits.h>

#include "LFSTKUtilityClass.h"

sqlite3			*DB; 
char				line[1024];
std::string		gotline;
std::string		thename;
std::string		thetype;
std::string		thepath;
std::string		prefixfolder;
std::string		folderpath;
std::string		indexname;
std::string		holdline;

int				verbose=0;
bool				gotlongname=false;
int				totlen;
struct winsize	w;
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

	thepath=LFSTK_UtilityClass::LFSTK_strStr(holdline,"href=\"",true);	
	pos=0;
	pos=thepath.find("\"",6);
	if(pos!=std::string::npos)
		thepath=prefixfolder+"/"+thepath.substr(6,pos-6);
	else
		thepath="";
}

void setTheName()
{
	long unsigned int	pos=0;

	thename=LFSTK_UtilityClass::LFSTK_strStr(holdline,"fullname=\"",true);
	if(thename.length()>0)
		{
			pos=0;
			pos=thename.find("\"",10);
			if(pos!=std::string::npos)
				{
					thename=thename.substr(10,pos-10);
					gotlongname=true;
				}
		}
	else
		{
			thename=LFSTK_UtilityClass::LFSTK_strStr(holdline,"name=\"",true);
			pos=thename.find("\"",6);
			if(pos!=std::string::npos)
				{
					thename=thename.substr(6,pos-6);
					gotlongname=false;
				}
		}	
}

void insertInto(void)
{
	int			exit=0; 
	char			*messaggeError; 
	std::string	sql;

	if((thename.empty()==true) && (thepath.empty()==true))
		return;

	sql=folderpath+"/"+thepath.substr(0,thepath.find_last_of('#'));

	if(sql.length()<NAME_MAX)
		{
			if(std::filesystem::is_regular_file(sql)==false)
				{
					if(verbose==2)
						std::cerr << "Error missing file: " <<sql << std::endl;
					return;
				}
		}
	else
		{
			if(verbose==3)
				std::cerr << "Filename to long: " <<sql << std::endl;
			return;
		}

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

void getFunction(void)
{
	long unsigned int	pos=0;
	std::string			sig;
	
	holdline=LFSTK_UtilityClass::LFSTK_strStr(gotline,"<function ",true);
	if(holdline.length()>0)
		{
			setTheName();
			setThePath();
			thetype=LFSTK_UtilityClass::LFSTK_strStr(holdline,"meta=\"",true);

			if(thetype.length()>0)
				{
					pos=thetype.find("\"",6);
					if(pos!=std::string::npos)
						thetype=thetype.substr(6,pos-6);
					if((thetype.compare("plain")==0) || (thetype.compare("move-constructor")==0) || (thetype.compare("move-assign")==0) || (thetype.compare("copy-constructor")==0) || (thetype.compare("copy-assign")==0) || (thetype.compare("")==0) || (thetype.compare("")==0) || (thetype.compare("")==0) || (thetype.compare("destructor")==0) || (thetype.compare("constructor")==0) || (thetype.compare("move-constructor")==0) || (thetype.compare("copy-constructor")==0))
						thetype="Function";
					else if(thetype.compare("qmlmethod")==0)
						thetype="Method";
					else if(thetype.compare("signal")==0)
						thetype="Event";
					else if(thetype.compare("slot")==0)
						thetype="Callback";
					else
						thetype="Function";
				}

			sig=LFSTK_UtilityClass::LFSTK_strStr(holdline,"signature=\"",true);
			if(sig.length()>0)
				{
					pos=sig.find("\"",11);
					if(pos!=std::string::npos)
						sig=sig.substr(11,pos-11);
					std::string shortname;
					std::string finalname;
					shortname=LFSTK_UtilityClass::LFSTK_strStr(holdline,"name=\"",true);
					pos=shortname.find("\"",6);
					if(pos!=std::string::npos)
						shortname=shortname.substr(6,pos-6);
					finalname=thename;
					if(gotlongname==true)
						{
							finalname=LFSTK_UtilityClass::LFSTK_strReplaceAllStr(sig,shortname,"XO@XO@XO@");
							finalname=LFSTK_UtilityClass::LFSTK_strReplaceAllStr(finalname,"XO@XO@XO@",thename);
							thename=finalname;
						}
				}

			if(verbose==1)
				fprintf(stderr,"Name=\"%s\" Type=\"%s\" Path=\"%s\"\n",thename.c_str(),thetype.c_str(),thepath.c_str());
			insertInto();
		}
}

void createEntry(std::string findwhat,std::string dbtypename)
{
	long unsigned int	pos=0;

	holdline=LFSTK_UtilityClass::LFSTK_strStr(gotline,findwhat,true);
	if(holdline.length()>0)
		{
			setTheName();
			setThePath();
			thetype=dbtypename;

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
	int			steps=0;
	int			cnt=0;

    ioctl(STDOUT_FILENO,TIOCGWINSZ,&w);
   
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
	folderpath=std::filesystem::path(argv[1]).parent_path().parent_path();
	indexname=std::filesystem::path(argv[1]).filename();
 	command="cat "+std::string(argv[1]);

	sqlite3_exec(DB,"BEGIN TRANSACTION",NULL,NULL,&messaggeError);
    fp=popen(command.c_str(), "r");
	totlen=std::string("Building from  "+indexname).length();
	cnt=0;
	steps=0;

	printf("\e[%i;1H",w.ws_row);
	printf("Building from %s ",indexname.c_str());
	fflush(stdout);

	if(fp!=NULL)
		{
			while(fgets(line,1024,fp))
				{
					steps++;
					if(steps>100)
						{
							steps=0;
							cnt++;
							printf("▒");
							fflush(stdout);
						}
					if(cnt>=(w.ws_col-totlen-1))
						{
							printf("\e[%i;1H",w.ws_row);
							printf("Building from %s \e[0K",indexname.c_str());
							cnt=0;
							steps=0;
						}

					smartenLine();
					createEntry("<class ","Class");
					getFunction();
					createEntry("<property ","Property");
					createEntry("<qmlproperty ","Property");
					createEntry("<qmlclass ","Class");
					createEntry("<namespace ","Namespace");
					createEntry("<enum ","Enumeration");
					createEntry("<struct ","Structure");
					createEntry("<variable ","Variable");
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