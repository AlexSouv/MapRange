#include "Topograph.h"

Topograph::Topograph(string fileName)
{
    ifstream ifs;
    
    string ncols;
    string nrows;
    
    string xllCorner;
    double xllNumber;
    
    string yllCorner;
    double yllNumber;
    
    string cellSize;
    double cellSizeNumber;
    
    ifs.open(fileName);
    
    ifs >> ncols >> width;
    ifs >> nrows >> height;
    
    ifs >> xllCorner >> xllNumber;
    ifs >> yllCorner >> yllNumber;
    
    ifs >> cellSize >> cellSizeNumber;
    
    //resizing first vector to width;
    v.resize(height);
    for(int i = 0; i < v.size(); i++)
    {
        //resizing the vector within the vector to height
        v[i].resize(width);
        for(int j = 0; j < width; j++)
        {
            ifs >> v.at(i).at(j);
        }
        
    }
}

void Topograph::drawMap(Bmp& bmp)
{
    findMin();
    findMax();
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            bmp(i,j).r = mapRange(v.at(i).at(j), min, max, 0, 255);
            bmp(i,j).g = bmp(i,j).r;
            bmp(i,j).b = bmp(i,j).r;
        }
    }
}

int Topograph::mapRange(int n, int fromLow, int fromHigh, int toLow, int toHigh)
{
    int newScale = 0;
    double rgbDistance;
    double nMinusLow;
    double highMinusLow;

    rgbDistance = toLow + toHigh;   //0 + 255 = 255;
    nMinusLow = n - fromLow;
    highMinusLow = fromHigh - fromLow; //Taking the max height and minimum height and subtracting them.
    newScale = toLow + (rgbDistance * (nMinusLow / highMinusLow)); // Putting all above into the main equation
    //Scale = 0 + (255 * (nMinusLow / highMinusLow));
    return newScale;
}

void Topograph::findMin()
{
    min = v.at(0).at(0);
    for(int i = 0; i < height; i++)
    {
       for(int j = 0; j < width; j++)
       {
           if(v.at(i).at(j) < min)
           {
               min = v.at(i).at(j);
           }
       }
    }
}
void Topograph::findMax()
{
    max = v.at(0).at(0);
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            if(v.at(i).at(j) > max)
            {
                max = v.at(i).at(j);
            }
        }
    }
}

//PART 2
///Draw one greedy path
///call moveForward to advance (i,j)
///set each bmp(i,j) to color
///returns the total sum of the elevation change
int Topograph::drawGreedyPath(Bmp& bmp, int startingRow, rgb color)
{
    int row = startingRow;
    int totalSum = 0;
    int elevationValue = v.at(row).at(0);
    
    for(int j = 0; j < width - 1;)
    {
        moveForward(row,j);
        
        totalSum += fabs(elevationValue - v.at(row).at(j)); //Keeping a running sum for elevation change
        
        elevationValue = v.at(row).at(j);       //Assigning the new value as elevation change;
        
        //Assigning that bmp at row,j as red
        bmp(row,j).r = color.r;
        bmp(row,j).g = color.g;
        bmp(row,j).b = color.b;
    }
    return totalSum;
}

void Topograph::drawBestPath(Bmp& bmp)
{
    rgb color;
    color.r = 255;
    color.b = 0;
    color.g = 0;
    int bestPath;
    int currentPath;
    bestPath = drawGreedyPath(bmp,0,color);
    int bestIncrement = 0;
    for(int i = 0; i < height; i++)
    {
        currentPath = drawGreedyPath(bmp,i,color);
        if(currentPath < bestPath)   //Doing currentPath > bestPath gives the largest change in elevation LOL
        {
            bestPath = currentPath;
            bestIncrement = i;
        }
    }
    //Assigns the best path as blue (0,255,0)
    color.r = 0;
    color.b = 255;
    color.g = 0;
    drawGreedyPath(bmp, bestIncrement, color);
    cout << "Best path: " << bestPath << endl;
}
void Topograph::moveForward(int& i, int& j)
{
    int Up;
    int Forward;
    int Down;
    
    int currentPlace = v.at(i).at(j);
    
    //If there is no up value
    if((i - 1) == -1)
    {
        Forward = (currentPlace - v.at(i).at(j + 1));
        Down = (currentPlace - v.at(i + 1).at(j + 1));
        
        //If Forward < Down OR Forward == Down, move forward
        if((Forward < Down) || (Forward == Down))
        {
            j = j + 1;
        }
        //If Down< Forward, move down
        else if(Down < Forward)
        {
            i = i + 1;
            j = j + 1;
        }
    }
    
    //If there is no bottom option(moving down)
    else if((i + 1) >= height)
    {
        Up = (currentPlace - v.at(i - 1).at(j + 1));
        Forward = (currentPlace - v.at(i).at(j + 1));
        
        //If Forward < Up OR Forward == Up, Forward has priority, move forward
        if((Forward < Up) || (Forward == Up))
        {
            j = j + 1;
        }
        //If A<B, A has priority, move upward
        else if(Up < Forward)
        {
            i = i - 1;
            j = j + 1;
        }
    }
    
    //If there is a upward and downward option
    else
    {
        Up = fabs(currentPlace - v.at(i - 1).at(j + 1));
        Forward = fabs(currentPlace - v.at(i).at(j + 1));
        Down = fabs(currentPlace - v.at(i + 1).at(j + 1));
        
        //(Forward == Up AND Forward < Up) OR (Forward == Down AND Forward < Up) OR (Forward < Up AND Forward < Down) || (Forward == Up and Down), move forward
        if(((Forward == Up) && (Forward < Down)) ||
           ((Forward == Down) && (Forward < Up)) ||
           ((Forward < Up) && (Forward < Down)) ||
           ((Forward == Up) && (Forward == Down)))
        {
            j = j + 1;
        }
        //(C=A AND C<B) OR (C=B AND C<A) OR (C<A AND C<B), move downward
        else if(((Down == Up) && (Down < Forward)) ||
                ((Down == Forward) && (Down < Up)) ||
                ((Down < Up) && (Down < Forward)))
        {
            i = i + 1;
            j = j + 1;
        }
        //A<B AND A<C, move upward
        else if((Up < Forward) && (Up < Down))
        {
            i = i - 1;
            j = j + 1;
        }
    }
}
