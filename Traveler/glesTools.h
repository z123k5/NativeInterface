/*
贴吧:z123k5 QQ:1994918916

Version 1.3_b1
时间戳:2019-1-30
历史更新:
1.2.2: 2016-11-5 21:16
1.1.0: 2016-6-25

GT库为了方便用户在Android系统上使用NativeActivity编程，特意增加这些函数。
使用GT库必须以void android_main(android_app*)
入口，而不是int main()
具体说明请见此文档。
本库自带一些附属工具:Renderer，Control。欢迎使用
一个简单的示例:
///////////////////////
#include <glesTools.h>//包含GT库头文件
gtWindow mainWindow;

//注意现在主函数（入口函数)是android_main()
void android_main(android_app* state)
{
	glesInitWindow(&mainWindow, state);//初始化窗口
	while(!glesShouldClose(&mainWindow))//循环
	{
		//这里进行OpenGLES绘图
		
		glesPollEvents(&mainWindow);//处理窗口消息
	}
	glesTerminate(&mainWindow);//关闭窗口
}
///////////////////////
*/

#ifndef _GLESTOOLS_H_
#define _GLESTOOLS_H_


#include <EGL/egl.h>
#include <GLES/gl.h>
#include <android_native_app_glue.h>


//OpenGL ES窗口信息
struct gtWindow
{
	EGLConfig config;
	EGLSurface surface;
	EGLContext context;
	EGLDisplay display;
	android_app *app;
	int width, height;
	bool run;
};
//触点信息
struct gtContact
{
	int x, y;//即时位置
	int x0, y0;//按下时的位置
	int xl, yl;//上一次位置
	int id;//触点标识
	
	gtContact(): id(-1) {}//构造函数
	
	//判断两个触点的id是否相等
	bool operator == (gtContact &c) {
		if(c.id==id)
		return true;
		return false;
	}
	
	//库内获取触点数据，无需用户调用
	void getStart(AInputEvent *event, int indexID, int index) {
		id=indexID;
		xl=x0=x=AMotionEvent_getX(event, index);
		yl=y0=y=AMotionEvent_getY(event, index);
	}
	void getMove(AInputEvent *event, int index) {
		xl=x; yl=y;
		x=AMotionEvent_getX(event, index);
		y=AMotionEvent_getY(event, index);
	}
};
//杂项
struct float2
{
	float x, y;
};
struct float3
{
	float x, y, z;
};
struct float4
{
	float x, y, z, w;
};

//局部变量/常量
#define PI 3.141592653589793
#define MAX_CONTACT 10
extern gtContact gt_contact[MAX_CONTACT];
extern gtContact gt_pcontact;
extern gtContact gt_rcontact;
extern bool gt_isPtrue;
extern bool gt_isRtrue;
extern int gt_numcontact;
extern int pf;

//一些有用的函数
//延时函数，单位毫秒
void glsleep(int milliseconds);
//单位化矩阵，高级用户可以使用
void glMakeIdentityf(GLfloat m[16]);
//透视投影
void glPerspectivef(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar);
//打开正投影
void glOrthoBegin(gtWindow *window);
//关闭正投影
void glOrthoEnd();

//GLES主要函数
//初始化程序和OpenGLES窗口
void glesInitWindow(gtWindow *window, android_app *state);
//处理窗口消息(窗口更新)
void glesPollEvents(gtWindow *window);
//检查窗口是否需要关闭
bool glesShouldClose(gtWindow *window);
//关闭窗口
void glesTerminate(gtWindow *window);

//系统函数，无需用户调用
//应用程序循环函数
void glesAppCmd(android_app *app, int cmd);
//应用程序输入函数
int glesAppInput(android_app *app, AInputEvent *event);

extern void (*appCmdSaveState)();

extern void (*appCmdInitWindow)();

extern void (*appCmdTermWindow)();

extern void (*appCmdGainedFocus)();

extern void (*appCmdLostFocus)();

//回调函数，需用户手动调用
//数据保存回调函数
void glesCmdSaveStateFunc(void (*func)());
//初始化回调函数
void glesCmdInitWindowFunc(void (*func)());
//销毁回调函数
void glesCmdTermWindowFunc(void (*func)());
//获得焦点回调函数
void glesCmdGainedFocusFunc(void (*func)());
//失去焦点回调函数
void glesCmdLostFocusFunc(void (*func)());

//输入检测函数
//返回范围内瞬时按下的触点数量
int rangePress(float2 p1, float2 p2);
//返回范围内按着的触点数量
int rangeKeep(float2 p1, float2 p2);
//返回范围内瞬时松开的触点数量
int rangeRelease(float2 p1, float2 p2);
//返回范围内瞬时按下的触点
gtContact contactRangePress(float2 p1, float2 p2);
//返回范围内按着的触点信息
gtContact contactRangeKeep(float2 p1, float2 p2);
//返回瞬时松开的触点信息
gtContact contactRangeRelease(float2 p1, float2 p2);
//返回ID所对的触点信息，该函数通常用在跟踪触点
gtContact getContactByID(int cid);


#endif