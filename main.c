#include <stdio.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <complex.h>
#include <stdbool.h>
 
#define WIDTH 1200
#define HEIGHT 650
#define MAXSTEPS 84400
 
complex multiplyComp(complex a, complex b) {
    complex c = (creal(a)*creal(b) - cimag(a)*cimag(b)) + (creal(a)*cimag(b) + cimag(a)*creal(b));
    return c;
}
complex subtractComp(complex a, complex b) {
    complex c = (creal(a)+cimag(a))-(creal(b)+cimag(b));
    return c;
}
bool in_Mandelbrot(complex c) {
    complex z;
    __real__ z = 0;
    __imag__ z = 0;
    int i=0;
    while(true){
        if(cabs(z)>1.0) return false;
        else if (i>MAXSTEPS) return true;
        z = z*z;
        z=c-z;
        i++;
    }
}
complex window_to_complex(int i, int j, int W, int H, double x, double y,
     double width, double height)
{
    complex c;
    //c = (x + width * i/W - width/2) + (y + height * j/H - height/2);
    __real__ c = (x+width*i/W - width/2);
    __imag__ c = (y+height * j/H - height/2);
    return c;
}
 
static void draw_pixel(Display *theDisplay, Window theWindow,
     long colour, int i, int j) {
    GC gc = XCreateGC(theDisplay, theWindow, 0, NULL);
    XSetForeground(theDisplay, gc, colour);
    XFillRectangle(theDisplay, theWindow, gc, i,j,1,1);
    XFreeGC(theDisplay, gc);
}
static void draw_Mandelbrot(Display *theDisplay,
    Window theWindow, long black, long white, int W, int H,
     double x, double y, double width, double height)
{
    complex c;
    for(int i=0;i<WIDTH;i++) {
        for(int j=0;j<HEIGHT;j++) {
            c=window_to_complex(i,j,W,H,x,y,width,height);
            if(in_Mandelbrot(c)) draw_pixel(theDisplay, theWindow, black, i,j);
            else draw_pixel(theDisplay, theWindow, white, i,j);
        }
    }
}
int main(int argc, char*argv[]) {
    XtAppContext context;
    int theScreen;
    Display *theDisplay;
    Window theWindow;
    long white, black;
    double x,y,width, height;
    Widget widget = XtVaAppInitialize(&context, "XMandel", NULL,
        0,&argc, argv, NULL, NULL);
    XtVaSetValues(widget, XtNheight, HEIGHT, XtNwidth, WIDTH, NULL);
    XtRealizeWidget(widget);
    theDisplay = XtDisplay(widget);
    theWindow = XtWindow(widget);
    theScreen = DefaultScreen(theDisplay);
    white = "CIELuv:30.0/12.0/27.0";
    black = BlackPixel(theDisplay, theScreen);
    printf("Enter: ");
    if(scanf("%lf%lf%lf%lf", &x, &y, &width,&height)!=4) {
        printf("Sorry cannot render");
    }
    else {
        draw_Mandelbrot(theDisplay, theWindow,
            black,white, WIDTH, HEIGHT,
            x,y,width,height);
        XtAppMainLoop(context);
    }
    return 0;
}