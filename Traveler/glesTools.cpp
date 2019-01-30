/*
Copyright by 贴吧:z123k5 QQ:void
All right reserved.

版本 1.2.2
时间戳:2016-11-5 21:16
历史更新:
1.1.0: 2016-6-25

详情看"glesTools.h"
*/

#include <math.h>
#include "glesTools.h"


//局部变量/常量
#define MAX_CONTACT 10
gtContact gt_contact[MAX_CONTACT];
int gt_numcontact=0;
gtContact gt_pcontact;
gtContact gt_rcontact;
bool gt_isPtrue=false;
bool gt_isRtrue=false;
int pf=0;


//一些有用的函数
//延时函数，单位毫秒
void glsleep(int milliseconds)
{
	for(int i=0; i<milliseconds* 500000000; i++);
}
//单位化矩阵，高级用户可以使用
void glMakeIdentityf(GLfloat m[16])
{
    m[0+4*0] = 1; m[0+4*1] = 0; m[0+4*2] = 0; m[0+4*3] = 0;
    m[1+4*0] = 0; m[1+4*1] = 1; m[1+4*2] = 0; m[1+4*3] = 0;
    m[2+4*0] = 0; m[2+4*1] = 0; m[2+4*2] = 1; m[2+4*3] = 0;
    m[3+4*0] = 0; m[3+4*1] = 0; m[3+4*2] = 0; m[3+4*3] = 1;
}
//透视投影
void glPerspectivef(GLfloat fovy,GLfloat aspect,GLfloat zNear,GLfloat zFar)
{
    GLfloat ymax,ymin,xmax,xmin;
    ymax = zNear*tan(fovy*PI/360.0f);
    ymin = -ymax;
    xmax = ymax*aspect;
    xmin = -xmax;

    glFrustumf(xmin, xmax, ymin, ymax, zNear, zFar);
}
//打开正投影
void glOrthoBegin(gtWindow *window)
{
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrthof(0, window->width, window->height, 0, -10.0f, 10.0f);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}
//关闭正投影
void glOrthoEnd()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
}

