#pragma once

#include <Windows.h>
#include <stdexcept>

#include "engine/input/InputDevice.hpp"
#include "engine/input/InputBinding.hpp"

namespace ParteeEngine {


    class Keyboard : public InputDeviceCRTP<Keyboard> {
    public:
        Keyboard() = default;
        ~Keyboard() override = default;

        bool isActive(InputBinding binding) const override;

        float getAnalog(InputBinding binding) const override;
        
    };
    
    namespace Bindings {
        inline constexpr InputBinding KeyA{Keyboard::typeID, 'A'};
        inline constexpr InputBinding KeyD{Keyboard::typeID, 'B'};
        inline constexpr InputBinding KeyC{Keyboard::typeID, 'C'};
        inline constexpr InputBinding KeyB{Keyboard::typeID, 'D'};
        inline constexpr InputBinding KeyE{Keyboard::typeID, 'E'};
        inline constexpr InputBinding KeyF{Keyboard::typeID, 'F'};
        inline constexpr InputBinding KeyG{Keyboard::typeID, 'G'};
        inline constexpr InputBinding KeyH{Keyboard::typeID, 'H'};
        inline constexpr InputBinding KeyI{Keyboard::typeID, 'I'};
        inline constexpr InputBinding KeyJ{Keyboard::typeID, 'J'};
        inline constexpr InputBinding KeyK{Keyboard::typeID, 'K'};
        inline constexpr InputBinding KeyL{Keyboard::typeID, 'L'};
        inline constexpr InputBinding KeyM{Keyboard::typeID, 'M'};
        inline constexpr InputBinding KeyN{Keyboard::typeID, 'N'};
        inline constexpr InputBinding KeyO{Keyboard::typeID, 'O'};
        inline constexpr InputBinding KeyP{Keyboard::typeID, 'P'};
        inline constexpr InputBinding KeyQ{Keyboard::typeID, 'Q'};
        inline constexpr InputBinding KeyR{Keyboard::typeID, 'R'};
        inline constexpr InputBinding KeyS{Keyboard::typeID, 'S'};
        inline constexpr InputBinding KeyT{Keyboard::typeID, 'T'};
        inline constexpr InputBinding KeyU{Keyboard::typeID, 'U'};
        inline constexpr InputBinding KeyV{Keyboard::typeID, 'V'};
        inline constexpr InputBinding KeyW{Keyboard::typeID, 'W'};
        inline constexpr InputBinding KeyX{Keyboard::typeID, 'X'};
        inline constexpr InputBinding KeyY{Keyboard::typeID, 'Y'};
        inline constexpr InputBinding KeyZ{Keyboard::typeID, 'Z'};
 
        inline constexpr InputBinding KeyNumPad0{Keyboard::typeID, VK_NUMPAD0};
        inline constexpr InputBinding KeyNumPad1{Keyboard::typeID, VK_NUMPAD1};
        inline constexpr InputBinding KeyNumPad2{Keyboard::typeID, VK_NUMPAD2};
        inline constexpr InputBinding KeyNumPad3{Keyboard::typeID, VK_NUMPAD3};
        inline constexpr InputBinding KeyNumPad4{Keyboard::typeID, VK_NUMPAD4};
        inline constexpr InputBinding KeyNumPad5{Keyboard::typeID, VK_NUMPAD5};
        inline constexpr InputBinding KeyNumPad6{Keyboard::typeID, VK_NUMPAD6};
        inline constexpr InputBinding KeyNumPad7{Keyboard::typeID, VK_NUMPAD7};
        inline constexpr InputBinding KeyNumPad8{Keyboard::typeID, VK_NUMPAD8};
        inline constexpr InputBinding KeyNumPad9{Keyboard::typeID, VK_NUMPAD9};
 
        inline constexpr InputBinding KeyMultiply{Keyboard::typeID, VK_MULTIPLY};
        inline constexpr InputBinding KeyAdd{Keyboard::typeID, VK_ADD};
        inline constexpr InputBinding KeySeparator{Keyboard::typeID, VK_SEPARATOR};
        inline constexpr InputBinding KeySubtract{Keyboard::typeID, VK_SUBTRACT};
        inline constexpr InputBinding KeyDecimal{Keyboard::typeID, VK_DECIMAL};
        inline constexpr InputBinding KeyDivide{Keyboard::typeID, VK_DIVIDE};
 
        inline constexpr InputBinding KeyLShift{Keyboard::typeID, VK_LSHIFT};
        inline constexpr InputBinding KeyRShift{Keyboard::typeID, VK_RSHIFT};
        inline constexpr InputBinding KeyLControl{Keyboard::typeID, VK_LCONTROL};
        inline constexpr InputBinding KeyRControl{Keyboard::typeID, VK_RCONTROL};
        inline constexpr InputBinding KeyLMenu{Keyboard::typeID, VK_LMENU};
        inline constexpr InputBinding KeyRMenu{Keyboard::typeID, VK_RMENU};
        inline constexpr InputBinding KeyLWin{Keyboard::typeID, VK_LWIN};
        inline constexpr InputBinding KeyRWin{Keyboard::typeID, VK_RWIN};
 
        inline constexpr InputBinding KeyApps{Keyboard::typeID, VK_APPS};
        inline constexpr InputBinding KeySleep{Keyboard::typeID, VK_SLEEP};
        inline constexpr InputBinding KeyScrollLock{Keyboard::typeID, VK_SCROLL};
        inline constexpr InputBinding KeyPause{Keyboard::typeID, VK_PAUSE};
        inline constexpr InputBinding KeyPrintScreen{Keyboard::typeID, VK_SNAPSHOT};
 
