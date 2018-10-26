#include "readOBJ.h"

using namespace std;


extern vector< vector< double > > vertices;
extern vector< vector< double > > faces;
extern vector< vector< double > > vertexNormals;


void parseFile(string OBJfile_name_)
{
    string filename = OBJfile_name_;
    ifstream fin(filename.c_str());
    string line = "";

    // Open the file
    if (fin.is_open())
    {
        while (fin.good())
        {
            getline(fin, line);
			
            istringstream input(line, istringstream::in);

            string type;
            input >> type;

            // Read in a vertex
            if (type == "v")
            {
                vector< double > vertex;
                for (int i = 0; i < 3; ++i)
                {
                    double value;
                    input >> value;
                    vertex.push_back(value);
                }
                vertices.push_back(vertex);
            }
            // Read in a face
            else if (type == "f")
            {
                vector< double > face;
                for (int i = 0; i < 3; ++i)
                {
                    double value;
                    input >> value;
                    face.push_back(value - 1);
                }
                faces.push_back(face);
            }
        }
    }
    else
    {
        cout << "Error: unable to open " << filename << "." << endl;
    }

    // Close the file
    fin.close();
}

/**
 * creates face normals for each face, and then uses them to create vertex normals for each vertex
 */
void createNormals()
{
    // Initialize the vertex normal array
    for (int i = 0; i < (int) vertices.size(); ++i)
    {
        vector< double > temp;
        for (int j = 0; j < 3; ++j)
        {
            temp.push_back(0.0);
        }
        vertexNormals.push_back(temp);
    }

    for (int i = 0; i < (int) faces.size(); ++i)
    {
        // Initialize the faceNormal vector
        vector< double > faceNormal;
        for (int j = 0; j < 3; ++j)
        {
            faceNormal.push_back(0);
        }

        // Find the face normal
        vector< double > a;
        vector< double > b;
        a = subtract(vertices[(int) faces[i][1]], vertices[(int) faces[i][0]]);
        b = subtract(vertices[(int) faces[i][2]], vertices[(int) faces[i][0]]);
        faceNormal = crossProduct(a, b);

        // Increase the vertex normal of each vertex
        // in the face by the face normal
        for (int j = 0; j < 3; ++j)
        {
            vector< double > vertexNormal = vertexNormals[(int) faces[i][j]];
            vector< double > newNormalA = add(vertexNormal, faceNormal);
            vector< double > newNormalB = subtract(vertexNormal, faceNormal);

            if (length(newNormalA) >= length(newNormalB))
            {
                vertexNormals[(int) faces[i][j]] = newNormalA;
            }
            else
            {
                vertexNormals[(int) faces[i][j]] = newNormalB;
            }
        }
    }

    // Normalize the vertex normals
    for (int i = 0; i < (int) vertexNormals.size(); ++i)
    {
        vertexNormals[i] = normalize(vertexNormals[i]);
    }
}

/**
 * Flips an image vertically. This must be done because image files have (0, 0) as the top-left point, and OpenGL has (0, 0) as the bottom-left point.
 * @param image
 *  The image to be flipped
 * @param width
 *  The width of the image
 * @param height
 *  The height of the image
 * @return
 *  The flipped image
 */
unsigned char * flipImage(unsigned char * image, int width, int height)
{
    const int RGB = 3;
    unsigned char imageArray[width][height][RGB];

    // Convert the 1-D input array into a 3-D array
    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            for (int k = 0; k < RGB; ++k)
            {
                imageArray[i][j][k] = image[i*RGB + j*width*RGB + k];
            }
        }
    }

    // Flip the image
    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height/2; ++j)
        {
            for (int k = 0; k < RGB; ++k)
            {
                unsigned char temp = imageArray[i][j][k];
                imageArray[i][j][k] = imageArray[i][height - j - 1][k];
                imageArray[i][height - j - 1][k] = temp;
            }
        }
    }

    // Convert the 3-D array into a 1-D array
    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            for (int k = 0; k < RGB; ++k)
            {
                image[i*RGB + j*width*RGB + k] = imageArray[i][j][k];
            }
        }
    }

    return image;
}
