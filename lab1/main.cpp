#include <QImage>
#include <QColor>
#include <iostream>

using namespace std;

int main(int argc, char** argv) {
    /* 
      Prompt user for 3 points separated by whitespace.

      Part 1:
          You'll need to get the x and y coordinate as floating point values
          from the user for 3 points that make up a triangle.

      Part 3:
          You'll need to also request 3 colors from the user each having
          3 floating point values (red, green and blue) that range from 0 to 1.
    */

    // create an image 640 pixels wide by 480 pixels tall
    // with a standard 32 bit red, green, blue format
    QImage image(640, 480, QImage::Format_RGB32); 

    /* 
      Part 1:
          Calculate the bounding box of the 3 provided points and loop
          over each pixel in that box and set it to white using:

          image.setPixel(x,y, qRgb(255,255,255));

      Part 2:
          Modify your loop from part 1. Using barycentric coordinates, 
          determine if the pixel lies within the triangle defined by 
          the 3 provided points. If it is color it white, otherwise
          move on to the next pixel.

          For more on barycentric coordinates:
          http://en.wikipedia.org/wiki/Barycentric_coordinate_system

      Part 3:
          For each pixel in the triangle, calculate the color based on
          the calculated barycentric coordinates and the 3 provided
          colors. Your colors should have been entered as floating point
          numbers from 0 to 1. Since we have a 32 bit image, the red, 
          green and blue components range from 0 to 255. Be sure to make
          the conversion.
    */

    if(image.save("triangle.jpg",0,100)) {
        cout << "Output triangle.jpg" << endl;
    } else {
        cout << "Unable to save triangle.jpg" << endl;
    }
}
