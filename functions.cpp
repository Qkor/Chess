#include <string>

std::string lan2coord(std::string in){
    std::string out="";
    out+=56-in[1];
    out+=in[0]-97;

    if(in.size()>3){
        out+=56-in[3];
        out+=in[2]-97;
    }
    //if(in.size()==3)
    //    out+=in[2];
    else if(in.size()==5){
        if(in[3]=='8')
            out+=in[4]-32;
        else
            out+=in[4];
    }
        

    return out;
}