        inline constexpr InputBinding KeyOem1{Keyboard::typeID, VK_OEM_1};
        inline constexpr InputBinding KeyOemPlus{Keyboard::typeID, VK_OEM_PLUS};
        inline constexpr InputBinding KeyOemComma{Keyboard::typeID, VK_OEM_COMMA};
        inline constexpr InputBinding KeyOemMinus{Keyboard::typeID, VK_OEM_MINUS};
        inline constexpr InputBinding KeyOemPeriod{Keyboard::typeID, VK_OEM_PERIOD};
        inline constexpr InputBinding KeyOem2{Keyboard::typeID, VK_OEM_2};
        inline constexpr InputBinding KeyOem3{Keyboard::typeID, VK_OEM_3};
        inline constexpr InputBinding KeyOem4{Keyboard::typeID, VK_OEM_4};
        inline constexpr InputBinding KeyOem5{Keyboard::typeID, VK_OEM_5};
        inline constexpr InputBinding KeyOem6{Keyboard::typeID, VK_OEM_6};
        inline constexpr InputBinding KeyOem7{Keyboard::typeID, VK_OEM_7};
        inline constexpr InputBinding KeyOem8{Keyboard::typeID, VK_OEM_8};
        inline constexpr InputBinding KeyOem102{Keyboard::typeID, VK_OEM_102};
 
        inline constexpr InputBinding KeyNumLock{Keyboard::typeID, VK_NUMLOCK};
        inline constexpr InputBinding KeyClear{Keyboard::typeID, VK_CLEAR};
 
        inline constexpr InputBinding Key0{Keyboard::typeID, '0'};
        inline constexpr InputBinding Key1{Keyboard::typeID, '1'};
        inline constexpr InputBinding Key2{Keyboard::typeID, '2'};
        inline constexpr InputBinding Key3{Keyboard::typeID, '3'};
        inline constexpr InputBinding Key4{Keyboard::typeID, '4'};
        inline constexpr InputBinding Key5{Keyboard::typeID, '5'};
        inline constexpr InputBinding Key6{Keyboard::typeID, '6'};
        inline constexpr InputBinding Key7{Keyboard::typeID, '7'};
        inline constexpr InputBinding Key8{Keyboard::typeID, '8'};
        inline constexpr InputBinding Key9{Keyboard::typeID, '9'};
 
        inline constexpr InputBinding KeySpace{Keyboard::typeID, VK_SPACE};
        inline constexpr InputBinding KeyEnter{Keyboard::typeID, VK_RETURN};
        inline constexpr InputBinding KeyEscape{Keyboard::typeID, VK_ESCAPE};
        inline constexpr InputBinding KeyTab{Keyboard::typeID, VK_TAB};
        inline constexpr InputBinding KeyShift{Keyboard::typeID, VK_SHIFT};
        inline constexpr InputBinding KeyControl{Keyboard::typeID, VK_CONTROL};
        inline constexpr InputBinding KeyAlt{Keyboard::typeID, VK_MENU};
        inline constexpr InputBinding KeyBackspace{Keyboard::typeID, VK_BACK};
        inline constexpr InputBinding KeyCapsLock{Keyboard::typeID, VK_CAPITAL};
        inline constexpr InputBinding KeyLeft{Keyboard::typeID, VK_LEFT};
        inline constexpr InputBinding KeyRight{Keyboard::typeID, VK_RIGHT};
        inline constexpr InputBinding KeyUp{Keyboard::typeID, VK_UP};
        inline constexpr InputBinding KeyDown{Keyboard::typeID, VK_DOWN};
        inline constexpr InputBinding KeyInsert{Keyboard::typeID, VK_INSERT};
        inline constexpr InputBinding KeyDelete{Keyboard::typeID, VK_DELETE};
        inline constexpr InputBinding KeyHome{Keyboard::typeID, VK_HOME};
        inline constexpr InputBinding KeyEnd{Keyboard::typeID, VK_END};
        inline constexpr InputBinding KeyPageUp{Keyboard::typeID, VK_PRIOR};
        inline constexpr InputBinding KeyPageDown{Keyboard::typeID, VK_NEXT};
        inline constexpr InputBinding KeyF1{Keyboard::typeID, VK_F1};
        inline constexpr InputBinding KeyF2{Keyboard::typeID, VK_F2};
        inline constexpr InputBinding KeyF3{Keyboard::typeID, VK_F3};
        inline constexpr InputBinding KeyF4{Keyboard::typeID, VK_F4};
        inline constexpr InputBinding KeyF5{Keyboard::typeID, VK_F5};
        inline constexpr InputBinding KeyF6{Keyboard::typeID, VK_F6};
        inline constexpr InputBinding KeyF7{Keyboard::typeID, VK_F7};
        inline constexpr InputBinding KeyF8{Keyboard::typeID, VK_F8};
        inline constexpr InputBinding KeyF9{Keyboard::typeID, VK_F9};
        inline constexpr InputBinding KeyF10{Keyboard::typeID, VK_F10};
        inline constexpr InputBinding KeyF11{Keyboard::typeID, VK_F11};
        inline constexpr InputBinding KeyF12{Keyboard::typeID, VK_F12};
    } 
 
} // namespace ParteeEngine