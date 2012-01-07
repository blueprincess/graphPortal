// Builders.cpp //Karine
// Template for OpenGL 3.*
// N. Dommanget dommange@univ-mlv.fr


#include "Builders.hpp"
#include "Object.hpp"
#include "Tools.hpp"

#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <cstdlib>

void buildAxis(Object *object)//void
{
	object->nbVertices=4;
	object->nbIndices=6;
//The vertices of the axis
		GLfloat vertices[]={0.0, 0.0, 0.0, 0.1,//O
		1.0, 0.0, 0.0, 1.0,//x
		0.0, 1.0, 0.0, 1.0,//y
		0.0, 0.0, 1.0, 1.0};//z
//The indices of the vertices of the 3 lines
		GLuint indices[]={0,1, 0, 2, 0, 3};
		GLfloat colors[]={1.0, 1.0, 1.0, 1.0,//white
				1.0, 0.0, 0.0, 1.0, //red
				0.0, 1.0, 0.0, 1.0, //green
				0.0, 0.0, 1.0, 1.0}; //blue
//Sends the data into buffers on the GPU
	object->sendPrimitives(vertices, indices);
	object->sendColors(colors);
}

void buildTriangle(Object * object, GLfloat decalage[])
{
    object->nbVertices=3;
    object->nbIndices=3;
	
    // The 3 vertices of a triangle
	GLfloat vertices[]={-0.5+decalage[0], decalage[1], decalage[2], 1.0, 
	                     0.5+decalage[0], decalage[1], decalage[2], 1.0,
	                     0.0+decalage[0], 0.5+decalage[1], decalage[2], 1.0};

	GLfloat uvs[]={0.0, 0.0, 1.0, 0.0, 0.5, 1,0};


    // The 3 indices of the vertices to draw the face
    GLuint indices[]={0, 1, 2};
    
    // Sends the data into buffers on the GPU
    object->sendPrimitives(vertices, indices);
    
	GLfloat colors[12]={1.0,0.0,0.0,1.0,
				0.0,1.0,0.0,1.0,
				0.0,0.0,1.0,1.0};
	object->sendColors(colors);
	object->sendUvs(uvs);
}

