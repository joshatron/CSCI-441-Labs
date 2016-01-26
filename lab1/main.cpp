#include <QImage>
#include <QColor>
#include <iostream>
#include <boost/algorithm/string.hpp>

using namespace std;

/**
 * This method determines the determinante of Martrix T
 */
double getDeterminent(double x1, double x2, double x3, double y1, double y2, double y3)
{
    double a = x1 - x3;
    double b = x2 - x3;
    double c = y1 - y3;
    double d = y2 - y3;

    return a*d - b*c;
}

/**
 * This method calculates lambda 1
 */
double getLambdaOne(double x, double y, double x1, double x2, double x3, double y1, double y2, double y3, double det)
{
    return ((y2 - y3) * (x - x3) + (x3 - x2) * (y - y3)) / det;
}

/**
 * This method calculates lambda 2
 */
double getLambdaTwo(double x, double y, double x1, double x2, double x3, double y1, double y2, double y3, double det)
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
        cout << inputs[k] << endl;
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

    // create an image 640 pixels wide by 480 pixels tall
    // with a standard 32 bit red, green, blue format
    QImage image(640, 480, QImage::Format_RGB32);

    double x1 = values[0][0];
    double x2 = values[1][0];
    double x3 = values[2][0];
    double y1 = values[0][1];
    double y2 = values[1][1];
    double y3 = values[2][1];
    double minX = min(min(x1, x2), x3);
    double maxX = max(max(x1, x2), x3);
    double minY = min(min(y1, y2), y3);
    double maxY = max(max(y1, y2), y3);
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

                if (lambda1 >= 0 && lambda2 >= 0 && lambda3 >= 0)
                {
                    image.setPixel(i,j, qRgb(colorR,colorG,colorB));
                }
            }
        }
    }

    if(image.save("triangle.jpg",0,100)) {
        cout << "Output triangle.jpg" << endl;
    } else {
        cout << "Unable to save triangle.jpg" << endl;
    }
}
