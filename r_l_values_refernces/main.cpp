#include <iostream>

class Widget
{
public:
    Widget()
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }
    
    Widget(const Widget & widget)
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }
    Widget(Widget && widget)
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }
    
    auto operator =(Widget const & widget) -> Widget &
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        return *this;
        }
        
        auto operator =(Widget && widget) -> Widget &
        {
            std::cout << __PRETTY_FUNCTION__ << std::endl;
            return *this;
        }
        
        };
        
        void f(Widget & w)
        {
            std::cout << __PRETTY_FUNCTION__ << std::endl;
        }
        
        void f(Widget && w)
        {
            std::cout << __PRETTY_FUNCTION__ << std::endl;
        }
        
        template<typename T> using remove_reference_t = typename std::remove_reference<T>::type;
        
        template <typename T>
        decltype(auto)
        move(T&& param) // выполняет безусловное приведение своего аргумнта к rvalue
        {
            return static_cast<remove_reference_t<T>&&>(param); // ? T - lvalue-ссылка => T && - становится rvalue-ссылкой
        }
        
        Widget simpleMakeWidget()
        {
            return Widget();
        }
        
        Widget complexMakeWidget()
        {
            Widget w1 = Widget();
            Widget w2 = Widget();
            
            return rand() ? w1 : w2;
        }
        
        Widget & makeWidgetAndReturnLvalueLink()
        {
            Widget * w1 = new Widget();
            Widget * w2 = new Widget();
            
            return rand() ? *w1 : *w2;
        }
        
        Widget && makeWidgetAndReturnRvalueLink()
        {
            Widget w1 = Widget();
            Widget w2 = Widget();
            
            return rand() ? move(w1) : move(w2);
        }
        
        // rvalue-ссылки возвращаемые функциями являются rvalue
        
        int main(int argc, const char * argv[]) {
            
            //lvalue-объект
            Widget w;
            {
                Widget lvo { w };
                Widget & lvo_lvl { w };
                //        Widget && lvo_rvl { w };// rvalue-ссылка не может быть инициализирована lvalue-объектом
            }
            f(w);
            
            //lvalue-объект типа lvalue-ссылка
            Widget & x { w };
            {
                Widget lvo {x};
                Widget & lvo_lvl {x};
                //        Widget && lvo_rvl { x };// rvalue-ссылка не может быть инициализирована lvalue-объектом
            }
            f(x);
            
            //lvalue-объект типа rvalue-ссылка
            Widget && y {move(w)};
            {
                Widget lvo { move(y) };
                Widget & lvo_lvl {y};
                //        Widget && lvo_rvl = { y };// rvalue-ссылка не может быть инициализирована lvalue-объектом
            }
            f(y);
            
            //rvalue-объект
            {
                Widget lvo { complexMakeWidget() };
                Widget const & lvo_lvl { complexMakeWidget() };// lvalue-ссылка может быть инициализирована rvalue-объектом, если только она является константной
                Widget && lvo_rvl { complexMakeWidget() };
            }
            {
                Widget lvo { simpleMakeWidget() };// RVO
            }
            f(complexMakeWidget());
            
            //rvalue-объект типа lvalue-ссылка
            {
                Widget lvo { makeWidgetAndReturnLvalueLink() };
                Widget & lvo_lvl { makeWidgetAndReturnLvalueLink() };
                //        Widget && lvo_rvl = makeWidgetAndReturnLvalueLink(); // rvalue-ссылка не может быть инициализирована объектом типа lvalue-ссылка
            }
            f(makeWidgetAndReturnLvalueLink());
            
            //rvalue-объект типа rvalue-ссылка
            {
                Widget lvo { makeWidgetAndReturnRvalueLink() };
                Widget const & lvo_lvl { makeWidgetAndReturnRvalueLink() }; // lvalue-ссылка может быть инициализирована rvalue-объектом, если только она является константной
                Widget && lvo_rvl { makeWidgetAndReturnRvalueLink() };
            }
            f(makeWidgetAndReturnRvalueLink());
            
            return 0;
        }