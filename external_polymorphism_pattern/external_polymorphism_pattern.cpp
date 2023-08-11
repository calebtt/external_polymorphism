// external_polymorphism_pattern.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <string>
#include <memory>
#include <functional>


// Our polymorphic concept type, provides a polymorphic interface.
// It will be possible to define the typical inheritance-based interface here and pass instantiations
// of the ShapeModel serving different "shape" types (data packs) and use them polymorphically through this.
class ShapeConcept
{
public:
    virtual ~ShapeConcept() = default;
    virtual void doAction() const = 0;
};

// Note the draw strategy/strategies can also be a template param instead of a function pointer wrapper.
template<typename ShapeT>
class ShapeModel : public ShapeConcept
{
public:
    using DrawStrategy = std::function<void(const ShapeT&)>;
    DrawStrategy Strat;
    ShapeT Shape;
public:
    explicit ShapeModel(ShapeT shape, DrawStrategy drawer) : Strat(std::move(drawer)), Shape(shape) { }

    void doAction() const override
    {
        if (Strat)
        {
            Strat(Shape);
        }
    }
};

// nice neat non-polymorphic classes (data packs typically)
class Circle
{
private:
    double m_radius{};
public:
    explicit Circle(double rad) : m_radius(rad) { }
    double Radius() const { return m_radius; }
};

class Square
{
    double m_side{};
public:
    explicit Square(double side) : m_side(side) { }
    double Side() const { return m_side; }
};

// A couple draw implementations
// i.e., implementations that can be used with the polymorphic interface type + template-arg chosen data pack
// via the "model" type.
inline
void draw_circle(const Circle& c)
{
    std::cout << "Rad: " << c.Radius() << '\n';
}
inline
void draw_square(const Square& sq)
{
    std::cout << "Side: " << sq.Side() << '\n';
}
// And the polymorphic drawer
inline
auto draw_item(ShapeConcept& sc)
{
    // forwards this call to the actual chosen implementation function, one of the two above.
    sc.doAction();
}
inline
void do_ext_poly_test()
{
    // The external polymorphism design pattern entails
    // making a so-called "model" class template type that combines (composition, not inheritance)
    // 1. The non-polymorphic type
    // 2. The polymorphic interface
    // 3. a function pointer to the implementation
    // Usage is to then pass the "concept" type, which is the base class for the "model" type for polymorphic calling.

    Circle c(2.0);
    Square sq(3.0);
    ShapeModel<Circle> smc(c, draw_circle);
    ShapeModel<Square> smsq(sq, draw_square);
    draw_item(smc);
    draw_item(smsq);

    // Extension entails creating your new shape type and a "draw" implementation (free function preferred),
    // Then you will instantiate a ShapeModel template with the extended type and behavior that can be given to a polymorphic "draw" fn
    // Oval o(4.0);
    // ShapeModel<Oval> smo(o, draw_oval);
    // draw_item(smo);

    // From this we can see that the pattern's purpose is to free the implementation of the "draw" function from each added type, now they can be directly
    // re-used where possible.
}

int main()
{
    do_ext_poly_test();
}
