//Junyu Lu 41176974
//This is my own work.

#include <iostream>
#include <unistd.h>
#include <deque>
#include <exception>
#include <fstream>
#include "stdio.h"
using namespace std;


string read(string output){
		string input;
		cout<<output;
		getline(cin,input);
		return input;
}

void exitMode(deque<string> cmd){
	if (cmd.size()==1){
		exit(0);
	}
	else if (cmd.size()==2){
		string exitCode= cmd[1];
		/*bool okToExit=true;
		for (int i=0;i< exitCode.length();i++){
			if (!exitCode.at(i).isDigit())
				okToExit=false;
			
		
		}
		if (okToExit)
			exit(stoi(exitCode));
		*/
		try {
			exit(stoi(exitCode));
		}catch(const exception& e){
			cout<<"command exit only works with a number!"<<endl;
		}
	}
	else{
		exit(0);
	}
	
}
void promptMode(deque<string> cmd, string* prompt){
	if (cmd.size()==1){
		*prompt="cwushell> "; 
	}else if(cmd.size()==2){
		if(cmd[1]=="-h" || cmd[1]=="-help" ){
			cout<<"Usage: prompt [new prompt]\n";
			cout<<"Change the current prompt to a new one which is given by the parameter. If there's no parameters, the prompt will be changed to the default (cwushell>).\n";	
		}else{
			*prompt=cmd[1];
			*prompt+="> ";
		}
	
		
	}else{
		
		cout<<"Please enter only one argument after prompt."<<endl;
	}
	
}

void cpuinfoMode(deque<string> cmd){
	string clockS,typeS,numberS,dummy;

	ifstream in;
	in.open("/proc/cpuinfo");
	while(in.good()){
		in>>dummy;
		if (dummy=="MHz"){
			in>>dummy;
			in>>clockS;
			
		}
		if(dummy=="name"){
			in>>dummy;
			getline(in,typeS);
			
		}
		if(dummy=="cores"){
			in>>dummy;
		   in>>numberS;
		 
		}
	}
	in.close();
	if (cmd.size()==1 || (cmd.size()==2&& (cmd[1]=="-h"||cmd[1]=="-help"))){
		cout<<"Usage: cpuinfo [-switch]\n";			       
		cout<<"1)-c will print the cpu clock(e.g.3.2GHz),2)-t will print the cpu type(e.g. Intel) and 3) -n -will print the number of cores(eg.8).\n";				       
	}else {
		bool isValid,isC,isT,isN;
		isValid=true;
		isT=false;
		isN=false;
		isC=false;
		for(int i=1;i<cmd.size();i++){
			if (cmd[i]=="-c")
				isC=true;
			if(cmd[i]=="-t")
				isT=true;
			if(cmd[i]=="-n")
				isN=true;
			if(cmd[i]!="-c"&&cmd[i]!="-n"&&cmd[i]!="-t")
				isValid=false;
		}
		if(!isValid){
			cout<<"invalid command! Enter 'cpuinfo' to see instructions.\n";
		}else{
			if (isC)
				cout<<"The cpu clock "<<clockS<<" MHz.\n";
			if (isT)
				cout<<"The cpu type is "<<typeS<<".\n";
			if (isN)
				cout<<"The number of cpu cores is "<<numberS<<".\n";
		}
		
		
	}
	
}

