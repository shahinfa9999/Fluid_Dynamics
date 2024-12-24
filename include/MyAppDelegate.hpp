#ifndef App_DELEGATE_HPP
#define App_DELEGATE_HPP

#include <MetalKit/MetalKit.hpp>
#include <Foundation/Foundation.hpp>
#include <AppKit/NSApplication.hpp>
#include <AppKit/AppKit.hpp>

class MyAppDelegate : public NS::ApplicationDelegateProtocol
{
    public:
        ~MyAppDelegate();

        NS::Menu* createMenuBar();

        virtual void applicationWillFinishLaunching( NS::Notification* pNotification ) override;
        virtual void applicationDidFinishLaunching( NS::Notification* pNotification ) override;
        virtual bool applicationShouldTerminateAfterLastWindowClosed( NS::Application* pSender ) override;

    private:
        NS::Window* _pWindow;
        MTK::View* _pMtkView;
        MTL::Device* _pDevice;
        MyMTKViewDelegate* _pViewDelegate = nullptr;
};

#endif // App_DELEGATE_HPP
