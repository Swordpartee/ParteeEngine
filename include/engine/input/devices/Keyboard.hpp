#pragma once

#include <Windows.h>
#include <stdexcept>

#include "engine/input/InputDevice.hpp"
#include "engine/input/InputBinding.hpp"

namespace parteeengine::input {

    class Keyboard : public InputDeviceCRTP<Keyboard> {
    public:
        Keyboard() = default;
        ~Keyboard() override = default;

        void poll() override {};

        bool isActive(InputBinding binding) const override;

        float getAnalog(InputBinding binding) const override;
        
    };
    
    namespace bindings {
        inline const InputBinding KeyA{Keyboard::getId(), 'A'};
        inline const InputBinding KeyB{Keyboard::getId(), 'B'};
        inline const InputBinding KeyC{Keyboard::getId(), 'C'};
        inline const InputBinding KeyD{Keyboard::getId(), 'D'};
        inline const InputBinding KeyE{Keyboard::getId(), 'E'};
        inline const InputBinding KeyF{Keyboard::getId(), 'F'};
        inline const InputBinding KeyG{Keyboard::getId(), 'G'};
        inline const InputBinding KeyH{Keyboard::getId(), 'H'};
        inline const InputBinding KeyI{Keyboard::getId(), 'I'};
        inline const InputBinding KeyJ{Keyboard::getId(), 'J'};
        inline const InputBinding KeyK{Keyboard::getId(), 'K'};
        inline const InputBinding KeyL{Keyboard::getId(), 'L'};
        inline const InputBinding KeyM{Keyboard::getId(), 'M'};
        inline const InputBinding KeyN{Keyboard::getId(), 'N'};
        inline const InputBinding KeyO{Keyboard::getId(), 'O'};
        inline const InputBinding KeyP{Keyboard::getId(), 'P'};
        inline const InputBinding KeyQ{Keyboard::getId(), 'Q'};
        inline const InputBinding KeyR{Keyboard::getId(), 'R'};
        inline const InputBinding KeyS{Keyboard::getId(), 'S'};
        inline const InputBinding KeyT{Keyboard::getId(), 'T'};
        inline const InputBinding KeyU{Keyboard::getId(), 'U'};
        inline const InputBinding KeyV{Keyboard::getId(), 'V'};
        inline const InputBinding KeyW{Keyboard::getId(), 'W'};
        inline const InputBinding KeyX{Keyboard::getId(), 'X'};
        inline const InputBinding KeyY{Keyboard::getId(), 'Y'};
        inline const InputBinding KeyZ{Keyboard::getId(), 'Z'};
 
        inline const InputBinding KeyNumPad0{Keyboard::getId(), VK_NUMPAD0};
        inline const InputBinding KeyNumPad1{Keyboard::getId(), VK_NUMPAD1};
        inline const InputBinding KeyNumPad2{Keyboard::getId(), VK_NUMPAD2};
        inline const InputBinding KeyNumPad3{Keyboard::getId(), VK_NUMPAD3};
        inline const InputBinding KeyNumPad4{Keyboard::getId(), VK_NUMPAD4};
        inline const InputBinding KeyNumPad5{Keyboard::getId(), VK_NUMPAD5};
        inline const InputBinding KeyNumPad6{Keyboard::getId(), VK_NUMPAD6};
        inline const InputBinding KeyNumPad7{Keyboard::getId(), VK_NUMPAD7};
        inline const InputBinding KeyNumPad8{Keyboard::getId(), VK_NUMPAD8};
        inline const InputBinding KeyNumPad9{Keyboard::getId(), VK_NUMPAD9};
 
        inline const InputBinding KeyMultiply{Keyboard::getId(), VK_MULTIPLY};
        inline const InputBinding KeyAdd{Keyboard::getId(), VK_ADD};
        inline const InputBinding KeySeparator{Keyboard::getId(), VK_SEPARATOR};
        inline const InputBinding KeySubtract{Keyboard::getId(), VK_SUBTRACT};
        inline const InputBinding KeyDecimal{Keyboard::getId(), VK_DECIMAL};
        inline const InputBinding KeyDivide{Keyboard::getId(), VK_DIVIDE};
 
        inline const InputBinding KeyLShift{Keyboard::getId(), VK_LSHIFT};
        inline const InputBinding KeyRShift{Keyboard::getId(), VK_RSHIFT};
        inline const InputBinding KeyLControl{Keyboard::getId(), VK_LCONTROL};
        inline const InputBinding KeyRControl{Keyboard::getId(), VK_RCONTROL};
        inline const InputBinding KeyLMenu{Keyboard::getId(), VK_LMENU};
        inline const InputBinding KeyRMenu{Keyboard::getId(), VK_RMENU};
        inline const InputBinding KeyLWin{Keyboard::getId(), VK_LWIN};
        inline const InputBinding KeyRWin{Keyboard::getId(), VK_RWIN};
 