void buildSphere(Object * object, GLfloat radius, GLuint discLat, GLuint discLong){

	object->nbVertices = discLong*6*discLat;
	object->nbIndices = discLong*6*discLat;

	GLfloat  vertices[object->nbVertices*4];		
	GLuint  indices[object->nbIndices];

	GLfloat alpha = (2*M_PI)/GLfloat(discLong); //largeur
	GLfloat theta = 0.0; //De 0 à 2*Pi

	GLfloat gamma = M_PI/GLfloat(discLat); //hauteur
	GLfloat omega = -M_PI/2.0; //De -Pi/2 à Pi/2
	
	GLuint etage;
	for(etage=0;etage<discLat;etage++){

		GLfloat radLocal = cos(omega);

		//faire une boucle pour créer une face avec deux triangles
		
		GLuint i;
		for(i=0;i<discLong;i++){

			vertices[(etage*discLong+i)*24+8+0] = radius * cos(omega+gamma) * cos(theta); 				
			vertices[(etage*discLong+i)*24+8+1] = radius * sin(omega+gamma);
			vertices[(etage*discLong+i)*24+8+2] =- radius * cos(omega+gamma) * -sin(theta);
			vertices[(etage*discLong+i)*24+8+3] = 	1.0;

			vertices[(etage*discLong+i)*24+4+0] = radius * cos(omega+gamma) * cos(theta+alpha); 				
			vertices[(etage*discLong+i)*24+4+1] = radius * sin(omega+gamma);
			vertices[(etage*discLong+i)*24+4+2] =- radius * cos(omega+gamma) * -sin(theta+alpha);
			vertices[(etage*discLong+i)*24+4+3] = 1.0;

			vertices[(etage*discLong+i)*24+0] = radius * radLocal * cos(theta); 				
			vertices[(etage*discLong+i)*24+1] = radius * sin(omega);
			vertices[(etage*discLong+i)*24+2] =- radius * radLocal * -sin(theta);
			vertices[(etage*discLong+i)*24+3] = 1.0;
		
			vertices[(etage*discLong+i)*24+12+0] = radius * radLocal * cos(theta);						
			vertices[(etage*discLong+i)*24+12+1] = radius * sin(omega);
			vertices[(etage*discLong+i)*24+12+2] =- radius * radLocal * -sin(theta);
			vertices[(etage*discLong+i)*24+12+3] = 	1.0;
			
			vertices[(etage*discLong+i)*24+20+0] = radius * cos(omega+gamma) * cos(theta+alpha); 		
			vertices[(etage*discLong+i)*24+20+1] = radius * sin(omega+gamma);
			vertices[(etage*discLong+i)*24+20+2] =- radius * cos(omega+gamma) * -sin(theta+alpha);
			vertices[(etage*discLong+i)*24+20+3] = 	1.0;

			vertices[(etage*discLong+i)*24+16+0] = radius * radLocal * cos(theta+alpha); 		
			vertices[(etage*discLong+i)*24+16+1] = radius * sin(omega);
			vertices[(etage*discLong+i)*24+16+2] =- radius * radLocal * -sin(theta+alpha);
			vertices[(etage*discLong+i)*24+16+3] = 	1.0;	

			theta += alpha;

			GLuint indexIndices = (etage*discLong+i)*6;
			indices[indexIndices+0]=indexIndices;
			indices[indexIndices+1]=indexIndices+1;
			indices[indexIndices+2]=indexIndices+2;
			indices[indexIndices+3]=indexIndices+3;
			indices[indexIndices+4]=indexIndices+4;
			indices[indexIndices+5]=indexIndices+5;
		}
		theta = 0.0;
		omega += gamma;
	}
	
	GLfloat normals[object->nbIndices*3];
	object->sendPrimitives(vertices, indices);
	setNormalsFlat(object->nbIndices,vertices,indices,normals);
	object->sendNormals(normals);

}

//To fill the provided normal Question 4 j TD1)
void normalFace(GLfloat *O, GLfloat *A, GLfloat *B, GLfloat *normal, bool toNormalize)
{
	GLfloat a[]={A[0]-O[0], A[1]-O[1], A[2]-O[2]};
	GLfloat b[]={B[0]-O[0], B[1]-O[1], B[2]-O[2]};
	
	vectorProduct(&(a[0]),&(b[0]), normal);
	if(toNormalize)
		normalize(normal);
}

//To fill the normals for vertices in flat triangles
void setNormalsFlat(GLuint nbIndices, GLfloat *vertices, GLuint *indices, GLfloat *normals)
{	
	for(GLuint iTriangle=0; iTriangle<nbIndices; iTriangle+=3)
	{
		GLuint iIndices0=indices[iTriangle+0];
		GLuint iIndices1=indices[iTriangle+1];
		GLuint iIndices2=indices[iTriangle+2];
		GLuint iVertices0=iIndices0*4;
		GLuint iVertices1=iIndices1*4;
		GLuint iVertices2=iIndices2*4;
		GLuint iNormals0=iIndices0*3;
		GLuint iNormals1=iIndices1*3;
		GLuint iNormals2=iIndices2*3;
	
		normalFace(&(vertices[iVertices0]),&(vertices[iVertices1]),&(vertices[iVertices2]), &(normals[iNormals0]), true);
		normals[iNormals1+0]=normals[iNormals0+0];
		normals[iNormals1+1]=normals[iNormals0+1];
		normals[iNormals1+2]=normals[iNormals0+2];
		normals[iNormals2+0]=normals[iNormals0+0];
		normals[iNormals2+1]=normals[iNormals0+1];
		normals[iNormals2+2]=normals[iNormals0+2];
	}
}



	
 

