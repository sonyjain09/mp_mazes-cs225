/* Your code here! */
#include "dsets.h"
void DisjointSets::addelements(int num){
    for(int i = 0; i < num; i++) uptrees.push_back(-1);
}
int DisjointSets::find(int elem){
    if(uptrees[elem] < 0) return elem;
    else return find(uptrees[elem]);
}
void DisjointSets::setunion(int a, int b){
    a = find(a);
    b = find(b);
    int newSize = uptrees[a] + uptrees[b];
    if(uptrees[a] < uptrees[b]){
        uptrees[b] = a;
        uptrees[a] = newSize;
    }
    else{
        uptrees[a] = b;
        uptrees[b] = newSize;
    }
}
int DisjointSets::size(int elem){
    return -1 * uptrees[find(elem)];
}

std::vector<int>& DisjointSets::getUptrees(){
    return uptrees;
}

