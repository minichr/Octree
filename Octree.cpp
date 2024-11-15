#include <iostream>
#include <cstdio>
#include <vector>
#include <array>
#include <utility> //for pair
#include <fstream> //filehandling 
#include <string>

//Test Checking
#define filehandling 0
#define OTreeCreation 1
#define Tree 0
#define wee 0 

#define vec 0 //do not use vec without some edits

class OcTree{
    private:
        std::string prePattern;
        #if vec
        std::vector<std::pair<char, 
        std::vector<std::pair<char,
        std::vector<std::pair<char,
        std::vector<char> > >> >> > octree;
        #else
        std::array< std::pair<char, std::array
                  < std::pair<char, std::array
                  < std::pair<char, std::array
                  < char,8>>,8> >,8>>,8 > octree;
        #endif
        
        int bCnt;

    public:
        OcTree(std::string pattern);
        ~OcTree(); //for deletion of tree
        std::string getPattern() {return this->prePattern;}
        OcTree addTree(const OcTree& other); //add pattern and use that to initialize
        int count_blue() {return bCnt;}
};

OcTree::OcTree(std::string pattern){
    this->prePattern = pattern;
    this->bCnt = 0;
    int sz = this->prePattern.length();

    #if vec
    octree.resize(8);
    #endif
    //single node
    if(sz == 1){
        if(this->prePattern[0] == 'b'){
            bCnt = 4096;
            for(int i = 0; i < 8; i++){
                octree[i].first = 'b';
            }
        }
        else{ 
            for(int i = 0; i < 8; i++){
                octree[i].first = 'w';
            }
        }
    }
    //subdivide
    else{
        int cnt = 0;
        if(this->prePattern[cnt] == 'p'){ 
            cnt++;
            for(int i = 0; i < 8; i++){
                if(this->prePattern[cnt] == 'b'){
                    octree[i].first = 'b';
                    cnt++;
                    bCnt += 512;
                }
                else if(this->prePattern[cnt] == 'w'){
                    octree[i].first = 'w';
                    cnt++;
                }
                else{ //p
                    octree[i].first = 'p';
                    cnt++;
                    for(int j = 0; j < 8; j++){
                        if(this->prePattern[cnt] == 'b'){
                            octree[i].second[j].first = 'b';
                            cnt++;
                            bCnt += 64;
                        }
                        else if(this->prePattern[cnt] == 'w'){
                            octree[i].second[j].first = 'w';
                            cnt++;
                        }
                        else{//p
                            octree[i].second[j].first = 'p';
                            cnt++;
                            for(int k = 0; k < 8; k++){
                                if(this->prePattern[cnt] == 'b'){
                                    octree[i].second[j].second[k].first = 'b';
                                    cnt++;
                                    bCnt += 8;
                                }
                                else if(this->prePattern[cnt] == 'w'){
                                    octree[i].second[j].second[k].first = 'w';
                                    cnt++;
                                }
                                else{//p
                                    octree[i].second[j].second[k].first = 'p';
                                    cnt++;
                                    for(int m = 0; m < 8; m++){
                                        if(this->prePattern[cnt] == 'b'){
                                            octree[i].second[j].second[k].second[m] = 'b';
                                            cnt++;
                                            bCnt += 1;
                                        }
                                        else if(this->prePattern[cnt] == 'w'){
                                            octree[i].second[j].second[k].second[m] = 'w';
                                            cnt++;
                                        }
                                        //no more
                                    }
                                }
                            }
                        }
                    }
                }
            }
        } //removing that if
    }
    //end of fcn
}

OcTree::~OcTree(){
    //destroy
}