// Centers and normalize any mesh
void centerAndNormalizeMesh(Object * object, GLfloat * vertices)
{
    GLfloat inf=100000.0;
    // Normalizes to 1.0 the size of the mesh
    GLfloat min[]={inf, inf, inf};
    GLfloat max[]={-inf, -inf, -inf};

    for(GLuint iVerticesValues=0 ; iVerticesValues<object->nbVertices ; iVerticesValues++)
    {
        for(GLuint iCoord=0 ; iCoord<3 ; iCoord++)
        {
            GLfloat value=vertices[iVerticesValues*4+iCoord];
            if (value<min[iCoord])
                min[iCoord]=value;
            if (value>max[iCoord])
                max[iCoord]=value;
        }
    }
    GLfloat maxSize[]={max[0]-min[0], max[1]-min[1], max[2]-min[2]};
    GLuint maxCoord=0;
    if ((maxSize[1]>=maxSize[0]) && (maxSize[1]>=maxSize[2])) maxCoord=1;    
    if ((maxSize[2]>=maxSize[0]) && (maxSize[2]>=maxSize[1])) maxCoord=2;

    for(GLuint iVerticesValues=0 ; iVerticesValues<object->nbVertices ; iVerticesValues++)
    {
        for(GLuint iCoord=0 ; iCoord<3 ; iCoord++)
        {
            vertices[iVerticesValues*4+iCoord]-=min[iCoord]+(maxSize[iCoord]/2.0);
            vertices[iVerticesValues*4+iCoord]/=maxSize[maxCoord];
        }
    } 
}






//______________________________________________________________________________
// Obj file reading fuctions


// Fills a std::vector with strings from cuts at every "delim" in "string"
void split(const std::string& string, std::vector<std::string>& tokens, const std::string& delim)
{
        // Clears the vector where the results are put
        tokens.clear();
        // Goes through the string and extract the tokens
        for (std::string::size_type p1=0, p2=0; p1!=std::string::npos; )
        {
                p1=string.find_first_not_of(delim, p1);
                if (p1!=std::string::npos)
                {
                        p2=string.find_first_of(delim, p1);
                        tokens.push_back(string.substr(p1, p2-p1));
                        p1=p2;
                }
        }
}

// Fills a std::vector with three GLfloat values in a string
void readVec3(std::istringstream& line, std::vector<GLfloat> * vertices)
{
    GLfloat vec[3]={0.0, 0.0, 0.0};
    line >> vec[0] >> vec[1] >> vec[2];
    vertices->push_back(vec[0]);
    vertices->push_back(vec[1]);
    vertices->push_back(vec[2]);
}

// Fills a std::vector with two GLfloat values in a string
void readVec2(std::istringstream& line, std::vector<GLfloat> * vertices)
{
    GLfloat vec[2]={0.0, 0.0};
    line >> vec[0] >> vec[1];
    vertices->push_back(vec[0]);
    vertices->push_back(1.0-vec[1]);
}

// Fills std::vectors with indices of three type for the vertices in the face
void readFace(std::istringstream& line, std::vector<GLuint> * indices,  std::vector<GLuint> * uvIndices, std::vector<GLuint> * normalsIndices)
{
    std::string face;
    while (!line.eof())
	{
        line >> face;
            std::vector<std::string> sequence;
            split(face, sequence, "/");
            size_t index=strtoul(sequence[0].c_str(), NULL, 10)-1;
            indices->push_back(index);

            size_t uvIndex=strtoul(sequence[1].c_str(), NULL, 10)-1;
            uvIndices->push_back(uvIndex);

            size_t normalIndex=strtoul(sequence[2].c_str(), NULL, 10)-1;
            normalsIndices->push_back(normalIndex);
    }
}

