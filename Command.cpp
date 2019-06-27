//
// Created by pooja on 05/06/19.
//

#include "Command.h"
/**
 * ctor for Command class*/
Command::Command(const string& command_line){
    // cout<<"in command";
    this->commandLine=command_line;
    bool status=true; //good or bad maybe
    string symbol{};
    string address1{};
    string address2{};
    parse(command_line);
}
/**
 * Parse function : reset and parse the commands supplied
 * @param command_line a string reference is supplied to be parsed*/
void Command::parse(const string& line){

    string temp=line;
    //removes all space characters
    temp.erase(std::remove(temp.begin(), temp.end(), ' '), temp.end());
    //transforms string to lower case
    transform(temp.begin(), temp.end(), temp.begin(), ::tolower);;
    //   cout<<"final value:"<<temp<<endl;
    string restTemp="\0";
    if(temp.length()==0){
        cout<<"zero length"<<'\n';
        return;
    }
    else if(temp[0]=='q'){
        return;
    }
    else if(!isalpha(temp[0])){
        //check for digit
        symbol='p';
        restTemp=temp;
    }
    else if(!isValidCommand(temp[0])){ //is alpha
        cout<<"Bad Command: "<<temp[0]<<'\n';
        return;

    }
    //is alpha & is valid command
    if(isalpha(temp[0])){
        symbol=temp[0];
        restTemp=temp.substr(1, temp.length());
    }
    /*  cout<<"symbol is: "<<symbol<<endl;
      cout<<"restemp is: "<<restTemp<<endl;*/
    int commas = std::count(restTemp.begin(), restTemp.end(), ',');
    if(restTemp.length()==0){
        address1=".";
        address2=".";
    }
    else if(commas >1){
        cout<<"Bad Command: ,"<<'\n';
        status=false;
        return;
    }
    else if(commas == 0){
        //parse for first line index
        address1 = restTemp.substr(0, restTemp.find(","));
        address2 = address1;
    }else{  //contains 1 comma, e.g. x,y
        address1 = restTemp.substr(0, restTemp.find(","));
        address2 = restTemp.substr(restTemp.find(",")+1, restTemp.length()-1);

        if(address1.length() == 0)
            address1=".";

        if(address2.length() == 0)
            address2=address1;

        compareAddresses();
    }
    if(!isValidDigit(address1)){
        cout<<"Bad address 1: "<<address1<<'\n';
        status= false;
        return;
    }else if(!isValidDigit(address2)){
        cout<<"Bad address 2: "<<address2<<'\n';
        status= false;
        return;
    }
    // compareAddresses();
    cout<<"at the end: symbol: "<<symbol<<" address1: "<<address1<<" address2: "<<address2<<endl;

    //check symbol
    if(symbol=="i"){
        return;
    }
}
void Command::compareAddresses() {
    if(address1=="0") {
        address1 = "1";
    }
    if(address2=="0") {
        address2 = "1";
    }
    //first compare . and $
    if(address1=="$" && address2==".")//Todo (is not swapping $,.)
        swapAddresses();
    else if(address1=="$" || address1=="." || address2=="$" || address2==".")   //ignore if any address is . or $
        return;
    else {  //this means both addresses are positive integers
        int address1Number = std::atoi (address1.c_str());
        int address2Number = std::atoi (address2.c_str());
        if(address1Number>address2Number)
            swapAddresses();
    }
}
void Command::swapAddresses(){
    string tempaddress=address1;
    address1=address2;
    address2=tempaddress;
}

bool Command::isValidDigit(string& address){
    if(address.length()==1 && (address[0]=='.'|| address[0]=='$'))
        return true;

    bool isNegative=false;
    string temp=address;
    if(address[0]=='-'){
        temp=address.substr(1, address.length()-1);
        isNegative=true;
    }

    for(int i=0;i<temp.length();i++){
        if (!isdigit(temp[i]))
            return false;
    }
    if(isNegative)
        address="1";
    return true;
}

bool Command::isValidCommand(const char &ch){
    return (ch=='a' || ch=='i' || ch=='v' || ch=='d' || ch=='x' || ch=='j' || ch=='p' || ch=='c' || ch=='u' || ch=='n' || ch=='g' || ch=='w');
}
/**
 * getter function for symbol*/
string Command::getSymbol(){
    return symbol;
}
/**
 * getter function for address1*/
string Command::getAddress1(){
    return address1;
}
/**
 * getter function for address2*/
string Command::getAddress2(){
    return address2;
}
/**
 * getter function for status*/
bool Command::getStatus(){
    return status;
}
/**
 * setter function for status*/
void Command::setStatus(bool status){
    this->status=status;
}