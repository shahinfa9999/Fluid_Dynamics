#ifndef VIEW_DELEGATE_HPP
#define VIEW_DELEGATE_HPP


#include "../metal-cpp/Metal/Metal.hpp"
#include "../include/rendering.hpp"
#include <MetalKit/MetalKit.hpp>

class MyMTKViewDelegate : public MTK::ViewDelegate
{
    public:
        MyMTKViewDelegate( MTL::Device* pDevice );
        virtual ~MyMTKViewDelegate() override;
        virtual void drawInMTKView( MTK::View* pView ) override;

    private:
        Renderer* _pRenderer;
};

#endif //VIEW_DELEGATE_HPP