// Regroup in four arrays instead of six by assigning a uv coordinates and a normal to the corresponding vertex. 
// Does not create new vertices : works only if one normal is sufficient per location in space : smoothed objects
void reorderUvsAndNormalsIfSmooth(std::vector<GLfloat> * vertices, std::vector<GLfloat> * uvs, std::vector<GLfloat> * normals,
                          std::vector<GLuint> * indices, std::vector<GLuint> * uvIndices, std::vector<GLuint> * normalIndices)
{
    std::vector<GLfloat> newUvs(vertices->size(), 0.0);
    std::vector<GLfloat> newNormals(vertices->size(), 0.0);
    GLuint uvIndex;
    GLuint normalIndex;
    for(GLuint iIndices=0 ; iIndices<indices->size() ; iIndices++)
    {
        uvIndex=(*uvIndices)[iIndices];
        normalIndex=(*normalIndices)[iIndices];
        for (GLuint iCoord=0 ; iCoord<3 ; iCoord++)
        {
            newNormals[((*indices)[iIndices]*3)+iCoord]=(*normals)[(normalIndex*3)+iCoord];
            if (iCoord<2)
                newUvs[((*indices)[iIndices]*2)+iCoord]=(*uvs)[(uvIndex*2)+iCoord];
        }
    }
    
    uvs->clear();
    normals->clear();
    for(GLuint iVertices=0 ; iVertices<vertices->size() ; iVertices++)
    {
        normals->push_back(newNormals[iVertices]);
    }
    for(GLuint iUvs=0 ; iUvs<newUvs.size() ; iUvs++)
    {      
        uvs->push_back(newUvs[iUvs]);
    }
}

// Regroup in four arrays instead of six by assigning a uv coordinates and a normal to corresponding vertices. 
// Creates new vertices to be able to give several normals per location in space : non smoothed objects
void reorderUvsAndNormalsIfNonSmooth(std::vector<GLfloat> * vertices, std::vector<GLfloat> * uvs, std::vector<GLfloat> * normals,
                                     std::vector<GLuint> * indices, std::vector<GLuint> * uvIndices, std::vector<GLuint> * normalIndices)
{
    std::vector<GLfloat> newVertices;
    std::vector<GLfloat> newUvs;
    std::vector<GLfloat> newNormals;
    GLuint vertexIndex;
    GLuint uvIndex;
    GLuint normalIndex;
    for(GLuint iIndices=0 ; iIndices<indices->size() ; iIndices++)
    {
        vertexIndex=(*indices)[iIndices];
        uvIndex=(*uvIndices)[iIndices];
        normalIndex=(*normalIndices)[iIndices];
        for (GLuint iCoord=0 ; iCoord<3 ; iCoord++)
        {
            newVertices.push_back((*vertices)[(vertexIndex*3)+iCoord]);
            newNormals.push_back((*normals)[(normalIndex*3)+iCoord]);
            if (iCoord<2)
                newUvs.push_back((*uvs)[(uvIndex*2)+iCoord]);
        }
        (*indices)[iIndices]=iIndices;
    }
    
    vertices->clear();
    uvs->clear();
    normals->clear();
    for(GLuint iVertices=0 ; iVertices<newVertices.size() ; iVertices++)
    {
        vertices->push_back(newVertices[iVertices]);
        normals->push_back(newNormals[iVertices]);
    }
    for(GLuint iUvs=0 ; iUvs<newUvs.size() ; iUvs++)
    {      
        uvs->push_back(newUvs[iUvs]);
    }
}

// Adds w to every vertices
void addHomogeneousToVertices(std::vector<GLfloat> * vertices)
{
    std::vector<GLfloat> newVertices;
    for(GLuint iVertices=0 ; iVertices<(vertices->size())/3 ; iVertices++)
    {
        for (GLuint iCoord=0 ; iCoord<3 ; iCoord++)
            newVertices.push_back((*vertices)[(iVertices*3)+iCoord]);        
        newVertices.push_back(1.0); 
    }
    
    vertices->clear();
    for(GLuint iVertices=0 ; iVertices<newVertices.size() ; iVertices++)
        vertices->push_back(newVertices[iVertices]);   
}


