#include "Topograph.h"

int main()
{
    string fileName0 = "map.bmp";
    string fileName1 = "map2.bmp";
    
    Topograph picture("map_input.txt");
    Bmp item(picture.getHeight(),picture.getWidth());
    
    picture.drawMap(item);
    system(("eog " + fileName0 + " &").c_str());
    
    picture.drawBestPath(item);
    item.write("map2.bmp");
    system(("eog " + fileName1 + " &").c_str());
    
}
