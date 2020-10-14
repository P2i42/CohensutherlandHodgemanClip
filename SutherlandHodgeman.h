#ifndef SUTHERLANDHODGEMAN_H
#define SUTHERLANDHODGEMAN_H
#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <map>
#include <vector>

#define MAX 20
typedef struct {
	double x;
	double y;
}Vertex;
//typedef Vertex VertexArray[2];
typedef std::vector<Vertex> VertexArray;

void drawVertexArray(VertexArray line) { //�޸ĳ����������ⶥ������
	//glLineWidth(2);//�����߶ο��
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_POLYGON);
	
	glColor3f(1.0, 0.0, 0.0);
	for (auto i : line) {
		glVertex2f(i.x, i.y); //�������귶Χ
	}
	glEnd();
}

bool isInside(Vertex p, VertexArray clipBoundary, int direction) {
	//direction�����жϵ����ĸ�λ��
	if (direction == 1) {
		//��
		if (p.x >= clipBoundary[0].x) return true;
		else return false;
	}
	else if (direction == 2) {
		//��
		if (p.y <= clipBoundary[1].y) return true;
		else return false;
	}
	else if (direction == 3) {
		//��
		if (p.x <= clipBoundary[1].x) return true;
		else return false;
	}
	else {
		//��
		if (p.y >= clipBoundary[0].y) return true;
		else return false;
	}
}

void intersect(Vertex s, Vertex p, VertexArray clipBoundary, Vertex &ip, int direction)
{
	if (p.x != s.x) {
		double k = (p.y - s.y) / (p.x - s.x);
		double b = p.y - k * p.x;
		if (direction == 1) {
			ip.x = clipBoundary[0].x;
			ip.y = k * ip.x + b;
		}
		else if (direction == 2) {
			ip.y = clipBoundary[1].y;
			ip.x = (ip.y - b) / k;
		}
		else if (direction == 3) {
			ip.x = clipBoundary[1].x;
			ip.y = k * ip.x + b;
		}
		else {
			ip.y = clipBoundary[0].y;
			ip.x = (ip.y - b) / k;
		}
	}
	else {
		if (direction == 1) {
			ip.x = s.x;
			ip.y = s.y;
		}
		else if (direction == 2) {
			ip.y = clipBoundary[1].y;
			ip.x = s.x;
		}
		else if (direction == 3) {
			ip.x = s.x;
			ip.y = s.y;
		}
		else {
			ip.y = clipBoundary[0].y;
			ip.x = s.x;
		}
	}
	
	
}

//clipBoundary�������º���������[0]��[1]
VertexArray SutherlandHodgeClip(VertexArray inVertexArray,
	VertexArray outVertexArray,
	VertexArray clipBoundary, int &inLength, int &outLength) {
	Vertex s, p, ip;
	int j;
	outLength = 0;
	for (int direction = 1; direction <= 4; direction++) {
		printf("\n");
		s = inVertexArray[inLength - 1];
		for (j = 0; j < inLength; j++) {
			
			p = inVertexArray[j];
			printf("��ǰs������(%f, %f), ��ǰp������(%f, %f)\n", s.x, s.y, p.x, p.y);
			if (isInside(p, clipBoundary, direction)) {
				if (isInside(s, clipBoundary, direction)) {
					//SP�ڴ����ڣ����1
					//output(p, outLength, outVertexArray);
					outLength++;
					outVertexArray.push_back(p);
					printf("���1��p������(%f, %f)\n" ,p.x, p.y);
				}
				else {
					//S�ڴ����⣬���4
					intersect(s, p, clipBoundary, ip, direction);
					outLength += 2;
					outVertexArray.push_back(ip);
					outVertexArray.push_back(p);
					printf("���4��ip������(%f, %f), p������(%f, %f)\n", ip.x, ip.y, p.x, p.y);
				}
			}
			else if (isInside(s, clipBoundary, direction)) {
				//S�ڴ����ڣ�P�ڴ����⣬���3
				intersect(s, p, clipBoundary, ip, direction);
				outLength++;
				outVertexArray.push_back(ip);
				printf("���3��ip������(%f, %f)\n", ip.x, ip.y);
			}
			
			s = p;
		}
		inVertexArray.clear();
		inVertexArray = outVertexArray;
		inLength = outLength;
		if (direction != 4) {
			outVertexArray.clear();
			outLength = 0;
		}
		for (auto i : inVertexArray) {
			printf("(%f,%f), ", i.x, i.y);
		}
		
	}
	return outVertexArray;
}

#endif // !SUTHERLANDHODGEMAN_H

#pragma once
