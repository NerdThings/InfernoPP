//
// Created by Reece Mackie on 29/11/18.
//

#include <limits>
#include "Inferno/Rectangle.h"

namespace Inferno {
    //Constructors
    
    Rectangle::Rectangle(float x, float y, float width, float height, float rotation,
                         Vector2 rotation_origin) : x(x), y(y), width(width), height(height), rotation(rotation), rotation_origin(rotation_origin) {
        
    }
    
    Rectangle::Rectangle(Vector2 position, Vector2 dimensions, float rotation,
                         Vector2 rotation_origin) : x(position.x), y(position.y), width(dimensions.x), height(dimensions.y), rotation(rotation), rotation_origin(rotation_origin)  {
        
    }
    
    //Methods
    
    bool Rectangle::axis_aligned() {
        return rotation == 0;
    }
    
    Line Rectangle::bottom() {
        return {bottom_right(), bottom_left()};
    }
    
    Vector2 Rectangle::bottom_left() {
        return Vector2::transform({x, y + height}, get_matrix());
    }
    
    Vector2 Rectangle::bottom_right() {
        return Vector2::transform({x + width, y + height}, get_matrix());
    }
    
    Vector2 Rectangle::center() {
        return Vector2::transform({x + width * 0.5f, y + height * 0.5f}, get_matrix());
    }
    
    bool Rectangle::contains(Vector2 point) {
        Vector2 translated_point = Vector2::transform(point, get_matrix(true));
        return this->x <= translated_point.x && translated_point.x < this->x + this->width && this->y <= translated_point.y && translated_point.y < this->y + this->height;
    }
    
    Matrix Rectangle::get_matrix(bool invert) {
        float r = rotation;
        if (invert)
            r = -r;
        return Matrix::create_translation(Vector3(-(rotation_origin + Vector2(x, y)), 0))
               * Matrix::create_rotation_z(r)
               * Matrix::create_translation(Vector3(rotation_origin + Vector2(x, y), 0));
    }
    
    //Thanks to https://manski.net/2011/05/rectangle-intersection-test-with-csharp/
    bool axis_separation(Vector2 x1, Vector2 x2, Vector2 x3, std::vector<Vector2> other_pts) {
        Vector2 vec = x2 - x1;
        Vector2 rotated = Vector2(-vec.y, vec.x);
        
        bool ref_size = (rotated.x * (x3.x - x1.x)
                         + rotated.y * (x3.y - x1.y)) >= 0;
        
        for (Vector2 v : other_pts) {
            bool side = (rotated.x * (v.x - x1.x)
                         + rotated.y * (v.y - x1.y)) >= 0;
            if (side == ref_size)
                return false;
        }
        return true;
    }
    
    bool Rectangle::intersects(Rectangle b) {
        std::vector<Vector2> a_pts;
        a_pts.emplace_back(top_left());
        a_pts.emplace_back(top_right());
        a_pts.emplace_back(bottom_right());
        a_pts.emplace_back(bottom_left());
        
        std::vector<Vector2> b_pts;
        b_pts.emplace_back(b.top_left());
        b_pts.emplace_back(b.top_right());
        b_pts.emplace_back(b.bottom_right());
        b_pts.emplace_back(b.bottom_left());
        
        if (axis_separation(top_left(), top_right(), bottom_right(), b_pts))
            return false;
        if (axis_separation(top_left(), bottom_left(), bottom_right(), b_pts))
            return false;
        if (axis_separation(bottom_left(), bottom_right(), top_left(), b_pts))
            return false;
        if (axis_separation(bottom_right(), top_right(), top_left(), b_pts))
            return false;
    
        if (axis_separation(b.top_left(), b.top_right(), b.bottom_right(), a_pts))
            return false;
        if (axis_separation(b.top_left(), b.bottom_left(), b.bottom_right(), a_pts))
            return false;
        if (axis_separation(b.bottom_left(), b.bottom_right(), b.top_left(), a_pts))
            return false;
        if (axis_separation(b.bottom_right(), b.top_right(), b.top_left(), a_pts))
            return false;
        
        return true;
    }
    
    bool Rectangle::intersects(Line b) {
        //Check for line intersection or the ends inside the rectangle
        
        for (Line line : lines()) {
            if (line.intersects(b))
                return true;
        }
        
        //return contains(b.p1, b.p2);
        return false;
    }
    
    Line Rectangle::left() {
        return {bottom_left(), top_left()};
    }
    
    std::vector<Line> Rectangle::lines() {
        return {top(), right(), bottom(), left()};
    }
    
    Line Rectangle::right() {
        return {top_right(), bottom_right()};
    }
    
    Line Rectangle::top() {
        return {top_left(), top_right()};
    }
    
    Vector2 Rectangle::top_left() {
        return Vector2::transform({x, y}, get_matrix());
    }
    
    Vector2 Rectangle::top_right() {
        return Vector2::transform({x + width, y}, get_matrix());
    }
    
    //Operators
    
    bool Rectangle::operator==(Rectangle b) {
        return (x == b.x &&
                y == b.y &&
                width == b.width &&
                height == b.height &&
                rotation == b.rotation &&
                rotation_origin == b.rotation_origin);
    }
    
    bool Rectangle::operator!=(Rectangle b) {
        return !(*this == b);
    }
}