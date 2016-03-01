#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

using namespace std;

using glm::vec2;
using glm::vec4;
using glm::mat4;
using glm::ortho;
using glm::inverse;

// This code shows conversions from different spaces that could be used in Program 1.
// If you use these conversions, when you resize the window the image will be centered
// in the window and the whole width of the image will be shown. If the aspect ratio
// of the image is less than the aspect ratio of the window then the image will be cropped
// a bit on the top and bottom. You can perform similar conversions to map the image so 
// the whole height of the image is shown in that case.

int main(int argc, char** argv) {
    vec2 mouseClick(300,400); // Qt mouse press event coordinate

    int width = 600;   // width of the window
    int height = 400;  // height of the window

    int image_width = 1200;   // width of the image
    int image_height = 1000;  // height of the image

    cout << "mouse click: (" << mouseClick.x << ", " << mouseClick.y << ")" << endl;

    mat4 mouse2ndc = ortho(0.0f, (float)width, (float)height, 0.0f); // A standard orthographic projection matrix, in this case
                                                                     // one that represents a conversion from out mouse click space
                                                                     // (from 0,0 in the top left to width,height in the bottom right)
                                                                     // to normalized device coordinates

    vec4 ndcCoord = mouse2ndc*vec4(mouseClick, 0, 1); 

    cout << "mouse click converted to NDC: (" << ndcCoord.x << ", " << ndcCoord.y << ")" << endl << endl;

    float aspect = (float)width/height;
    float invaspect = (float)height/width;

    float image_aspect = (float)image_width/image_height;
    float image_invaspect = (float)image_height/image_width;

    cout << "aspect ratio of window: " << width << " / " << height << " = " << aspect << endl;
    cout << "inverse aspect ratio of window: " << height << " / " << width << " = " << invaspect << endl << endl;

    cout << "aspect ratio of image: " << image_width << " / " << image_height << " = " << image_aspect << endl;
    cout << "inverse aspect ratio of image: " << image_height << " / " << image_width << " = " << image_invaspect << endl << endl;

    mat4 ws2ndc = ortho(-1.0f, 1.0f, -invaspect, invaspect); // Orthographic projection matrix mapping our world space
                                                       // to ndc. I just decided on using this space, but you could
                                                       // easily use a different space to store your points in.
                                                       // We'd pass this to our vertex shader.
    mat4 ndc2ws = inverse(ws2ndc); // Take the inverse so we can convert from NDC back to world space.
                                   // We'll use this to convert our ndcCoord into our world space.

    vec4 wsCoord = ndc2ws*ndcCoord;

    cout << "world space coordinate: (" << wsCoord.x << ", " << wsCoord.y << ")" << endl << endl;

    vec2 imgCoord(wsCoord); // Start with our world space coordinate.
                             // We'll convert it to an image space coordinate.
                             // This conversion is what I got hung up on in class.

    imgCoord.y *= -1; // negate y because positive y goes down when indexing into the image
    imgCoord += vec2(1.0f,(float)image_invaspect); // offset the origin to the top left of the image
    imgCoord *= .5*image_width; // divide by 2 (the width of the screen in world space coordinates) then multiply
                                // by the width to scale our space to the width of the image.

    // We could also use a matrix to do the conversion, which is a combined translation and scale matrix
    mat4 ws2img(.5*image_width, 0, 0, 0,
                0, -.5*image_width, 0, 0,
                0, 0, 1, 0,
                .5*image_width, .5*image_height, 0, 1);

    cout << "image coordinate: (" << imgCoord.x << ", " << imgCoord.y << ")" << endl;

    vec4 imgCoord2 = ws2img*wsCoord;
    cout << "image coordinate2: (" << imgCoord2.x << ", " << imgCoord2.y << ")" << endl << endl;

    // The image coordinate isn't necessarily in the image bounds as our window can be bigger than the image.
    // We'll have to check this before sampling to avoid getting some qt warnings.

    if(imgCoord.x >= 0 && imgCoord.x < image_width &&
        imgCoord.y >= 0 && imgCoord.y < image_height) {
        // sample color from image 
        cout << "inside the image" << endl;
    } else {
        cout << "not inside the image" << endl;
    }
}