OcTree OcTree::addTree(const OcTree& other){
    
    std::string newPattern = "p";
    if(this->prePattern.size() == 1 || other.prePattern.size() == 1){
        if(this->prePattern[0] == 'b' || other.prePattern[0] == 'b'){
            return OcTree("b");
        }
        else if(this->prePattern[0] == 'w' && other.prePattern[0] == 'p'){
            return OcTree(other.prePattern);
        }
        else if(this->prePattern[0] == 'p' && other.prePattern[0] == 'w'){
            return OcTree(this->prePattern);
        }
        else if(this->prePattern[0] == 'w' && other.prePattern[0] == 'w'){
            return OcTree("w");
        }
    }
    
    else{ //if trees are not so simple
        for(int i = 0; i < 8; i++){
            if(this->octree[i].first == 'b' || other.octree[i].first == 'b'){
                newPattern += 'b';
            }
            else if(this->octree[i].first == 'w' && other.octree[i].first == 'w'){
                newPattern += 'w';
            }
            else{
                newPattern += 'p';
                if(this->octree[i].first == 'w' || other.octree[i].first == 'w'){
                    if(this->octree[i].first == 'p'){
                        for(int j = 0; j < 8; j++){
                            if(this->octree[i].second[j].first != 'p'){
                                newPattern += this->octree[i].second[j].first;
                            }
                            else{ //if p
                                newPattern += 'p';
                                for(int k = 0; k < 8; k++){
                                    if(this->octree[i].second[j].second[k].first != 'p'){
                                        newPattern += this->octree[i].second[j].second[k].first;
                                    }
                                    else{ // if p
                                        newPattern += 'p';
                                        for(int m = 0; m < 8; m++){
                                            newPattern += this->octree[i].second[j].second[k].second[m];
                                        }
                                    }
                                }
                            }
                        }
                    }
                    else if(other.octree[i].first == 'p'){
                        for(int j = 0; j < 8; j++){
                            if(other.octree[i].second[j].first != 'p'){
                                newPattern += other.octree[i].second[j].first;
                            }
                            else{ //if p
                                newPattern += 'p';
                                for(int k = 0; k < 8; k++){
                                    if(other.octree[i].second[j].second[k].first != 'p'){
                                        newPattern += other.octree[i].second[j].second[k].first;
                                    }
                                    else{ // if p
                                        newPattern += 'p';
                                        for(int m = 0; m < 8; m++){
                                            newPattern += other.octree[i].second[j].second[k].second[m];
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                else{ //else p both
                    for(int j = 0; j < 8; j++){
                        if(this->octree[i].second[j].first == 'b' || other.octree[i].second[j].first == 'b'){
                            newPattern += 'b';
                        }
                        else if(this->octree[i].second[j].first == 'w' && other.octree[i].second[j].first == 'w'){
                            newPattern += 'w';
                        }
                        else{ //one or both p
                            newPattern += 'p';
                            if(this->octree[i].second[j].first == 'w' || other.octree[i].second[j].first == 'w'){
                                if(this->octree[i].second[j].first == 'p'){
                                    for(int k = 0; k < 8; k++){
                                        if(this->octree[i].second[j].second[k].first != 'p'){
                                            newPattern += this->octree[i].second[j].second[k].first;
                                        }
                                        else{
                                            newPattern += 'p';
                                            for(int m = 0; m < 8; m++){
                                                newPattern += this->octree[i].second[j].second[k].second[m];
                                            }
                                        }
                                    }
                                }
                                else if(other.octree[i].second[j].first == 'p'){
                                    for(int k = 0; k < 8; k++){
                                        if(other.octree[i].second[j].second[k].first != 'p'){
                                            newPattern += other.octree[i].second[j].second[k].first;
                                        }
                                        else{
                                            newPattern += 'p';
                                            for(int m = 0; m < 8; m++){
                                                newPattern += other.octree[i].second[j].second[k].second[m];
                                            }
                                        }
                                    }
                                }
                            }
                            else{ //both p
                                for(int k = 0; k < 8; k++){
                                    if(this->octree[i].second[j].second[k].first == 'b' || other.octree[i].second[j].second[k].first == 'b'){
                                        newPattern += 'b';
                                    }
                                    else if(this->octree[i].second[j].second[k].first == 'w' && other.octree[i].second[j].second[k].first == 'w'){
                                        newPattern += 'w';
                                    }
                                    else{ //one or both p
                                        newPattern += 'p';
                                        //at lease one
                                        if(this->octree[i].second[j].second[k].first == 'p' && other.octree[i].second[j].second[k].first != 'p'){
                                            for(int m = 0; m < 8; m++){
                                                newPattern += this->octree[i].second[j].second[k].second[m];
                                            }
                                        }
                                        else if(other.octree[i].second[j].second[k].first == 'p' && this->octree[i].second[j].second[k].first != 'p'){ //yung isa pala lol
                                            for(int m = 0; m < 8; m++){
                                                newPattern += other.octree[i].second[j].second[k].second[m];
                                            }
                                        }
                                        else{ //both p
                                            for(int m = 0; m < 8; m++){
                                                if(this->octree[i].second[j].second[k].second[m] == 'b' || other.octree[i].second[j].second[k].second[m] == 'b'){
                                                    newPattern += 'b';
                                                }
                                                else{
                                                    newPattern += 'w';
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            //end of first for
        }
        //end of else
    }

    return OcTree(newPattern);
}


int main(void){
    //i did not define = sign for the addition
    // filehandling
    std::fstream fs("me_in2.txt");
    int N;
    std::string str1, str2;
    std::vector< std::pair< std::string, std::string > > imgPair;

    fs >> N;
    for(int i = 0; i < N; i++){
        fs >> str1;
        fs >> str2;
        imgPair.emplace_back( std::make_pair(str1, str2) );
    }

    #if filehandling
    std::cout << "fh test:\n";
    for(int i = 0; i < N; i++){
        std::cout << "pair " << i << '\n';
        std::cout << imgPair[i].first << '\n';
        std::cout << imgPair[i].second << '\n';
    } 
    #endif

    #if OTreeCreation
    for(int i = 0; i < N; i++){
        OcTree ocT1(imgPair[i].first);
        OcTree ocT2(imgPair[i].second);
        #if Tree
        std::cout << "Two Tree What " << i << '\n';
        std::cout << ocT1.getPattern() << " : " 
                  << ocT1.count_blue() << "\n";
        std::cout << ocT2.getPattern() << " : " 
                  << ocT2.count_blue() << "\n";
        #endif
        //std::cout << ocT1.addTree(ocT2).getPattern() << "\n";
        std::cout << ocT1.addTree(ocT2).count_blue() << "\n";

        
        
    } 
    
    #endif
    
    return 0;
}
