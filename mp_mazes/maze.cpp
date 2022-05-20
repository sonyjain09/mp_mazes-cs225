/* Your code here! */
#include "maze.h"
#include <queue>
#include <algorithm>

SquareMaze::SquareMaze(){
    int width_ = 0;
    int height_ = 0;
    int numCells = 0;
}

void SquareMaze::makeMaze(int width, int height){
    //if the maze already exists delete it
    cells = new DisjointSets();
    //construct a maze with all the walls in place
    maze.resize(height);
    std::vector<bool> fill = {true,true};
    for(int i = 0; i < height; i++) maze[i].resize(width,fill);
    width_ = width;
    height_ = height;
    numCells = width_*height_;
    cells->addelements(numCells);
    //repeat until every cell is connected to the maze (all one disjoint set)
    while(cells->size(0) != numCells){
        //randomly select walls to delete. each cell has a right wall and bottom wall
        int cell1 = rand() % numCells;
        //0 is right, 1 is below
        int wall = rand() % 2;
        //get the coordinates of cells
        int x = cell1/width_;
        int y = cell1%width_;
        //set cell2
        int cell2 = (wall == 0) ? cell1+1 : cell1 + width_;
        //if the wall is on the perimeter or there will be a cycle(in the same disjoint set) dont't delete wall
        if ((x == height_-1 and wall == 1) or (y == width_-1 and wall == 0)) continue;
        if(cells->find(cell1) == cells->find(cell2)) continue;
        //delete the wall 
        setWall(y,x,wall,false);
        //merge two sets
        cells->setunion(cell1,cell2);
    }
}


void SquareMaze::setWall(int x, int y, int dir, bool exists){
    maze[y][x][dir] = exists;
}


bool SquareMaze::canTravel(int x, int y, int dir) const{
    //right
    if(dir == 0) return (x == width_ -1) ? false : !(maze[y][x][0]);
    //down
    else if(dir == 1) return (y==height_-1) ? false : !(maze[y][x][1]); 
    //left
    else if(dir == 2) return (x == 0) ? false : !(maze[y][x-1][0]);
    //up
    else return (y == 0) ? false : !(maze[y-1][x][1]);
}


std::vector<int> SquareMaze::solveMaze(){
    //make a queue for cells
    std::queue<int> q;
    //make a visited vector
    std::vector<std::vector<bool>> visited;
    //make a vector that stores each cells previous cell
    std::vector<std::vector<int>> previous;
    //make a vector that stores the direction that is taken to go into each cell
    std::vector<std::vector<int>> directions;
    //set dimensions of all vectors
    visited.resize(height_);
    previous.resize(height_);
    directions.resize(height_);
    for(int i = 0; i < height_; i++){
        visited[i].resize(width_);
        previous[i].resize(width_);
        directions[i].resize(width_);
    }
    //push the first cell to the queue and mark it as visited
    q.push(0);
    visited[0][0] = true;
    //loop through while q isnt empty
    while(!q.empty()){
        //get the front
        int cell = q.front();
        //coordinates of front
        int x = cell/width_;
        int y = cell%width_;
        //remove the front
        q.pop();
        //add neighbors to queue 
        //check if they can travel because only want to add possible directions to queue
        //check that they haven't been visited yet because don't want to go backwards
        //check right
        if(canTravel(y,x,0) and !visited[x][y+1]){
            visited[x][y+1] = true;
            directions[x][y+1] = 0;
            previous[x][y+1] = cell;
            q.push(cell+1);
        }
        //check down
        if(canTravel(y,x,1) and !visited[x+1][y]){
            visited[x+1][y] = true;
            directions[x+1][y] = 1;
            previous[x+1][y] = cell;
            q.push(cell+width_);
        }
        //check left
        if(canTravel(y,x,2) and !visited[x][y-1]){
            visited[x][y-1] = true;
            directions[x][y-1] = 2;
            previous[x][y-1] = cell;
            q.push(cell-1);
        }
        //check up
        if(canTravel(y,x,3) and !visited[x-1][y]){
            visited[x-1][y] = true;
            directions[x-1][y] = 3;
            previous[x-1][y] = cell;
            q.push(cell-width_);
        }
    }
    //make variables for the currpath we're going through and max path we will return
    std::vector<int> curr;
    std::vector<int> max;
    //go through every cell at the bottom row of the maze (across the width_)
    //only one possible path to each cell in bottom row so only need to check path to each cell once
    for(int i=0;i<width_;i++){
        //get the current cell number
		int currCell = i+((height_-1)*width_);
        int destX = currCell/width_;
        int destY = currCell%width_;
        //clear the path from previous use
		curr.clear();
        //go until we've gotten to the first cell
		while(currCell !=0){
            //use insert because should go at the beginning since we're going from end to front
            curr.insert(curr.begin(),directions[currCell/width_][currCell%width_]);
            //update curr cell to be the previous one
			currCell=previous[currCell/width_][currCell%width_];
		}
        //if needed change the max path
        //don't check equal to because we check the ones with lowest y indices first so has to be 
        //strictly greater
		if(curr.size()>max.size()){
            max = curr;
        }
	}
    //return the max path
    return max;
}


