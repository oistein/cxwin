//
// Created by Øistein Sletten Løvik on 22/02/2017.
//

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#ifndef CXWIN_alloc
#include <stdlib.h>
#define CXWIN_FREE(x) (free(x))
#define CXWIN_CALLOC(x) (calloc(1, sizeof(x)))
#endif

#ifdef __APPLE__
    #define CXWIN_APPLE 1
#elif __linux__
    #define CXWIN_LINUX 1
#elif _WIN32
    #define CXWIN_WINDOWS 1
#endif

typedef struct cxwin_window cxwin_window;

cxwin_window* cxwin_create_window();
void cxwin_next_event(cxwin_window* win);
void cxwin_terminate(cxwin_window* win);

#pragma region macos
#if defined(CXWIN_APPLE)

#include <objc/message.h>
#include <CoreGraphics/CoreGraphics.h>
#include <assert.h>

extern id NSApp;

typedef struct AppDel {
    Class isa;
    id window;
} AppDelegate;

Class AppDelClass;

struct cxwin_window {
    uint16_t width;
    uint16_t height;
};

static void cxwin_thread_noop(Class self, SEL _cmd, id obj) {

}

static BOOL AppDel_didFinishLaunching(AppDelegate *self, SEL _cmd, id notification) {
    objc_msgSend(NSApp, sel_getUid("stop:"), NULL);

    id pool = objc_msgSend((id)objc_getClass("NSAutoreleasePool"), sel_getUid("alloc"));
    objc_msgSend(pool, sel_getUid("init"));

    id event = objc_msgSend((id)objc_getClass("NSEvent"),
                            sel_getUid("otherEventWithType:location:modifierFlags:timestamp:windowNumber:context:subtype:data1:data2:"),
                            15,
                            CGPointMake(0, 0),
                            0,
                            0,
                            0,
                            NULL,
                            0,
                            0,
                            0);

    objc_msgSend(NSApp, sel_getUid("postEvent:atStart:"), event, 1);
    objc_msgSend(pool, sel_getUid("drain"));
    return YES;
}

static void RunApplication() {
    objc_msgSend((id)objc_getClass("CXWINApplication"), sel_getUid("sharedApplication"));

    if (NSApp == NULL) {
        fprintf(stderr,"Failed to initialized NSApplication...  terminating...\n");
        return;
    }

    id self = (id) objc_getClass("NSThread");
    objc_msgSend(self,
                 sel_getUid("detachNewThreadSelector:toTarget:withObject:"),
                 sel_getUid("cxwin_thread_noop:"),
                 NSApp,
                 NULL);

    objc_msgSend(NSApp, sel_getUid("setActivationPolicy:"), 0);

    id appDelObj = objc_msgSend((id)objc_getClass("AppDelegate"), sel_getUid("alloc"));
    appDelObj = objc_msgSend(appDelObj, sel_getUid("init"));

    objc_msgSend(NSApp, sel_getUid("setDelegate:"), appDelObj);
    objc_msgSend(NSApp, sel_getUid("run"));

    id window = objc_msgSend((id)objc_getClass("NSWindow"), sel_getUid("alloc"));

    window = objc_msgSend(window, sel_getUid("initWithContentRect:styleMask:backing:defer:"),
                          CGRectMake(0,0,1024,460), 0, 0, false);

    objc_msgSend(window, sel_getUid("becomeFirstResponder"));
    objc_msgSend(window, sel_getUid("makeKeyAndOrderFront:"), self);
}

cxwin_window* cxwin_create_window() {

    cxwin_window* win = CXWIN_CALLOC(sizeof(cxwin_window));

    Class cxWinClass = objc_allocateClassPair(objc_getClass("NSApplication"), "CXWINApplication", 0);
    class_addMethod(cxWinClass, sel_getUid("cxwin_thread_noop:"), (IMP)cxwin_thread_noop, "@:@");
    objc_registerClassPair(cxWinClass);

    AppDelClass = objc_allocateClassPair((Class)objc_getClass("NSObject"), "AppDelegate", 0);
    class_addMethod(AppDelClass, sel_getUid("applicationDidFinishLaunching:"), (IMP)AppDel_didFinishLaunching, "i@:@");
    objc_registerClassPair(AppDelClass);

    RunApplication();
    return win;
}

void cxwin_next_event(cxwin_window* win) {
    id event = objc_msgSend(NSApp, sel_getUid("nextEventMatchingMask:untilDate:inMode:dequeue:"),
                            ULONG_MAX,
                            objc_msgSend((id)objc_getClass("NSDate"), sel_getUid("distantFuture")),
                            kCFRunLoopDefaultMode,
                            1);
    objc_msgSend(NSApp, sel_getUid("sendEvent:"), event);
}

void cxwin_terminate(cxwin_window* win) {
    assert(win);

    CXWIN_FREE(win);
}

#endif
#pragma endregion macos

#ifdef __cplusplus
}
#endif