        inline const InputBinding KeyApps{Keyboard::getId(), VK_APPS};
        inline const InputBinding KeySleep{Keyboard::getId(), VK_SLEEP};
        inline const InputBinding KeyScrollLock{Keyboard::getId(), VK_SCROLL};
        inline const InputBinding KeyPause{Keyboard::getId(), VK_PAUSE};
        inline const InputBinding KeyPrintScreen{Keyboard::getId(), VK_SNAPSHOT};
 
        inline const InputBinding KeyOem1{Keyboard::getId(), VK_OEM_1};
        inline const InputBinding KeyOemPlus{Keyboard::getId(), VK_OEM_PLUS};
        inline const InputBinding KeyOemComma{Keyboard::getId(), VK_OEM_COMMA};
        inline const InputBinding KeyOemMinus{Keyboard::getId(), VK_OEM_MINUS};
        inline const InputBinding KeyOemPeriod{Keyboard::getId(), VK_OEM_PERIOD};
        inline const InputBinding KeyOem2{Keyboard::getId(), VK_OEM_2};
        inline const InputBinding KeyOem3{Keyboard::getId(), VK_OEM_3};
        inline const InputBinding KeyOem4{Keyboard::getId(), VK_OEM_4};
        inline const InputBinding KeyOem5{Keyboard::getId(), VK_OEM_5};
        inline const InputBinding KeyOem6{Keyboard::getId(), VK_OEM_6};
        inline const InputBinding KeyOem7{Keyboard::getId(), VK_OEM_7};
        inline const InputBinding KeyOem8{Keyboard::getId(), VK_OEM_8};
        inline const InputBinding KeyOem102{Keyboard::getId(), VK_OEM_102};
 
        inline const InputBinding KeyNumLock{Keyboard::getId(), VK_NUMLOCK};
        inline const InputBinding KeyClear{Keyboard::getId(), VK_CLEAR};
 
        inline const InputBinding Key0{Keyboard::getId(), '0'};
        inline const InputBinding Key1{Keyboard::getId(), '1'};
        inline const InputBinding Key2{Keyboard::getId(), '2'};
        inline const InputBinding Key3{Keyboard::getId(), '3'};
        inline const InputBinding Key4{Keyboard::getId(), '4'};
        inline const InputBinding Key5{Keyboard::getId(), '5'};
        inline const InputBinding Key6{Keyboard::getId(), '6'};
        inline const InputBinding Key7{Keyboard::getId(), '7'};
        inline const InputBinding Key8{Keyboard::getId(), '8'};
        inline const InputBinding Key9{Keyboard::getId(), '9'};
 
        inline const InputBinding KeySpace{Keyboard::getId(), VK_SPACE};
        inline const InputBinding KeyEnter{Keyboard::getId(), VK_RETURN};
        inline const InputBinding KeyEscape{Keyboard::getId(), VK_ESCAPE};
        inline const InputBinding KeyTab{Keyboard::getId(), VK_TAB};
        inline const InputBinding KeyShift{Keyboard::getId(), VK_SHIFT};
        inline const InputBinding KeyControl{Keyboard::getId(), VK_CONTROL};
        inline const InputBinding KeyAlt{Keyboard::getId(), VK_MENU};
        inline const InputBinding KeyBackspace{Keyboard::getId(), VK_BACK};
        inline const InputBinding KeyCapsLock{Keyboard::getId(), VK_CAPITAL};
        inline const InputBinding KeyLeft{Keyboard::getId(), VK_LEFT};
        inline const InputBinding KeyRight{Keyboard::getId(), VK_RIGHT};
        inline const InputBinding KeyUp{Keyboard::getId(), VK_UP};
        inline const InputBinding KeyDown{Keyboard::getId(), VK_DOWN};
        inline const InputBinding KeyInsert{Keyboard::getId(), VK_INSERT};
        inline const InputBinding KeyDelete{Keyboard::getId(), VK_DELETE};
        inline const InputBinding KeyHome{Keyboard::getId(), VK_HOME};
        inline const InputBinding KeyEnd{Keyboard::getId(), VK_END};
        inline const InputBinding KeyPageUp{Keyboard::getId(), VK_PRIOR};
        inline const InputBinding KeyPageDown{Keyboard::getId(), VK_NEXT};
        inline const InputBinding KeyF1{Keyboard::getId(), VK_F1};
        inline const InputBinding KeyF2{Keyboard::getId(), VK_F2};
        inline const InputBinding KeyF3{Keyboard::getId(), VK_F3};
        inline const InputBinding KeyF4{Keyboard::getId(), VK_F4};
        inline const InputBinding KeyF5{Keyboard::getId(), VK_F5};
        inline const InputBinding KeyF6{Keyboard::getId(), VK_F6};
        inline const InputBinding KeyF7{Keyboard::getId(), VK_F7};
        inline const InputBinding KeyF8{Keyboard::getId(), VK_F8};
        inline const InputBinding KeyF9{Keyboard::getId(), VK_F9};
        inline const InputBinding KeyF10{Keyboard::getId(), VK_F10};
        inline const InputBinding KeyF11{Keyboard::getId(), VK_F11};
        inline const InputBinding KeyF12{Keyboard::getId(), VK_F12};
    } // bindings
 
} // namespace parteeengine::input