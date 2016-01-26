#include <QImage>
#include <QColor>
#include <iostream>

using namespace std;

/**
 * This method determines the determinante of Martrix T
 */
int getDeterminent(int x1, int x2, int x3, int y1, int y2, int y3)
{
    int a = x1 - x3;
    int b = x2 - x3;
    int c = y1 - y3;
    int d = y2 - y3;

    return a*d - b*c;
}

/**
 * This method calculates lambda 1
 */
double getLambdaOne(int x, int y, int x1, int x2, int x3, int y1, int y2, int y3, int det)
{
    return ((y2 - y3) * (x - x3) + (x3 - x2) * (y - y3)) / det;
}

/**
 * This method calculates lambda 2
 */
double getLambdaTwo(int x, int y, int x1, int x2, int x3, int y1, int y2, int y3, int det)
{
    return ((y3 - y1) * (x - x3) + (x1 - x3) * (y - y3)) / det;
}

/**
 * This method calculates lambda 3
 */
double getLambdaThree(double lambda1, double lambda2)
{
    return 1 - lambda1 - lambda2;
}

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

    cout << "starting" << endl;

    int x1 = 22;
    int x2 = 22;
    int x3 = 125;
    int y1 = 33;
    int y2 = 236;
    int y3 = 236;
    int minX = 22;
    int maxX = 125;
    int minY = 33;
    int maxY = 236;
    cout << "before det" << endl;
    int det = getDeterminent(x1, x2, x3, y1, y2, y3);
    cout << "after det" << endl;

    for (int i = 0; i < 640; i++)
    {
        if (i < minX || i > maxX) continue;

        for (int j = 0; j < 480; j++)
        {
            if (j >= minY && j <= maxY)
            {
                double lambda1 = getLambdaOne(i, j, x1, x2, x3, y1, y2, y3, det);
                double lambda2 = getLambdaTwo(i, j, x1, x2, x3, y1, y2, y3, det);
                double lambda3 = getLambdaThree(lambda1, lambda2);

                if (lambda1 > 0 && lambda2 > 0 && lambda3 > 0)
                {
                    cout << "setting a pixel" << endl;
                    image.setPixel(i,j, qRgb(255,255,255));
                }
            }
        }
    }

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
