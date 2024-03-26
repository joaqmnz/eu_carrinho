#pragma once
#include<iostream>
#include<fstream>
#include<sstream>
#include"Primitivas.h"
#include <stdio.h>
#include <vector>

using namespace std;

enum Poligono{TRIANG=3, QUAD=4};

struct face {
	Poligono tipo;
	int vertice[4];
	int normal[4];
	face(Poligono tipo_, 
		int v1, int v2, int v3, int v4,
		int n1, int n2, int n3, int n4) {
		vertice[0] = v1; vertice[1] = v2;
		vertice[2] = v3; vertice[3] = v4;
		normal[0] = n1; normal[1] = n2;
	   	normal[2] = n3; normal[3] = n4;
		tipo = tipo_;
	}
};
namespace ObjLoader
{
	vetor3 getVertice(string s);
	vetor3 getNormal(string s);
	face getFace(Poligono tipo_, string s);
	void loadOBJ(unsigned& id, const char* filePath);
};

vetor3 ObjLoader::getVertice(string s)
{
	float x, y, z;
	sscanf(s.c_str(), "v %f %f %f", &x, &y, &z);
	vetor3 result(x, y, z);
	
	return result;
}

vetor3 ObjLoader::getNormal(string s)
{
	float x, y, z;
	sscanf(s.c_str(), "vn %f %f %f", &x, &y, &z);
	vetor3 result(x, y, z);
	
	return result;
}

face ObjLoader::getFace(Poligono tipo_, string s)
{
	int v1, v2, v3, v4, vt, n1, n2, n3, n4;

	if (tipo_ == Poligono::TRIANG) {
		v4 = -1; n4 = -1;
		sscanf(s.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &v1, &vt, &n1, &v2, &vt, &n2, &v3, &vt, &n3);
	}
	else if (tipo_ == Poligono::QUAD) {
		sscanf(s.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", &v1, &vt, &n1, &v2, &vt, &n2, &v3, &vt, &n3, &v4, &vt, &n4);
	}
	
	face result(tipo_, v1, v2, v3, v4, n1, n2, n3, n4);
	return result;
}

void ObjLoader::loadOBJ(unsigned & id, const char * filePath)
{
	vector<vetor3> vertices;
	vector<vetor3> normals;
	vector<face> faces;

	fstream arq(filePath);
	string line = "";

	if (!arq.is_open()) {
		cout << "ERRO::Nao foi possivel abrir o arquivo " << filePath << "\n";
	}

	while (getline(arq, line)) {
		if (line.find("v ") != string::npos) {
			vetor3 tempVertice = getVertice(line);
			vertices.push_back(tempVertice);
		}
		else if (line.find("vn ") != string::npos) {
			vetor3 tempNormal = getNormal(line);
			normals.push_back(tempNormal);
		}
		else if (line.find("f ") != string::npos) {
			short nSpace = 0;
			for (char c : line)
				if (c == ' ') nSpace++;
			Poligono tipo_ = (Poligono)nSpace;
			face tempFace = getFace(tipo_,line);
			faces.push_back(tempFace);
		}
	}

	id = glGenLists(1);
	
	glNewList(id, GL_COMPILE);
	glPolygonMode(GL_FRONT, GL_FILL);
	for (int i = 0; i < faces.size(); i++) {
		int v1 = faces[i].vertice[0] - 1; int v2 = faces[i].vertice[1] - 1;
		int v3 = faces[i].vertice[2] - 1; int v4 = faces[i].vertice[3] - 1;
		int n1 = faces[i].normal[0] - 1;  int n2 = faces[i].normal[1] - 1;
		int n3 = faces[i].normal[2] - 1;  int n4 = faces[i].normal[3] - 1;

		if (faces[i].tipo == Poligono::TRIANG) {
			glBegin(GL_TRIANGLES);
			glNormal3fv(&normals[n1].x); glVertex3fv(&vertices[v1].x);
			glNormal3fv(&normals[n2].x); glVertex3fv(&vertices[v2].x);
			glNormal3fv(&normals[n3].x); glVertex3fv(&vertices[v3].x);
			glEnd();
		}
		else if (faces[i].tipo == Poligono::QUAD) {
			glBegin(GL_QUADS);
			glNormal3fv(&normals[n1].x); glVertex3fv(&vertices[v1].x);
			glNormal3fv(&normals[n2].x); glVertex3fv(&vertices[v2].x);
			glNormal3fv(&normals[n3].x); glVertex3fv(&vertices[v3].x);
			glNormal3fv(&normals[n4].x); glVertex3fv(&vertices[v4].x);
			glEnd();
		}
	}

	glEndList();
}