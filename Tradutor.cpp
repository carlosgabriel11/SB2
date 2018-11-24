#include <iostream>
#include <cstdlib>
#include <string>
#include <cstdio>

using namespace std;

//prototype of functions
string getFileName(const string&);
void directiveData(const string&);
void instructions(const string&);

//start of the main function
int main(int argc, char** argv){
    //if there is no argument
    if(argc == 1){
        cout << "NO FILE INPUT" << endl;
        //leave the program
        exit(1);
    }

    directiveData(argv[1]);
    instructions(argv[1]);

    return 0;
}

//the function to get the name of the file without the asm
string getFileName(const string& str){
    //a position variable
    size_t pos = str.find('.');
    //a regular counter
    unsigned int counter;
    //the file name
    string fileName;

    for(counter = 0; counter < pos; counter++){
        fileName.push_back(str[counter]);
    }

    return fileName + ".s";
}

//a function to get a line of the file
string getFileLine(FILE* fp){
    //an auxiliar char
    char chAux = '\0';
    //the line
    string line;

    while(chAux != EOF){
        //get the character
        chAux = fgetc(fp);
        //put in the line
        line.push_back(chAux);

        //check if it is in the end of the file
        if((chAux == EOF) || (chAux == '\0')){
            break;
        }

        //end of the line
        if(chAux == '\n'){
            return line;
        }
    }

    return "";
}

//a function to get the label
string getLabel(const string& line){
    //an position variable
    size_t pos;
    //a regular counter
    unsigned int counter;
    //the label
    string label;

    //get the first position that isn't space or tab
    for(counter = 0; ; counter++){
        if((line[counter] != ' ') && (line[counter] != '\t')){
            pos = counter;
            break;
        }
    }

    for(counter = pos; counter < line.find(':'); counter++){
        label.push_back(line[counter]);
    }

    return label;
}

//a function to get the const value
string getConstValue(const string& line){
    //a position variable
    size_t pos = line.find("CONST");
    //a regular counter
    unsigned int counter;
    //the const value
    string value;

    //get the first position of the const value
    for(counter = pos + 5; ; counter++){
        if((line[counter] != ' ') && (line[counter] != '\t')){
            pos = counter;
            break;
        }
    }

    //get the const value
    for(counter = pos; counter < line.size() - 1; counter++){
        value.push_back(line[counter]);
    }

    return value;
}

//a function to get the space value
string getSpaceValue(const string& line){
    //a position variable
    size_t pos = line.find("SPACE");
    //a regular counter
    unsigned int counter;
    //the const value
    string value;

    //get the first position of the space value
    for(counter = pos + 5; ; counter++){
        if(line[counter] == '\n'){
            return "1";
        }

        if((line[counter] != ' ') && (line[counter] != '\t')){
            pos = counter;
            break;
        }
    }

    //get the const value
    for(counter = pos; counter < line.size() - 1; counter++){
        value.push_back(line[counter]);
    }

    return value;
}

//the function to write on the output file the section data part
void directiveData(const string& inputFile){
    //the file variable to read the input file
    FILE* src = fopen(inputFile.c_str(), "r");
    //the file variable to write on the output file
    FILE* dst = fopen(getFileName(inputFile).c_str(), "w");
    //each line of the file
    string line = "\n";
    //a flag to indicate that the program is in the section data
    bool flag_data = false;
    //a flag to inficate that the program is in the section text
    bool flag_text = false;
    //a flag to indicate that the program is in the section bss
    bool flag_bss = false;
    //general flag
    bool flag = false;
    //a label
    string label;

    //the loop of this part of the program
    while(line != ""){
        line = getFileLine(src);

        if(line == ""){
            break;
        }

        //detected the section data
        if(line == "SECTION DATA\n"){
            if(flag){
                fputs("\n", dst);
            }
            else{
                flag = true;
            }

            fputs("section .data\n", dst);
            flag_text = false;
            flag_bss = false;
            flag_data = true;

            continue;
        }
        //detected the section bss
        if(line == "SECTION BSS\n"){
            if(flag){
                fputs("\n", dst);
            }
            else{
                flag = true;
            }
            fputs("section .bss\n", dst);
            flag_data = false;
            flag_text = false;
            flag_bss = true;

            continue;
        }
        //detected the section text
        if(line == "SECTION TEXT\n"){
            flag_data = false;
            flag_bss = false;
            flag_text = true;
        }

        //if it is in the section text, go to next iteration
        if(flag_text){
            continue;
        }

        //if it is in the section data
        if(flag_data){
            //put the const value in the file
            fputs(getLabel(line).c_str(), dst);
            fputs(" dd ", dst);
            fputs(getConstValue(line).c_str(), dst);
            fputs("\n", dst);

            continue;
        }

        if(flag_bss){
            //put the space value in the file
            fputs(getLabel(line).c_str(), dst);
            fputs(" resd ", dst);
            fputs(getSpaceValue(line).c_str(), dst);
            fputs("\n", dst);

            continue;
        }

    }

    fputs("\n", dst);

    //close the files
    fclose(src);
    fclose(dst);
}

//the function that converts the assembly into IA-32
void instructions(const string& inputFile){
    //the file variable to read the input file
    FILE* src = fopen(inputFile.c_str(), "r");
    //the file variable to write on the output file
    FILE* dst = fopen(getFileName(inputFile).c_str(), "w");
    //each line of the file
    string line = "\n";
    //a flag to inficate that the program is in the section text
    bool flag_text = false;
    //a label
    string label;
    //a flag to label
    bool flag_label = false;
    //a flag to check if it is a vector
    bool flag_vec = false;

    //start of reading the instructions
    while(line != ""){
        line = getFileLine(src);

        if(line == ""){
            break;
        }

        //section data detected
        if(line == "SECTION DATA\n"){
            flag_text = false;
            continue;
        }

        //section bss detected
        if(line == "SECTION BSS\n"){
            flag_text = false;
            continue;
        }

        //detected the section data
        if(line == "SECTION TEXT\n"){
            flag_text = true;
            continue;
        }

        //analyze the instructions
        if(flag_text){
            //the add instruction
            if(line.find("ADD") != string::npos){

            }

            //the sub instruction
            if(line.find("SUB") != string::npos){

            }

            //the mult instruction
            if(line.find("MULT") != string::npos){

            }

            //the div instruction
            if(line.find("DIV") != string::npos){

            }

            //the jmp instruction
            if(line.find("JMP") != string::npos){

            }

            //the jmpn instruction
            if(line.find("JMPN") != string::npos){

            }

            //the jmpp instruction
            if(line.find("JMPP") != string::npos){

            }

            //the jmpz instruction
            if(line.find("JMPZ") != string::npos){

            }

            //the copy instruction
            if(line.find("COPY") != string::npos){

            }

            //the load instruction
            if(line.find("LOAD") != string::npos){

            }

            //the store instruction
            if(line.find("STORE") != string::npos){

            }

            //the input instruction
            if(line.find("INPUT") != string::npos){

            }

            //the output instruction
            if(line.find("OUTPUT") != string::npos){

            }

            //the c_input instruction
            if(line.find("C_INPUT") != string::npos){

            }

            //the c_output instruction
            if(line.find("C_OUTPUT") != string::npos){

            }

            //the s_input instruction
            if(line.find("S_INPUT") != string::npos){

            }

            //the s_output instruction
            if(line.find("S_OUTPUT") != string::npos){

            }

            //the stop instruction
            if(line.find("STOP") != string::npos){

            }
        }
    }


    //closing the files
    fclose(src);
    fclose(dst);
}
