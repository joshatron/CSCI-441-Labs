#include <iostream>
#include <string>

//Part 5
class Vector3 {
    public:
        float x;
        float y;
        float z;

        // Constructor
        Vector3(float xx, float yy, float zz) : x(xx), y(yy), z(zz) {
            // nothing to do here as we've already initialized x, y, and z above
            std::cout << "in Vector3 constructor" << std::endl;
        }

        Vector3() : x(0), y(0), z(0)
        {
        }

        // Destructor - called when an object goes out of scope or is destroyed
        ~Vector3() {
            // this is where you would release resources such as memory or file descriptors
            // in this case we don't need to do anything
            std::cout << "in Vector3 destructor" << std::endl;
        }
};

Vector3 add(const Vector3& v, const Vector3& v2)
{
    Vector3 c(v.x + v2.x, v.y + v2.y, v.z + v2.z);
    return c;
}

//Part 6
Vector3 operator+(const Vector3& v, const Vector3& v2)
{
    return add(v, v2);
}

std::ostream& operator<<(std::ostream& stream, const Vector3& v) {
    // std::cout is a std::ostream object, just like stream
    // so use stream as if it were cout and output the components of
    // the vector

    stream << "[" << v.x << ", " << v.y << ", " << v.z << "]";

    return stream;
}

//Part 1
int main(int argc, char** argv)
{
    //Part 4
    std::cout << "Part 4" << std::endl;
    std::string name;
    std::cin >> name;
    std::cout << "hello " << name << "; what the user types ";

    for(int k = 1; k < argc; k++)
    {
        std::cout << argv[k] << " ";
    }

    std::cout << ";" << std::endl;

    //Part 5
    std::cout << "Part 5" << std::endl;
    Vector3 a(1,2,3);
    Vector3 b(4,5,6);

    Vector3 c = add(a, b);

    std::cout << "x: " << c.x << ", y: " << c.y << ", z: " << c.z << std::endl;

    //Part 6
    std::cout << "Part 6" << std::endl;
    Vector3 v(1,2,3);
    Vector3 v2(4,5,6);

    std::cout << v + v2 << std::endl;

    //Part 7
    std::cout << "Part 7" << std::endl;
    Vector3 vector(0, 0, 0);
    vector.y = 5;
    std::cout << vector << std::endl;

    Vector3* vector2 = new Vector3(0, 0, 0);
    vector2->y = 5;
    std::cout << *vector2 << std::endl;
    delete vector2;

    //Part 8
    std::cout << "Part 8" << std::endl;
    Vector3 vectorArray[10];

    Vector3* vectorArray2 = new Vector3[10];
    for(int k = 0; k < 10; k++)
    {
        vectorArray2[k].y = 5;
    }

    delete [] vectorArray2;

    //Part 9
    std::cout << "Part 9" << std::endl;
    Vector3 a2(1,2,3);
    Vector3 b2(4,5,6);

    Vector3 c2 = add(a2, b2);

    std::cout << c2 << std::endl;
    std::cout << a2 + b2 << std::endl;
}
