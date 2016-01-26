#include <QImage>
#include <QColor>
#include <iostream>
#include <boost/algorithm/string.hpp>

using namespace std;

/**
 * This method determines the determinante of Martrix T
 */
double getDeterminent(int x1, int x2, int x3, int y1, int y2, int y3)
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
double getLambdaOne(int x, int y, int x1, int x2, int x3, int y1, int y2, int y3, double det)
{
    return ((y2 - y3) * (x - x3) + (x3 - x2) * (y - y3)) / det;
}

/**
 * This method calculates lambda 2
 */
double getLambdaTwo(int x, int y, int x1, int x2, int x3, int y1, int y2, int y3, double det)
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
    cout << "Enter 3 points in form x,y:r,g,b:" << endl;
    string inputs[3];
    cin >> inputs[0];
    cin >> inputs[1];
    cin >> inputs[2];

    double values[3][5];

    for(int k = 0; k < 3; k++)
    {
        vector<string> strs, points, colors;
        boost::split(strs, inputs[k], boost::is_any_of(":"));
        boost::split(points, strs[0], boost::is_any_of(","));
        boost::split(colors, strs[1], boost::is_any_of(","));
        values[k][0] = ::atof(points[0].c_str());
        values[k][1] = ::atof(points[1].c_str());
        values[k][2] = ::atof(colors[0].c_str());
        values[k][3] = ::atof(colors[1].c_str());
        values[k][4] = ::atof(colors[2].c_str());

        cout << "(" << values[k][0] << ", " << values[k][1] << "), R:" << values[k][2] << ", G:" << values[k][3] << ", B:" << values[k][4] << endl;
    }

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

    int x1 = values[0][0];
    int x2 = values[1][0];
    int x3 = values[2][0];
    int y1 = values[0][1];
    int y2 = values[1][1];
    int y3 = values[2][1];
    int minX = min(min(x1, x2), x3);
    int maxX = max(max(x1, x2), x3);
    int minY = min(min(y1, y2), y3);
    int maxY = max(max(y1, y2), y3);
    double det = getDeterminent(x1, x2, x3, y1, y2, y3);

    double r1 = values[0][2];
    double r2 = values[1][2];
    double r3 = values[2][2];
    double g1 = values[0][3];
    double g2 = values[1][3];
    double g3 = values[2][3];
    double b1 = values[0][4];
    double b2 = values[1][4];
    double b3 = values[2][4];

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

                double colorR = (r1 * lambda1 + r2 * lambda2 + r3 * lambda3) * 255;
                double colorG = (g1 * lambda1 + g2 * lambda2 + g3 * lambda3) * 255;
                double colorB = (b1 * lambda1 + b2 * lambda2 + b3 * lambda3) * 255;

                if (lambda1 > 0 && lambda2 > 0 && lambda3 > 0)
                {
                    image.setPixel(i,j, qRgb(colorR,colorG,colorB));
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