//GT主要函数
//初始化程序和OpenGLES窗口
void glesInitWindow(gtWindow *window, android_app *state)
{
	if(window->display!=NULL)
	return;
	app_dummy();
	window->app=state;
	window->width=64;
	window->height=48;
	window->run=false;
	state->userData=window;
	state->onAppCmd=glesAppCmd;
	state->onInputEvent = glesAppInput;
	while(!window->display) glesPollEvents(window);
}
//处理窗口消息(窗口更新)
void glesPollEvents(gtWindow *window)
{
	int ident;
	int events;
	android_poll_source *source;
	
	eglSwapBuffers(window->display, window->surface);
	gt_isPtrue=gt_isRtrue=false;
	
	while ((ident = ALooper_pollAll(0, NULL, &events, (void**)&source)) >= 0) {
		if (source != NULL)
		source->process(window->app, source);
		if (window->app->destroyRequested != 0) {
			window->run=false;
			return;
		}
	}
}
//检查窗口是否需要关闭
bool glesShouldClose(gtWindow *window)
{
	if(!window->run)
	return true;
	return false;
}
//关闭窗口
void glesTerminate(gtWindow *window)
{
	if (window->display != EGL_NO_DISPLAY) {
		eglMakeCurrent(window->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		if (window->context != EGL_NO_CONTEXT) {
			eglDestroyContext(window->display, window->context);
		}
		if (window->surface != EGL_NO_SURFACE) {
			eglDestroySurface(window->display, window->surface);
		}
		eglTerminate(window->display);
	}
	window->display = EGL_NO_DISPLAY;
	window->context = EGL_NO_CONTEXT;
	window->surface = EGL_NO_SURFACE;
}

//系统函数(无需用户调用)
//创建OpenGLES上下文
void glesCreateWindow(gtWindow *window)
{
	//AConfiguration_setOrientation(window->app->config, ACONFIGURATION_ORIENTATION_LAND);
	
	const EGLint attribs[] = {
		EGL_SURFACE_TYPE,
		EGL_WINDOW_BIT,
		EGL_BLUE_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_RED_SIZE, 8,
		EGL_DEPTH_SIZE, 8,
		EGL_NONE
	};
	EGLint format;
	EGLint numConfigs;
	window->display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	eglInitialize(window->display, 0, 0);
	eglChooseConfig(window->display, attribs, &window->config, 1, &numConfigs); 
	eglGetConfigAttrib(window->display, window->config, EGL_NATIVE_VISUAL_ID, &format);
	ANativeWindow_setBuffersGeometry(window->app->window, 0, 0, format);
	window->surface = eglCreateWindowSurface(window->display, window->config, window->app->window, NULL);
//OpenGL ES Context版本 1.0/2.0切换，3.0和2.0都是采用2.0的Context。
//注意：2.0的Context只支持shader渲染！
#if 0
	EGLint contextAtt[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };
	window->context = eglCreateContext(window->display, window->config, 0, contextAtt);
#else
	window->context = eglCreateContext(window->display, window->config, 0, 0);
#endif
	if (eglMakeCurrent(window->display, window->surface, window->surface, window->context) == EGL_FALSE) {
			//LOGW("Unable to eglMakeCurrent");
			window->run=false;
			return;
	}
	eglQuerySurface(window->display, window->surface, EGL_WIDTH, &(window->width));
	eglQuerySurface(window->display, window->surface, EGL_HEIGHT, &(window->height));
		
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
	
	window->run=true;
}
//应用程序循环函数
void glesAppCmd(android_app *app, int cmd)
{
	switch (cmd) {
		case APP_CMD_SAVE_STATE:
		if(appCmdSaveState!=NULL) {
			appCmdSaveState();
		}
		break;
		case APP_CMD_INIT_WINDOW:
		glesCreateWindow((gtWindow*)app->userData);
		if(appCmdInitWindow!=NULL) {
			appCmdInitWindow();
		}
		break;
		case APP_CMD_TERM_WINDOW:
		if(appCmdTermWindow!=NULL) {
			appCmdTermWindow();
		}
		glesTerminate((gtWindow*)app->userData);
		break;
		case APP_CMD_GAINED_FOCUS:
		if(appCmdGainedFocus!=NULL) {
			appCmdGainedFocus();
		}
		break;
		case APP_CMD_LOST_FOCUS:
		if(appCmdLostFocus!=NULL) {
			appCmdLostFocus();
		}
		break;
	}
}
//应用程序输入函数
//目前还有点BUG，欢迎提出意见！
int glesAppInput(android_app *app, AInputEvent *event)
{
	int evtType = AInputEvent_getType(event);
	switch (evtType) {
		case AINPUT_EVENT_TYPE_KEY:
		
		break;
		case AINPUT_EVENT_TYPE_MOTION:
		int sourceId = AInputEvent_getSource(event);
		if(AINPUT_SOURCE_TOUCHSCREEN == sourceId) {
			int eAction = AMotionEvent_getAction(event);
			int eIndex = eAction>>8;
			int iID=-1;
			int cntCount=AMotionEvent_getPointerCount(event);
			
			//AMOTION_EVENT_ACTION_POINTER_INDEX_MASK: 0xff00
			//AMOTION_EVENT_ACTION_MASK: 0x00ff
			if(cntCount<=MAX_CONTACT)
			switch (eAction & AMOTION_EVENT_ACTION_MASK) {
				case AMOTION_EVENT_ACTION_DOWN:
				case AMOTION_EVENT_ACTION_POINTER_DOWN:
				{
					iID=AMotionEvent_getPointerId(event, eIndex);
					gt_contact[iID].getStart(event, iID, eIndex);
					gt_pcontact=gt_contact[iID];
					gt_numcontact++;
					gt_isPtrue=true;
				}
				break;
				case AMOTION_EVENT_ACTION_CANCEL:
				case AMOTION_EVENT_ACTION_UP:
				case AMOTION_EVENT_ACTION_POINTER_UP:
				{
					iID=AMotionEvent_getPointerId(event, eIndex);
					gt_rcontact=gt_contact[iID];
					gt_contact[iID].id=-1;
					gt_numcontact--;
					if(gt_numcontact<0) gt_numcontact=0;
					gt_isRtrue=true;
				}
				break;
				case AMOTION_EVENT_ACTION_MOVE:
				{
					for(int i=0; i<cntCount; i++) {
						iID=AMotionEvent_getPointerId(event, i);
						if(gt_contact[iID].id!=-1)
						gt_contact[iID].getMove(event, i);
					}
					//gt_contact[eIndex].getMove(event, eIndex);
					/*
					gtContact c[MAX_CONTACT];
					int i, j;
					size_t num = AMotionEvent_getPointerCount(event);
					for(i=0; i<num; i++) {
						c[i].get(event, i);
						for(j=0; j<gt_numcontact; j++) {
							if(gt_contact[j]==c[i]) {
								c[i].xl=gt_contact[i].xl;
								c[i].yl=gt_contact[i].yl;
								c[i].xo=c[i].x-c[i].xl;
								c[i].yo=c[i].y-c[i].yl;
								break;
							}
						}
						if(j==gt_numcontact) {
							c[i].xl=c[i].x;
							c[i].yl=c[i].y;
							c[i].xo=c[i].yo=0;
							gt_pcontact=c[i];
							gt_isPtrue=true;
						}
					}
					for(i=0; i<gt_numcontact; i++) {
						for(j=0; j<num; j++) {
							if(c[j]==gt_contact[i])
							break;
						}
						if(j==num) {
							gt_rcontact=gt_contact[i];
							gt_isRtrue=true;
						}
						
					}
					for(i=0; i<num; i++) {
						gt_contact[i]=c[i];
					}
					gt_numcontact=num;
					*/
				}
				break;
				/*case AMOTION_EVENT_ACTION_POINTER_DOWN:
				case AMOTION_EVENT_ACTION_POINTER_UP:
				{
					int i;
					int num=AMotionEvent_getPointerCount(event);
					for(i=0; i<num; i++)
					gt_contact[i].get(event, i);
					gt_numcontact=num;
					for(i=num-1; i<MAX_CONTACT; i++)
					gt_contact[i].xl=-1;
				}
				break;*/
			}
		}else if(AINPUT_SOURCE_TRACKBALL == sourceId) {
			
		}
		break;
	}
	return 0;
}

void (*appCmdSaveState)()=NULL;

void (*appCmdInitWindow)()=NULL;

void (*appCmdTermWindow)()=NULL;

void (*appCmdGainedFocus)()=NULL;

void (*appCmdLostFocus)()=NULL;

//回调函数，需用户手动调用
//数据保存回调函数
void glesCmdSaveStateFunc(void (*func)()) {
	appCmdSaveState=func;
}
//初始化回调函数
void glesCmdInitWindowFunc(void (*func)()) {
	appCmdInitWindow=func;
}
//销毁回调函数
void glesCmdTermWindowFunc(void (*func)()) {
	appCmdTermWindow=func;
}
//获得焦点回调函数
void glesCmdGainedFocusFunc(void (*func)()) {
	appCmdGainedFocus=func;
}
//失去焦点回调函数
void glesCmdLostFocusFunc(void (*func)()) {
	appCmdLostFocus=func;
}

//输入检测函数
//返回范围内瞬时按下的触点数量
int rangePress(float2 p1, float2 p2)
{
	float temp, count=0;
	if (p1.x>p2.x) {
		temp=p1.x;
		p1.x=p2.x;
		p2.x=temp;
	}
	if (p1.y>p2.y) {
		temp=p1.y;
		p1.y=p2.y;
		p2.y=temp;
	}
	if(gt_isPtrue)
	if(gt_pcontact.x>=p1.x && gt_pcontact.x<=p2.x)
	if(gt_pcontact.y>=p1.y && gt_pcontact.y<=p2.y)
	count++;
	return count;
}
//返回范围内按着的触点数量
int rangeKeep(float2 p1, float2 p2)
{
	float temp, count=0;
	if (p1.x>p2.x) {
		temp=p1.x;
		p1.x=p2.x;
		p2.x=temp;
	}
	if (p1.y>p2.y) {
		temp=p1.y;
		p1.y=p2.y;
		p2.y=temp;
	}
	for(int i=0; i<gt_numcontact; i++)
	if(gt_contact[i].x>=p1.x && gt_contact[i].x<=p2.x)
	if(gt_contact[i].y>=p1.y && gt_contact[i].y<=p2.y)
	count++;
	return count;
}
//返回范围内瞬时松开的触点数量
int rangeRelease(float2 p1, float2 p2)
{
	float temp, count=0;
	if (p1.x>p2.x) {
		temp=p1.x;
		p1.x=p2.x;
		p2.x=temp;
	}
	if (p1.y>p2.y) {
		temp=p1.y;
		p1.y=p2.y;
		p2.y=temp;
	}
	if(gt_isRtrue)
	if(gt_rcontact.x>=p1.x && gt_rcontact.x<=p2.x)
	if(gt_rcontact.y>=p1.y && gt_rcontact.y<=p2.y)
	count++;
	return count;
}
//返回范围内瞬时按下的触点信息
gtContact contactRangePress(float2 p1, float2 p2)
{
	float temp, count=0;
	if (p1.x>p2.x) {
		temp=p1.x;
		p1.x=p2.x;
		p2.x=temp;
	}
	if (p1.y>p2.y) {
		temp=p1.y;
		p1.y=p2.y;
		p2.y=temp;
	}
	if(gt_pcontact.x>=p1.x && gt_pcontact.x<=p2.x)
	if(gt_pcontact.y>=p1.y && gt_pcontact.y<=p2.y)
	return gt_pcontact;
	gtContact nullContact;
	return nullContact;
}
//返回范围内按着的触点信息
gtContact contactRangeKeep(float2 p1, float2 p2)
{
	float temp, count=0;
	if (p1.x>p2.x) {
		temp=p1.x;
		p1.x=p2.x;
		p2.x=temp;
	}
	if (p1.y>p2.y) {
		temp=p1.y;
		p1.y=p2.y;
		p2.y=temp;
	}
	for(int i=0; i<gt_numcontact; i++)
	if(gt_contact[i].x>=p1.x && gt_contact[i].x<=p2.x)
	if(gt_contact[i].y>=p1.y && gt_contact[i].y<=p2.y)
	return gt_contact[i];
	gtContact nullContact;
	return nullContact;
}
//返回范围内瞬时松开的触点信息
gtContact contactRangeRelease(float2 p1, float2 p2)
{
	float temp, count=0;
	if (p1.x>p2.x) {
		temp=p1.x;
		p1.x=p2.x;
		p2.x=temp;
	}
	if (p1.y>p2.y) {
		temp=p1.y;
		p1.y=p2.y;
		p2.y=temp;
	}
	if(gt_rcontact.x>=p1.x && gt_rcontact.x<=p2.x)
	if(gt_rcontact.y>=p1.y && gt_rcontact.y<=p2.y)
	return gt_rcontact;
	gtContact nullContact;
	return nullContact;
}
//返回ID所对的触点信息，该函数通常用在跟踪触点
gtContact getContactByID(int cid)
{
	return gt_contact[cid];
	/*if(gt_isPtrue)
	if(gt_pcontact.id==cid)
	return gt_pcontact;
	if(gt_isRtrue)
	if(gt_rcontact.id==cid)
	return gt_rcontact;
	for(int i=0; i<gt_numcontact; i++) {
		if(gt_contact[i].id==cid)
		return gt_contact[i];
	}
	gtContact nullContact;
	return nullContact;*/
}