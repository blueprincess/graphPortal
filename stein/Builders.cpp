// Builders.cpp
// Template for OpenGL 3.*
// N. Dommanget dommange@univ-mlv.fr

#include "Builders.hpp"
#include "Color.hpp"
#include "Object.hpp"
#include "Tools.hpp"
#include "MeshBuilder.h"
#include "math/StreamUtils.h"

#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cfloat>

using namespace std;

namespace stein {

// Builds one triangle
void buildTriangle(Object &object) {
    MeshBuilder builder;
    builder.addVertex(-0.2, 0, 0);
    builder.addVertex(0.2, 0, 0);
    builder.addVertex(0, 1, 0);
    builder.addNormal(0, 0, 1);
    builder.addFace(0, 1, 2).setNormalIndices(0, 0, 0);
    
    vector<unsigned int> indices;
    vector<Vector3f> vertices;
    vector<Vector3f> normals;
    vector<UV> uvs;

    builder.unpack(indices, vertices, normals, uvs);

    // Sends the data into buffers on the GPU
    object.sendPrimitives(vertices, indices);
}

// Builds one square
void buildSquare(Object &object, const GLfloat side) {
    MeshBuilder builder;
    builder.addVertex(-side, -side, 0); // bottom left
    builder.addVertex(side, -side, 0); // bottom right
    builder.addVertex(-side, side, 0); // top left
    builder.addVertex(side, side, 0); // top right
    builder.addFace(0, 2, 3); // First triangle : top left
    builder.addFace(0, 1, 3); // Second triangle : bottom right

    vector<unsigned int> indices;
    vector<Vector3f> vertices;
    vector<Vector3f> normals;
    vector<UV> uvs;

    builder.unpack(indices, vertices, normals, uvs);

    // Sends the data into buffers on the GPU
    object.sendPrimitives(vertices, indices);
}

struct Triple {
    GLuint a, b, c;
};

static Triple readTriple(string& line) {
    replace(line.begin(), line.end(), '/', ' ');
    istringstream ss(line);
    size_t a, b, c;
    ss >> a >> b >> c;
    Triple value = { --a, --b, --c };
    return value;
}

// Builds an object made from an OBJ file, taking only geometry into account (not materials)
bool buildObjectGeometryFromOBJ(Object &object, const char* fileName, bool smoothObject) {
    ifstream file(fileName, ios_base::in);
    if (!file) {
        cout << "       Error while loading object from .obj file : " << fileName << "." << endl;
        return false;
    }
    cout << "       Loading object from .obj file : " << fileName << "." << endl;

    MeshBuilder builder;
    bool hasVt = false;
    bool hasVn = false;
    string buf, key, name, MTLFileName;

    while (getline(file, buf)) {
        istringstream line(buf);
        line >> key;
        if (key == "o")
            line >> name;
        else if (key == "v") {
            float x, y, z;
            line >> x >> y >> z;
            builder.addVertex(x, y, z);
        } else if (key == "vt") {
            float u, v;
            line >> u >> v;
            builder.addUV(u, 1 - v);
            hasVt = true;
        } else if (key == "vn") {
            float x, y, z;
            line >> x >> y >> z;
            builder.addNormal(x, y, z);
            hasVn = true;
        } else if (key == "f") {
            string a, b, c;
            line >> a >> b >> c;
            const Triple t1 = readTriple(a);
            const Triple t2 = readTriple(b);
            const Triple t3 = readTriple(c);
            builder.addFace(t1.a, t2.a, t3.a).setUvIndices(t1.b, t2.b, t3.b).setNormalIndices(t1.c, t2.c, t3.c);
        } else if (key == "mltlib")
            line >> MTLFileName;
    }

    cout << "       Obj mesh " << name << " loading..." << endl;
    cout << "       Obj meshes should only be made of triangles (for this loader), make sure this is correct in file." << endl;

    //    if (smoothObject)
//        reorderUvsAndNormalsIfSmooth(&vertices, &uvs, &normals, &indices, &uvIndices, &normalIndices);
//    else
//        reorderUvsAndNormalsIfNonSmooth(&vertices, &uvs, &normals, &indices, &uvIndices, &normalIndices);

//    conformToObject(&vertices, &uvs, &normals);

    builder.centerAndNormalizeMesh();
    builder.normalizeNormals();

    vector<unsigned int> indices;
    vector<Vector3f> vertices;
    vector<Vector3f> normals;
    vector<UV> uvs;

    builder.unpack(indices, vertices, normals, uvs);

    object.sendPrimitives(vertices, indices);

    if (hasVn) {
        const bool oneNormalPerTriangle = (normals.size() == indices.size());
        if (oneNormalPerTriangle) {
            cout << "       Obj file " << name << " was not smoothed in modeler." << endl;
            if (smoothObject)
                cout << "       WARNING : smoothObject==true. Normals will be wrong : change it to false." << endl;
        }
        object.sendNormals(normals);
    } else
        cout << "       WARNING : Obj file " << name << " has no normals, add some in modeler." << endl;

    if (hasVt)
        object.sendUvs(uvs);
    else
        cout << "       WARNING : Obj file " << name << " has no texture coordinates, add some in modeler." << endl;

    cout << "       Material files are not taken into account by this loader." << endl;

    return true;
}

} // namespace stein
