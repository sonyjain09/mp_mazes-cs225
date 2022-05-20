/* Your code here! */
#pragma once
#include <vector>
class DisjointSets{
    public:
        void addelements(int num);
        int find(int elem);
        void setunion(int a, int b);
        int size(int elem);
        std::vector<int>& getUptrees();
        void clearUptrees();
    private:
        std::vector<int> uptrees;
};
