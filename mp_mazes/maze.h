/* Your code here! */
#pragma once
#include "cs225/PNG.h"
#include <vector>
#include "dsets.h"
using namespace cs225;
class SquareMaze{
    public:
        SquareMaze();
        void makeMaze(int width, int height);
        bool canTravel(int x, int y, int dir) const;
        void setWall(int x, int y, int dir, bool exists);
        std::vector<int> solveMaze();
        PNG* drawMaze() const;
        PNG* drawMazeWithSolution();
    private:
        int width_;
        int height_;
        std::vector<std::vector<std::vector<bool>>> maze;
        DisjointSets* cells;
        int numCells;

};