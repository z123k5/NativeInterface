//#include <iostream>
//#include <android_native_app_glue.h>
//#include <glesTools.h>
#include <stdlib.h>
#include <GLES/gl.h>
#include "renderer.h"
#include "glesTools.h"

		void Renderer::init(int maxVertexNum, bool useTexture, bool open3dMode, bool useNormal) {
			arrSize = maxVertexNum;
			useTex=useTexture;
			mode3D=open3dMode;
			useNom=useNormal;
			vtxArray = new float[arrSize*(2+mode3D)];
			colArray = new float[arrSize*4];
			if(useTex)
			texArray = new float[arrSize*2];
			if(useNom)
			nomArray = new float[arrSize*3];
			ptr = 0;
			glGenBuffers(2+(useNom? 2:useTex), vbo);
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);
		}
		Renderer::Renderer() {
			vtxArray=nullptr;
		}
		Renderer::~Renderer() {
			if (vtxArray != nullptr) {
				delete[] vtxArray;
				glDeleteBuffers(2+(useNom? 2:useTex), vbo);
				delete[] colArray;
				if(useTex)
				delete[] texArray;
				if(useNom)
				delete[] nomArray;
			}
		}

		void Renderer::clear() {
			ptr = 0;
		}
		void Renderer::vertex2f(float x, float y) {
			vtxArray[ptr*2] = x;
			vtxArray[ptr*2+1] = y;
			ptr+=1;
		}
		void Renderer::vertex3f(float x, float y, float z) {
			vtxArray[ptr*3] = x;
			vtxArray[ptr*3+1] = y;
			vtxArray[ptr*3+2] = z;
			ptr+=1;
		}
		void Renderer::color3f(float r, float g, float b) {
			colArray[ptr*4] = r;
			colArray[ptr*4+1] = g;
			colArray[ptr*4+2] = b;
			colArray[ptr*4+3] = 1;
		}
		void Renderer::color4f(float r, float g, float b, float a) {
			colArray[ptr*4] = r;
			colArray[ptr*4+1] = g;
			colArray[ptr*4+2] = b;
			colArray[ptr*4+3] = a;
		}
		void Renderer::texture2f(float u, float v) {
			texArray[ptr*2] = u;
			texArray[ptr*2+1] = v;
		}
		void Renderer::normal3f(float vx, float vy, float vz) {
			nomArray[ptr*3] = vx;
			nomArray[ptr*3+1] = vy;
			nomArray[ptr*3+2] = vz;
		}
		void Renderer::vertex2fq(float2 d1, float2 d2, float2 d3, float2 d4) {
			int p=ptr*2;
			vtxArray[p++] = d1.x;
			vtxArray[p++] = d1.y;
			vtxArray[p++] = d3.x;
			vtxArray[p++] = d3.y;
			vtxArray[p++] = d4.x;
			vtxArray[p++] = d4.y;
			
			vtxArray[p++] = d1.x;
			vtxArray[p++] = d1.y;
			vtxArray[p++] = d2.x;
			vtxArray[p++] = d2.y;
			vtxArray[p++] = d3.x;
			vtxArray[p++] = d3.y;
			ptr+=6;
		}
		void Renderer::vertex3fq(float3 d1, float3 d2, float3 d3, float3 d4){
			int p=ptr*3;
			vtxArray[p++] = d1.x;
			vtxArray[p++] = d1.y;
			vtxArray[p++] = d1.z;
			vtxArray[p++] = d3.x;
			vtxArray[p++] = d3.y;
			vtxArray[p++] = d3.z;
			vtxArray[p++] = d4.x;
			vtxArray[p++] = d4.y;
			vtxArray[p++] = d4.z;
			
			vtxArray[p++] = d1.x;
			vtxArray[p++] = d1.y;
			vtxArray[p++] = d1.z;
			vtxArray[p++] = d2.x;
			vtxArray[p++] = d2.y;
			vtxArray[p++] = d2.z;
			vtxArray[p++] = d3.x;
			vtxArray[p++] = d3.y;
			vtxArray[p++] = d3.z;
			ptr+=6;
		}
		void Renderer::color3fq(float3 d1, float3 d2, float3 d3, float3 d4) {
			int p=ptr*4;
			colArray[p++] = d1.x;
			colArray[p++] = d1.y;
			colArray[p++] = d1.z;
			colArray[p++] = 1.0f;
			colArray[p++] = d3.x;
			colArray[p++] = d3.y;
			colArray[p++] = d3.z;
			colArray[p++] = 1.0f;
			colArray[p++] = d4.x;
			colArray[p++] = d4.y;
			colArray[p++] = d4.z;
			colArray[p++] = 1.0f;
			
			colArray[p++] = d1.x;
			colArray[p++] = d1.y;
			colArray[p++] = d1.z;
			colArray[p++] = 1.0f;
			colArray[p++] = d2.x;
			colArray[p++] = d2.y;
			colArray[p++] = d2.z;
			colArray[p++] = 1.0f;
			colArray[p++] = d3.x;
			colArray[p++] = d3.y;
			colArray[p++] = d3.z;
			colArray[p++] = 1.0f;
		}
		void Renderer::color4fq(float4 d1, float4 d2, float4 d3, float4 d4) {
			int p=ptr*4;
			colArray[p++] = d1.x;
			colArray[p++] = d1.y;
			colArray[p++] = d1.z;
			colArray[p++] = d1.w;
			colArray[p++] = d3.x;
			colArray[p++] = d3.y;
			colArray[p++] = d3.z;
			colArray[p++] = d3.w;
			colArray[p++] = d4.x;
			colArray[p++] = d4.y;
			colArray[p++] = d4.z;
			colArray[p++] = d4.w;
			
			colArray[p++] = d1.x;
			colArray[p++] = d1.y;
			colArray[p++] = d1.z;
			colArray[p++] = d1.w;
			colArray[p++] = d2.x;
			colArray[p++] = d2.y;
			colArray[p++] = d2.z;
			colArray[p++] = d2.w;
			colArray[p++] = d3.x;
			colArray[p++] = d3.y;
			colArray[p++] = d3.z;
			colArray[p++] = d3.w;
		}
		void Renderer::texture2fq(float2 d1, float2 d2, float2 d3, float2 d4) {
			int p=ptr*2;
			texArray[p++] = d1.x;
			texArray[p++] = d1.y;
			texArray[p++] = d3.x;
			texArray[p++] = d3.y;
			texArray[p++] = d4.x;
			texArray[p++] = d4.y;
			
			texArray[p++] = d1.x;
			texArray[p++] = d1.y;
			texArray[p++] = d2.x;
			texArray[p++] = d2.y;
			texArray[p++] = d3.x;
			texArray[p++] = d3.y;
		}
		void Renderer::normal3fq(float3 d1, float3 d2, float3 d3, float3 d4){
			int p=ptr*3;
			nomArray[p++] = d1.x;
			nomArray[p++] = d1.y;
			nomArray[p++] = d1.z;
			nomArray[p++] = d3.x;
			nomArray[p++] = d3.y;
			nomArray[p++] = d3.z;
			nomArray[p++] = d4.x;
			nomArray[p++] = d4.y;
			nomArray[p++] = d4.z;
			
			nomArray[p++] = d1.x;
			nomArray[p++] = d1.y;
			nomArray[p++] = d1.z;
			nomArray[p++] = d2.x;
			nomArray[p++] = d2.y;
			nomArray[p++] = d2.z;
			nomArray[p++] = d3.x;
			nomArray[p++] = d3.y;
			nomArray[p++] = d3.z;
		}
		void Renderer::vct(int numVertex, float *v, float *c, float *t) {
			if(mode3D)
			memcpy(vtxArray+ptr*3, v, numVertex*3*sizeof(float));
			else
			memcpy(vtxArray+ptr*2, v, numVertex*2*sizeof(float));
			memcpy(colArray+ptr*4, c, numVertex*4*sizeof(float));
			if(useTex)
				memcpy(texArray+ptr*2, t, numVertex*2*sizeof(float));
			ptr += numVertex;
		}
		void Renderer::rendererUpdate() {
			glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
			if(mode3D) {
				glBufferData( GL_ARRAY_BUFFER, ptr*3*sizeof(float), vtxArray, GL_STATIC_DRAW);
				glVertexPointer(3, GL_FLOAT, 0, NULL);
			}else {
				glBufferData( GL_ARRAY_BUFFER, ptr*2*sizeof(float), vtxArray, GL_STATIC_DRAW);
				glVertexPointer(2, GL_FLOAT, 0, NULL);
			}	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
				glBufferData(GL_ARRAY_BUFFER, ptr*4*sizeof(float), colArray, GL_STATIC_DRAW);
				glColorPointer(4, GL_FLOAT, 0, NULL);
		if(useTex) {
			glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindTexture(GL_TEXTURE_2D, tex);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
			glBufferData(GL_ARRAY_BUFFER, ptr*2*sizeof(float), texArray, GL_STATIC_DRAW);
			glTexCoordPointer(2, GL_FLOAT, 0, NULL);
		}else {
			glDisable(GL_TEXTURE_2D);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}
		if(useNom) {
			glEnableClientState(GL_NORMAL_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
			glBufferData(GL_ARRAY_BUFFER, ptr*3*sizeof(float), nomArray, GL_STATIC_DRAW);
			glNormalPointer(GL_FLOAT, 0, NULL);
		}else {
			glDisableClientState(GL_NORMAL_ARRAY);
		}
		}
		void Renderer::bind() {
			glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
			if(mode3D) {
				glVertexPointer(3, GL_FLOAT, 0, NULL);
			}else {
				glVertexPointer(2, GL_FLOAT, 0, NULL);
			}
				glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
				glColorPointer(4, GL_FLOAT, 0, NULL);

		if(useTex) {
			glEnable(GL_TEXTURE_2D);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glBindTexture(GL_TEXTURE_2D, tex);
				glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
			glTexCoordPointer(2, GL_FLOAT, 0, NULL);
		}else {
			glDisable(GL_TEXTURE_2D);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}
		if(useNom) {
			glEnableClientState(GL_NORMAL_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
			glNormalPointer(GL_FLOAT, 0, NULL);
		}else {
		glDisableClientState(GL_NORMAL_ARRAY);
		}
		}
		void Renderer::rendering(int type) {
			glDrawArrays(type, 0, ptr);
		}
		
		void Renderer::setTexture(unsigned int texid, int tex_width, int tex_height) {
			tex=texid;
			texWidth= tex_width;
			texHeight= tex_height;
		}

		void Renderer::setTextureFormat(int format_width, int format_height) {
			formatWidth=format_width;
			formatHeight=format_height;
		}

		/*Tools*/
		void Renderer::addQuad(int x, int y, int width, int height, unsigned int texPos, float *color) {
			float vertex3D[12] = {
					x+width/2.0f, y-height/2.0f, 0,
					x-width/2.0f, y-height/2.0f, 0,
					x-width/2.0f, y+height/2.0f, 0,
					x+width/2.0f, y+height/2.0f, 0
			};
			float vertex2D[8] = {
					x+width/2.0f, y+height/2.0f,
					x-width/2.0f, y+height/2.0f,
					x-width/2.0f, y-height/2.0f,
					x+width/2.0f, y-height/2.0f
			};
			float texture[8];
			getTexIndexPointer(texPos, texture);
			if(mode3D) vct(4, vertex3D, color, texture);
			else vct(4, vertex2D, color, texture);
		}

		void Renderer::getTexIndexPointer(unsigned int texPos, float *pointer) {
			float x=texPos>>16, y=texPos<<16>>16;
			pointer[0] = x*formatWidth/texWidth;
			pointer[1] = y*formatHeight/texHeight;

			pointer[2] = (x+1)*formatWidth/texWidth;
			pointer[3] = y*formatHeight/texHeight;

			pointer[4] = (x+1)*formatWidth/texWidth;
			pointer[5] = (y+1)*formatHeight/texHeight;

			pointer[6] = x*formatWidth/texWidth;
			pointer[7] = (y+1)*formatHeight/texHeight;
			
			/*pointer[0]=16/256.0;
			pointer[1]=0;
			pointer[2]=32/256.0;
			pointer[3]=0;
			pointer[4]=32/256.0;
			pointer[5]=16/256.0;
			pointer[6]=16/256.0;
			pointer[7]=16/256.0;*/
		}