PNG* SquareMaze::drawMaze() const{
    //First, create a new PNG. Set the dimensions of the PNG to (width*10+1,height*10+1)
    PNG* png_ = new PNG((width_*10)+1, (height_*10)+1);
    //Blacken the entire topmost row and leftmost column of pixels, except the entrance (1,0) through (9,0). 
    for(int i = 10; i < (width_*10)+1; i++){
        cs225::HSLAPixel& pixel = png_->getPixel(i,0);
        pixel.l = 0;
    }
    for(int i = 0; i < (height_*10)+1; i++){
        cs225::HSLAPixel& pixel = png_->getPixel(0,i);
        pixel.l = 0;
    }
    //For each square in the maze, call its maze coordinates (x,y). 
    for(int i = 0; i < height_; i++){
        for(int j = 0; j < width_; j++){
            std::vector<bool> temp = maze[i][j];
            //If the right wall exists, then blacken the pixels with coordinates ((x+1)*10,y*10+k) for k from 0 to 10. 
            if(temp[0]){
                for(int k = 0; k < 11; k++){
                    cs225::HSLAPixel& pixel = png_->getPixel((j+1)*10,i*10+k);
                    pixel.l = 0;
                }
            }
            //If the bottom wall exists, then blacken the pixels with coordinates (x*10+k, (y+1)*10) for k from 0 to 10.
            if(temp[1]){
                for(int k = 0; k < 11; k++){
                    cs225::HSLAPixel& pixel = png_->getPixel(j*10+k,(i+1)*10);
                    pixel.l = 0;
                }
            }
        }
    }
    return png_;
}


PNG* SquareMaze::drawMazeWithSolution(){
    //call drawMaze (original image)
    PNG* png_ = drawMaze();
    //cal  solveMaze (solution vector)
    std::vector<int> solution = solveMaze();
    //Start at pixel (5,5). 
    int i = 5; 
    int j = 5; 
    //Continue in this manner until you get to the end of the solution vector
    for(unsigned k = 0; k < solution.size(); k++){
        int direction = solution[k];
        //update i and j accordingly
        //(Red is 0,1,0.5,1 in HSLA).
        //If the step is downward, color pixels (i,j) through (i,j+10) red.
        if(direction == 1){
            for(int x = 0; x < 11; x++){
                cs225::HSLAPixel& pixel = png_->getPixel(i,j+x);
                pixel.h = 0;
                pixel.s = 1;
                pixel.l = 0.5;
                pixel.a = 1;
            }
            j += 10;
        } 
        //if the step is right, color pixels (i,j) through (i+10,j) red. 
        else if (direction == 0){
            for(int x = 0; x < 11; x++){
                cs225::HSLAPixel& pixel = png_->getPixel(i+x,j);
                pixel.h = 0;
                pixel.s = 1;
                pixel.l = 0.5;
                pixel.a = 1;
            }
            i += 10;
        }
        //if the step is up, color pixels (i,j) through (i,j-10)
        else if(direction == 3){
            for(int x = 0; x < 11; x++){
                cs225::HSLAPixel& pixel = png_->getPixel(i,j-x);
                pixel.h = 0;
                pixel.s = 1;
                pixel.l = 0.5;
                pixel.a = 1;
            }
            j -= 10;
        }
        //if the step is left, color pixels (i,j) through (i-10,j) red
        else{
            for(int x = 0; x < 11; x++){
                cs225::HSLAPixel& pixel = png_->getPixel(i-x,j);
                pixel.h = 0;
                pixel.s = 1;
                pixel.l = 0.5;
                pixel.a = 1;
            }
            i -= 10;
        }
    }
    //already drawn to the middle of the destination cell so just need to go to bottom and beginning of 
    //where we're at
    int x = i-5;
    int y = j+5;
    //whiten the pixels at the bottom (y) from i to i+10
    for(int k = 1; k < 10; k++){
        cs225::HSLAPixel & pixel = png_->getPixel(x+k,y);
        pixel.l = 1;
    }
    return png_;
}