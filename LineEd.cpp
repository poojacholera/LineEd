//
// Created by pooja on 05/06/19.
//

#include "LineEd.h"
LineEd::LineEd(const string& f) {
    //reset buffer
    buffer.clear();
    filename=f;
    if(filename.empty()){
        cout<<"\"? \""<<'\t'<<"[ New File ]" <<'\n';
        currentLine=0;
        totalLines=0;
        input="";
    }else {
        cout << "\"? \"" << '\t' << "\"" << filename << "\"" << '\n';
        filename = "/home/meet/pooja-workspace/CLionWorkspace/Ass2" + filename;
        int count = addToBuffer(filename);
        if (count >= 1) {
            cout<<"number of lines in file : "<<count;
            this->totalLines=count;
            this->currentLine=count;
            input = "p1,$";
            Command command1(input);
            printBuffer(command1);
        } else{
            currentLine = 1;
            totalLines = 0;
            input = "";

        }
    }
}
void LineEd::run() {
    //open file
    // count lines in the file
    //if file does not exist create one file
    //ask question
    while(tolower(input[0])!='q'){
        bool editorflag= runCommand(input);
        if(!editorflag && !buffer.empty()) {
            string question="There are unsaved changes.\nDo you wish to save the changes (y or n) ?";
            if(askYesNo(question)){
                cout<<"Enter the name of a file to write to: ";
                cin>>filename;
                if(!filename.empty()){
                    bufferToFile(filename);

                }
            }
            cout<<"bye";
            return;
        }
        //reset input
        input="";
        //Enter input mode if i and exit input mode if  .
        cout<<"Entering Input Mode"<<'\n';
        while(true){
            if(!inputMode())break;
        }
    }
}
bool LineEd::runCommand(const string & c) {
    //call command from here

    string cmdLine=c;
    char ch;
    cout<<"Entering Command Mode"<<'\n';
    //exit command mode if  i

    while(tolower(cmdLine[0])!='i' && tolower(cmdLine[0])!='q'){
        if(tolower(cmdLine[0])=='q') return false; //return false only if symbol =q
        cout<<'?'<<'\t';

        getline(cin >> ws,cmdLine);
        Command cmd(cmdLine);
        if(tolower(cmdLine[0])=='q'){
            return false;
        }else if((cmd.getSymbol()=="p" || cmd.getSymbol()=="." || cmd.getSymbol()=="$" )&& buffer.empty() && cmd.getStatus()){
            cout<<"File empty. Must use one of I, A, Q commands."<<'\n';
        }else if((cmd.getSymbol()=="p" || cmd.getSymbol()=="." || cmd.getSymbol()=="$" )&& !buffer.empty() && cmd.getStatus()){
            if(cmd.getSymbol()=="p" && !buffer.empty()){
                printBuffer(cmd);
            }
        }else if(cmd.getSymbol()=="a" && cmd.getStatus()){

            //Enter input mode if i and exit input mode if  .
            cout<<"Entering Input Mode"<<'\n';
            while(true){
                if(!inputMode())break;
            }input="";
            runCommand(input);
        } else if(cmd.getSymbol()=="w" && cmd.getStatus()){
            if(!filename.empty()){
                string question="Save changes to the file: " +filename+" (y or n)?";
                if(askYesNo(question)){
                    bufferToFile(filename);
                    cout<<buffer.size()<<" lines written to file \""<<filename<<"\"" <<'\n';
                }
            }else{
                string question="Save changes to the file:  (y or n)?";
                if(askYesNo(question)) {
                    cout << "Enter the name of a file to write to: ";
                    cin >> filename;
                    bufferToFile(filename);
                    cout << buffer.size() << " lines written to file \"" << filename << "\""<<'\n';
                }
            }
        }else if(cmd.getSymbol()=="g" && cmd.getStatus()){
            cout<<"currentLine is : "<<cmd.getAddress1();
        }

    }
    return true;

}
bool LineEd::askYesNo(const string & question){
    cout<<question;
    string answer;
    char ch;
    while(ch!='y' && ch!='n'){
        getline(cin>>ws,answer);
        ch=tolower(answer[0]);
    }
    return ch=='y';
}
int LineEd::addToBuffer(const string& file){

    string str;
    ifstream in(file);
    int count=0;
    if(in.is_open()){
        // Read the next line from File until it reaches the end.
        while (getline(in, str))
        {
            // Line contains string of length > 0 then save it in vector
            if(str.size() > 0)
                buffer.push_back(str);
            ++count;
        }
        in.close();
    }else{
        cout<<"unable to open";
    }
    return count;
}
void LineEd::printBuffer(Command& cmd) const{
    //cout<<"in print"<<endl;
    int from=1, to=0;
    //address1
    if(cmd.getAddress1()=="."){
        from=this->currentLine;
    }else if(cmd.getAddress1()=="$"){
        from=totalLines;
    }else{
        from=stoi(cmd.getAddress1());
    }
    //address 2
    if(cmd.getAddress2()=="."){
        to=currentLine;
    }else if(cmd.getAddress2()=="$"){
        to=this->totalLines;
    } else{
        to=stoi(cmd.getAddress2());
    }

    if(!buffer.empty())
    {  // currentLine=buffer.size();
        // Print the vector contents
        int linecount=1;
        for(list<string>::const_iterator it = buffer.begin(); it != buffer.end(); ++it)
        {   advance(it,from-1);
            if(linecount!=currentLine){
                cout<<linecount<<":\t"<<it->c_str()<<'\n';
            }else{
                cout<<linecount<<">\t"<<it->c_str()<<'\n';
            }
            linecount++;
            if (linecount>to)return;
        }linecount=0;
    }
}
void LineEd::bufferToFile(const string & filename){
    ofstream ofstream1;
    ofstream1.open("/home/meet/pooja-workspace/CLionWorkspace/Ass2"+filename, std::ofstream::app);
    if (ofstream1){
        list<string>::iterator itr;
        for(itr=buffer.begin();itr != buffer.end();itr++){
            ofstream1<<*itr<<'\n';
        }
    }ofstream1.close();
}
bool LineEd::inputMode(){

    cout<<"?"<<'\t';
    getline(std::cin >> ws,input);
    if((input.length()==1 && input[0]=='.')) {
        return false;
    }
    if(input.size() > 0){
        buffer.push_back(input);
        // currentLine++; //TODO
        totalLines++;
    }
    return true;
}