//
// Created by pooja on 05/06/19.
//

#include "LineEd.h"
#include <algorithm>
#include <string>
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
        //TODO create file if no file found
        int count = addToBuffer(filename);
        if (count >= 1) {
            cout << "\"" << f << "\"\t"<<count<< " lines\n";
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
        cout<<"Entering Command Mode\n";
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
    }
}
bool LineEd::runCommand(const string & cmdline) {
    //call command from here

    string cmdLine=cmdline;
  //  cout<<"Entering Command Mode"<<'\n';

    //exit command mode if  i
    do{

        cout<<'?'<<'\t';

        getline(cin >> ws,cmdLine);
        Command cmd(cmdLine);


        /*if(!cmd.getStatus()){
            break;
        }else */
        if(cmd.getSymbol()=="i" && cmd.getAddress1()!="." ){
            //Condition to insert at line x
            cout<<"Entering Input Mode ix"<<'\n';
            if(stoi(cmd.getAddress1())<=totalLines){
                while(true){
                    if(!inputAt(stoi(cmd.getAddress1())))break;
                }
                cout<<"Entering Command Mode"<<'\n';
            }else{
                cout<<"line range out of bound"<<'\n';
                break;
            }
        }else if(cmd.getSymbol()=="i" && cmd.getAddress1()=="."){
            //condition to input mode only
            //Enter input mode if i and exit input mode if  .
            cout<<"Entering Input Mode\n";
            while(true){
                if(!inputMode())break;
            }
            cout<<"Entering Command Mode\n";
        }else if((cmd.getSymbol()=="p" || cmd.getSymbol()=="." || cmd.getSymbol()=="$" )&& cmd.getStatus()) {
            if (buffer.empty()) {
                cout<<"File empty. Must use one of I, A, Q commands."<<'\n';
            } else {
                printBuffer(cmd);
            }
        }else if(cmd.getSymbol()=="a" && cmd.getStatus()){
            cout<<"Entering Input Mode\n";
            while(true){
                if(!inputMode())break;
            }cout<<"Entering Command Mode\n";
        }else if(cmd.getSymbol()=="w" && cmd.getStatus()){
            string question="Save changes to the file: " +filename+" (y or n)?";
            if(askYesNo(question)) {
                if (filename.empty()) {
                    cout << "Enter the name of a file to write to: ";
                    cin >> filename;
                }
                bufferToFile(filename);
                cout << buffer.size() << " lines written to file \"" << filename << "\"" << '\n';
            }
        }else if(cmd.getSymbol()=="g" && cmd.getStatus()){
            // cout<<"currentLine is : "<<cmd.getAddress1();
           g(cmd);
            break;
        }else if(cmd.getSymbol()=="u" && cmd.getStatus()){
            u(cmd);
            break;
        }else if(cmd.getSymbol()=="n" && cmd.getStatus()){
            n(cmd);break;
        }else if(cmd.getSymbol()=="d" && cmd.getStatus()){
            int x=stoi(cmd.getAddress1());
            int y=stoi(cmd.getAddress2());
            if(x<=0 && x>totalLines){
                cmd.setStatus(false);
            }
            if(y<=0 && y>totalLines){
                cmd.setStatus(false);
            }
            if(cmd.getStatus()){
                d(x,y); break;
            }
        }else if(cmd.getSymbol()=="j" && cmd.getStatus()){
            int x,y;

            if(cmd.getAddress1()=="$"){
                x=totalLines;
            }else if(cmd.getAddress1()=="."){
                x=currentLine;
            }else{
                x=stoi(cmd.getAddress1());
            }

            if(cmd.getAddress2()=="$"){
                y=totalLines;
            }else if(cmd.getAddress2()=="."){
                y=currentLine;
            }else{
                y=stoi(cmd.getAddress2());
            }

            if(x<=0 && x>totalLines){
                cmd.setStatus(false);
            }
            if(y<=0 && y>totalLines){
                cmd.setStatus(false);
            }
            if(cmd.getStatus()){
                c(x,y);
                break;
            }

        }else if(cmd.getSymbol()=="x" && cmd.getStatus()){

        }else if(cmd.getSymbol()=="c" && cmd.getStatus()){
            const int x=stoi(cmd.getAddress1());
            const int y=stoi(cmd.getAddress2());
            if(x<=0 && x>totalLines){
                cmd.setStatus(false);
            }
            if(y<=0 && y>totalLines){
                cmd.setStatus(false);
            }
            if(cmd.getStatus()){
                c(x,y);
                break;
            }
        }else if(cmd.getSymbol()=="v" && cmd.getStatus()){

        }else if(tolower(cmdLine[0])=='q'){
            return false;//return false only if symbol is "q"
        }
    }while (tolower(cmdLine[0])!='q');
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
    ifstream in;
    in.open(file, std::ofstream::in);
    int count=0;
    if(in.is_open()){
        // Read the next line from File until it reaches the end.
        while (getline(in, str)){
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
        to=this->currentLine;
    }else if(cmd.getAddress2()=="$"){
        to=this->totalLines;
    } else{
        to=stoi(cmd.getAddress2());
    }

    if(!buffer.empty())
    {
        int linecount=from;
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
/**
 *      command w
 * save the buffer to file
 * Also, used after command quit when user wants to save the content to file.
 * @param filename  name of the file where buffer is copied to
 * @return void
 * */
void LineEd::bufferToFile(const string & filename){
    ofstream ofstream1;
   // ofstream1.open("/home/meet/pooja-workspace/CLionWorkspace/Ass2/"+filename, std::ofstream::app);
    ofstream1.open(filename, std::ofstream::app);
    if (ofstream1){
        list<string>::iterator itr;
        for(itr=buffer.begin();itr != buffer.end();itr++){
            ofstream1<<*itr<<'\n';
        }
    }ofstream1.close();
}
/**
 *      command i (when no addresses provided)
 *      command a
 * starts the input mode and the content is added to buffer.
 * Also, used to append the content to the buffer.
 * @return bool  return false if single "." is given as input else true.
 * */
bool LineEd::inputMode(){
    //exit input mode if  "."(dot)
    cout<<"?"<<'\t';
    getline(std::cin >> ws,input);
    if((input.length()==1 && input[0]=='.')) {
        return false;
    }
    if(input.size() > 0){
        buffer.push_back(input);
        ++currentLine;
        ++totalLines;
    }
    return true;
}
/**
 *      command: ix
 * input or insert text at line x in the buffer. ignores address 2
 * */
bool LineEd::inputAt(const int& x){
    cout<<"?"<<'\t';
    int linecount=1;
    getline(std::cin >> ws,input);
    if((input.length()==1 && input[0]=='.')) {
        return false;
    }else if(buffer.size()!=0)
    {   // traverse buffer contents upto x-1

        list<string>::iterator it = buffer.begin();
        while(linecount<x){
            ++it;
            ++linecount;
        }
        //insert at line x
        if(linecount==x){
            buffer.insert(it,"\t"+input);
            currentLine=x;
            ++totalLines;
        }

    }
    return true;
}
void LineEd::u(Command& cmd){

    if(currentLine==1){
        cout<<"BOF reached\n";
    } else{
        --currentLine;
        printBuffer(cmd);
    }
}
void LineEd::n(Command& cmd){

    if(currentLine==totalLines){
        cout<<"EOF reached\n";
    } else{
        ++currentLine;
        printBuffer(cmd);
    }
}
void LineEd::g(Command& cmd){

    if(cmd.getAddress1()!="." && (stoi(cmd.getAddress1()))>0 && (stoi(cmd.getAddress1()))<=totalLines){
        currentLine=stoi(cmd.getAddress1());
        printBuffer(cmd);
    }else{
        cout<<"line out of range\n";
    }
}
void LineEd::d(const int& x, const int & y){
    //dx,y
    //sets the currentline
    if(buffer.size()<=0){   //if buffer is empty
        currentLine=0;  //currentline is undefined
    }else if (y!=totalLines){   // if the line exists after the deleted line range
        currentLine=((y-x)+1);  // set the next existing line after delete to currentline
    }else{                      // if line range deletes the last line of file
        currentLine=x-1;        //set the currentline to the line before the line range
    }
    //deletes the supplied line range
    if(buffer.size()>0)
    {   // delete the buffer contents
        list<string>::iterator itx,ity;
        itx=ity= buffer.begin();
        advance(itx,x-1);
        advance(ity,y);
        buffer.erase(itx,ity);
        totalLines-=((y-x)+1);  // set the totallines
    }
}
void LineEd::c(const int& x, const int& y){
    cout<<"change  \?";
    string toBeChanged{};
    getline(cin,toBeChanged);
    cout<<"to  \?";
    string changeTo{};
    getline(cin,changeTo);
    list<string>::iterator itx,ity;
    itx=ity=buffer.begin();
    advance(itx,x-1);
    advance(ity,y);
    while (itx!=ity){

        string temp=*itx;
        int index=0;

        while(true){
            index = temp.find(toBeChanged, index);
            if (index == string::npos) break;//break if not found
            temp.replace(index, changeTo.size(), changeTo);
            index += changeTo.size();
        }
        *itx=temp;
        itx++;
    }
}