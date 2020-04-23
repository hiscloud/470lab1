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

void exitMode(deque<string> cmd,int* exitcodeP){
	if (cmd.size()==1){
		exit(*exitcodeP);
	}
	else if (cmd.size()==2&&(cmd[1]=="-h"||cmd[1]=="-help")){
		cout<<"Usage: exit\n";
		cout<<"Terminates the shell with the exit value n if argument is given, otherwise the exit value will be the value returned bythe last executed command (or 0 if no commands were executed).\n";
		*exitcodeP=0;
	}
	else if (cmd.size()==2){
		string exitCode= cmd[1];
		
		try {
			exit(stoi(exitCode));
		}catch(const exception& e){
			cout<<"command exit only works with a number/itself!"<<endl;
			*exitcodeP=1;
		}
	}
	else{
		cout<<"command exit only works with a number/itself!"<<endl;
		*exitcodeP=1;
	}
	
}
void promptMode(deque<string> cmd, string* prompt,int* exitcodeP){
	if (cmd.size()==1){
		*prompt="cwushell> "; 
	}else if(cmd.size()==2){
		if(cmd[1]=="-h" || cmd[1]=="-help" ){
			cout<<"Usage: prompt [new prompt]\n";
			cout<<"Change the current prompt to a new one which is given by the parameter. If there's no parameters, the prompt will be changed to the default (cwushell>).\n";	
			*exitcodeP=0;
		}else{
			*prompt=cmd[1];
			*prompt+="> ";
			*exitcodeP=0;
		}
	
		
	}else{
		
		cout<<"Please enter only one argument after prompt."<<endl;
		*exitcodeP=1;
	}
	
}

void cpuinfoMode(deque<string> cmd,int* exitcodeP){
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
		*exitcodeP=0;
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
			*exitcodeP=1;
		}else{
			if (isC)
				cout<<"The cpu clock "<<clockS<<" MHz.\n";
			if (isT)
				cout<<"The cpu type is "<<typeS<<".\n";
			if (isN)
				cout<<"The number of cpu cores is "<<numberS<<".\n";
			*exitcodeP=0;
		}
		
		
	}
	
}

void meminfoMode(deque<string> cmd,int* exitcodeP){
	string totalMemS,freeMemS,l2cacheS,dummy;
	ifstream in;
	in.open("/proc/meminfo");
	in>>dummy;
	in>>totalMemS; //both kB
	in>>dummy>>dummy;
	in>>freeMemS; 
	long totalMem= stol(totalMemS);
	long freeMem= stol(freeMemS);
	long usedMem= totalMem - freeMem;
	
	in.close();
	//l2cache size/core
	
	string numberS;
	in.open("/proc/cpuinfo");
	while(in.good()){
		in>>dummy;
		if(dummy=="cores"){
		   in>>dummy;
		   in>>numberS;
		}
	}
	
	long coreN=stol(numberS);
	
	in.close();
	long sum=0;
	for(int i=0;i<coreN;i++){
		string iS= to_string(i);
		string filename="/sys/devices/system/cpu/cpu"+iS+"/cache/index2/size";
		//cat /sys/devices/system/cpu/cpu0/cache/index2/size
		in.open(filename);
		in>> l2cacheS;
		l2cacheS.pop_back();
		
		long l2cache=stol(l2cacheS);
		sum+=l2cache;
		in.close();	
	}
	long result=sum/coreN;

	
	if(cmd.size()==1 ||(cmd.size()==2&&(cmd[1]=="-help"||cmd[1]=="-h"))){
		cout<<"Usage: meminfo [-switch]\n";
		cout<<"1)-t will print the total RAM memory available in the system in bytes, 2) -u will print the used RAM memory and 3) -c will print the size of the L2 cache/core in bytes.\n";
		*exitcodeP=0;
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
			*exitcodeP=1;
		}else{
			if (isT)
				cout<<"The total RAM memory is "<<totalMem*1024<<" bytes.\n";
			if (isC)
				cout<<"The used RAM memory is "<<usedMem*1024<<" bytes.\n";
			if (isU)
				cout<<"The size of the L2 cache/core is "<<result*1024<<" bytes.\n";
			*exitcodeP=0;
		}
	}
}
void manualMode(deque<string> cmd,int * exitcodeP){
	if (cmd.size()>=2){
		cout<<"type 'manual' only to check the mannual file of cwushell"<<endl;
		*exitcodeP=1;
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
		*exitcodeP=0;
	}
}
void interpret(string input, string* outputP,int* exitcodeP){
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
		exitMode(command,exitcodeP);	//1 exit
	}	
	
	
	else if(command[0]=="prompt"){
		promptMode(command,outputP,exitcodeP);//2 prompt
		
	}	
	//*outputP="sb";
	
	else if (command[0]=="cpuinfo" ){
		
		cpuinfoMode(command,exitcodeP);//3
	}
	else if (command[0]=="meminfo"){
		
		meminfoMode(command,exitcodeP);//4
	}
	else if (command[0]=="manual"){
		manualMode(command,exitcodeP);//manual
	}else{
		const char* cmd=&input[0];
		try {
			system(cmd);
	//5 system commands
		}catch(const exception& e){
			cout<<"no such command as: "<<command[0]<<endl;
			*exitcodeP=1;
		}
	}
	
	
}

int main(){
	string output="cwushell> ";
	int exitcode;
	string input;
	input=read(output);
	exitcode=0;
	
	while(input != "exit"){
		
		interpret(input,&output,&exitcode);
		input=read(output);
		
		
	}
	
}