// Changes the array to conform our Object's vbos formats
void conformToObject(std::vector<GLfloat> * vertices, std::vector<GLfloat> * uvs, std::vector<GLfloat> * normals)
{
    addHomogeneousToVertices(vertices);
    // Normalizes normals
    for(GLuint iNormals=0 ; iNormals<(normals->size())/3 ; iNormals++)
    {
        normalize(&((*normals)[iNormals*3]));
    }
}


// Builds an object made from an OBJ file, taking only geometry into account (not materials)
bool buildObjectGeometryFromOBJ(Object * object, const std::string& fileName, bool smoothObject)
{
    std::ifstream file(fileName.c_str(), std::ios_base::in);
    if(!file)
    {
        std::cout<<"       Error while loading object from .obj file : "<<fileName<<"."<<std::endl;
        return false;
    }
    std::cout<<"       Loading object from .obj file : "<<fileName<<"."<<std::endl; 

    bool hasVt=false;
    bool hasVn=false;
	std::vector<GLfloat> vertices;
    std::vector<GLfloat> uvs;
    std::vector<GLfloat> normals;
    std::vector<GLuint> indices;
    std::vector<GLuint> uvIndices;
    std::vector<GLuint> normalIndices;
    std::string buf, key, name, MTLFileName;

    while (getline(file, buf))
	{
            std::istringstream line(buf);
            line >> key;    
            if (key=="o")
      		    line >> name;
            else if(key=="v")
      		     readVec3(line, &vertices);
      		else if(key == "vt")
            {    readVec2(line, &uvs); hasVt=true; }
            else if(key=="vn")
      		{    readVec3(line, &normals); hasVn=true; }
            else if(key=="f")
      		    readFace(line, &indices,  &uvIndices, &normalIndices);
            else if (key=="mltlib")
      		    line >> MTLFileName;
    }

    std::cout<<"       Obj mesh "<<name<<" loading..."<<std::endl;
    std::cout<<"       Obj meshes should only be made of triangles (for this loader), make sure this is correct in file."<<std::endl;
    if (!hasVt) 
        std::cout<<"       Obj file "<<name<<" has no texture coordinates, add some in modeler."<<std::endl;

    if (!hasVn) 
    {
        std::cout<<"       Obj file "<<name<<" has no normals, add some in modeler."<<std::endl;
    }
    else 
    {
        if (normals.size()*3==(indices.size()*3)) 
        {
            std::cout<<"       Obj file "<<name<<" was not smoothed in modeler."<<std::endl;
            if (smoothObject==true)
                std::cout<<"       Normals will be wrong with smoothObject parameter as true, change it to false."<<std::endl;
        }
    }
    bool onenormalPerTriangle=(normals.size()*3==(indices.size()*3));

    if (smoothObject)
        reorderUvsAndNormalsIfSmooth(&vertices, &uvs, &normals, &indices, &uvIndices, &normalIndices);
    else
        reorderUvsAndNormalsIfNonSmooth(&vertices, &uvs, &normals, &indices, &uvIndices, &normalIndices);
    conformToObject(&vertices, &uvs, &normals);

    object->nbVertices=vertices.size()/4;
    object->nbIndices=indices.size();
    
    // Normalizes to 1.0 the size of the mesh and centers it
    centerAndNormalizeMesh(object, vertices.data());


    object->sendPrimitives(vertices.data(), indices.data());
    if (!hasVt) 
        std::cout<<"       WARNING : Obj file "<<name<<" has no texture coordinates, add some in modeler."<<std::endl;
   else
        object->sendUvs(uvs.data());

    if (!hasVn) 
    {
        std::cout<<"       WARNING : Obj file "<<name<<" has no normals, add some in modeler."<<std::endl;
    }
    else 
    {
        if (onenormalPerTriangle) 
        {
            std::cout<<"       Obj file "<<name<<" was not smoothed in modeler."<<std::endl;
            if (smoothObject)
                std::cout<<"       WARNING : smoothObject==true. Normals will be wrong : change it to false."<<std::endl;
        }
        //object->sendNormals(normals.data());
    }
    std::cout<<"       Material files are not taken into account by this loader."<<std::endl;
    
    return true;
}
