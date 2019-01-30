#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <GLES/gl.h>
#include "glesTools.h"


//OpenGLES辅助库:渲染器
class Renderer
	{
	public:
		float *vtxArray, *colArray, *texArray, *nomArray;//顶点，颜色，纹理
		unsigned int arrSize;//缓冲区大小
		unsigned int ptr;
		GLuint vbo[4];//顶点，颜色，纹理
		unsigned int tex;//纹理编号
		int texWidth;//纹理宽
		int texHeight;//纹理长
		float formatWidth;//纹理格式化长
		float formatHeight;//纹理格式化宽
		bool mode3D, useTex, useNom;//是否使用3d模式，是否使用纹理，是否使用法线向量
		
	//public:
		void init(int maxVertexNum=65536, bool useTexture=false, bool open3dMode=false, bool useNormal=false);
		Renderer();
		~Renderer();

		void clear();//清除
		void vertex2f(float x, float y);//最后指定
		void vertex3f(float x, float y, float z);//最后指定
		void color3f(float r, float g, float b);
		void color4f(float r, float g, float b, float a);
		void normal3f(float vx, float vy, float vz);
		void texture2f(float u, float v);
//几个fq后缀的函数是以四边形为图元指定的
		void vertex2fq(float2 d1, float2 d2, float2 d3, float2 d4);//最后指定
		void vertex3fq(float3 d1, float3 d2, float3 d3, float3 d4);//最后指定
		void color3fq(float3 d1, float3 d2, float3 d3, float3 d4);
		void color4fq(float4 d1, float4 d2, float4 d3, float4 d4);
		void texture2fq(float2 d1, float2 d2, float2 d3, float2 d4);
		void normal3fq(float3 d1, float3 d2, float3 d3, float3 d4);
//vct很少用
		void vct(int numVertex, float *v, float *c, float *t);
		void rendererUpdate();//每更新了缓冲区数据，就要调用一次
		void bind();//多个渲染器时要绑定一下
		void rendering(int type);//渲染
		
		void setTexture(unsigned int texid, int tex_width, int tex_height);//设置纹理
		void setTextureFormat(int format_width, int format_height);//格式化纹理，把纹理切成小块，每个小块的长宽就是格式化的长宽

		/*Tools*/
		void addQuad(int x, int y, int width, int height, unsigned int texPos, float *color);//没用

		void getTexIndexPointer(unsigned int texPos, float *pointer);//获取第texPos个小块(0开始数)的纹理数据
	};

#endif