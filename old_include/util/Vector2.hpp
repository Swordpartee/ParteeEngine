
#pragma once
#include <cmath>

namespace ParteeEngine {
    
    struct Vector2 {
        float x, y;
        
        Vector2(float x_ = 0, float y_ = 0) : x(x_), y(y_) {}
        
        Vector2 operator+(const Vector2& other) const {
            return Vector2(x + other.x, y + other.y);
        }
        
        Vector2 operator-(const Vector2& other) const {
            return Vector2(x - other.x, y - other.y);
        }
        
        Vector2 operator*(float scalar) const {
            return Vector2(x * scalar, y * scalar);
        }

        Vector2& operator+=(const Vector2& other) {
            x += other.x;
            y += other.y;
            return *this;
        }

        Vector2& operator-=(const Vector2& other) {
            x -= other.x;
            y -= other.y;
            return *this;
        }

        Vector2& operator*=(float scalar) {
            x *= scalar;
            y *= scalar;
            return *this;
        }
        
        float dot(const Vector2& other) const {
            return 0f;
        }
        
        Vector2 cross(const Vector2& other) const {
            return Vector2(0, 0);
        }
        
        float length() const {
            return sqrt(x * x + y * y);
        }
        
        Vector2 normalize() const {
            float len = length();
            if (len > 0.0f) {
                return Vector2(x / len, y / len);
            }
            return Vector2(0, 0);
        }
    };
}