void meminfoMode(deque<string> cmd){
	string totalMemS,freeMemS,l2cacheS,dummy;

	ifstream in;
	in.open("/sys/devices/system/cpu/cpu0/cache/index2/size");
	in>> l2cacheS;
	
	in.close();	
	in.open("/proc/meminfo");
	in>>dummy;
	in>>totalMemS; //both kB
	in>>dummy>>dummy;
	in>>freeMemS; 
	long totalMem= stol(totalMemS);
	long freeMem= stol(freeMemS);
	long usedMem= totalMem - freeMem;

	long l2cache=stol(l2cacheS);
	
	in.close();
	if(cmd.size()==1 ||(cmd.size()==2&&(cmd[1]=="-help"||cmd[1]=="-h"))){
		cout<<"Usage: meminfo [-switch]\n";
		cout<<"1)-t will print the total RAM memory available in the system in bytes, 2) -u will print the used RAM memory and 3) -c will print the size of the L2 cache/core in bytes.\n";
	}else{
		bool isValid,isT,isU,isC;
		isValid=true;
		isT=false;
		isU=false;
		isC=false;
		for(int i=1;i<cmd.size();i++){
			if (cmd[i]=="-t")
				isT=true;
			if(cmd[i]=="-u")
				isU=true;
			if(cmd[i]=="-c")
				isC=true;
			if(cmd[i]!="-c"&&cmd[i]!="-u"&&cmd[i]!="-t")
				isValid=false;
		}
		if(!isValid){
			cout<<"invalid command! Enter 'meminfo' to see instructions.\n";
		}else{
			if (isT)
				cout<<"The total RAM memory is "<<totalMem*1024<<" bytes.\n";
			if (isC)
				cout<<"The used RAM memory is "<<usedMem*1024<<" bytes.\n";
			if (isU)
				cout<<"The size of the L2 cache/core is "<<l2cache*1024<<" bytes.\n";
		}
	}
}
void manualMode(deque<string> cmd){
	if (cmd.size()>=2){
		cout<<"type 'manual' only to check the mannual file of cwushell"<<endl;
	}else{
		cout<<"CWUSHELL						General Commands Manual\n\n";
		cout<<"NAME\n	cwushell\n\n";
		cout<<"DESCRIPTION\n	cwushell is a linux shell which mainly provides the feature of checking the cpu and memory information of one linux machine.\n\n";
		cout<<"OPTIONS\n	1. exit [n] -- terminates the shell with the exit value n if argument is given, otherwise the exit value will be the value returned bythe last executed command (or 0 if no commands were executed).\n";
		cout<<"	2. prompt <new prompt> -- change the current shell prompt to the new_prompt. If no arguments given, will change to cwushell\n.";	
		cout<<"	3. cpuinfo -switch --1)-c will print the cpu clock(e.g.3.2GHz),2)-t will print the cpu type(e.g. Intel) and 3) -n -will print the number of cores(eg.8).\n";
	    	cout<<"	4. meminfo -swich -- 1)-t will print the total RAM memory available in the system in bytes, 2) -u will print the used RAM memory and 3) -c will print the size of the L2 cache/core in bytes.\n";
		cout<<"	5. all other exising shell commands(e.g. ls, cat,pwd).\n";
		cout<<"CWUSHELL						General Commands Manual\n\n";
	}
}
void interpret(string input, string* outputP){
	deque<string> command;
	string s="";
	for(int i=0;i<input.length();i++){
		if (input.at(i)!=' ') {
			s+=input.at(i);
		}else if(s!=""){
			command.push_back(s);
			s="";
		}
		
	}
	if (s!="")
		command.push_back(s);
	//command stored with input words.
	

	if(command[0]=="exit"){
		exitMode(command);	//1 exit
	}	
	
	
	else if(command[0]=="prompt"){
		promptMode(command,outputP);//2 prompt
		
	}	
	//*outputP="sb";
	
	else if (command[0]=="cpuinfo" ){
		
		cpuinfoMode(command);//3
	}
	else if (command[0]=="meminfo"){
		meminfoMode(command);//4
	}
	else if (command[0]=="manual"){
		manualMode(command);//manual
	}else{
		const char* cmd=&input[0];
		try {
			system(cmd);
	//5 system commands
		}catch(const exception& e){
			cout<<"no such command as: "<<command[0]<<endl;
		}
	}
	
	
}

int main(){
	string output="cwushell> ";

	string input;
	input=read(output);
	
	while(input != "exit"){
		interpret(input,&output);
		input=read(output);
		
		
	}
	
}
