#include <iostream>

namespace goltsov
{
  struct point_t
  {
    double x, y;
  };
  struct rectangle_t
  {
    double width, height;
    point_t pos;
  };
  struct Shape
  {
    virtual double getArea() const = 0;
    virtual rectangle_t getFrameRect() const = 0;
    virtual void move(point_t) = 0;
    virtual void move(double, double) = 0;
    virtual void scale(double k) = 0;
    virtual ~Shape() = default;
  };
  struct Rectangle : Shape
  {
    Rectangle(double width, double height, point_t pos)
    {
      a.width = width;
      a.height = height;
      a.pos = pos;
    }
    Rectangle(const Rectangle& other)
    {
      a = other.a;
    }
    Rectangle(Rectangle&& other)
    {
      a = std::move(other.a);
    }
    Rectangle& operator=(const Rectangle& other)
    {
      if (this != &other)
      {
        a = other.a;
      }
      return * this;
    }
    Rectangle& operator=(Rectangle&& other)
    {
      if (this != &other)
      {
        a = std::move(other.a);
      }
      return * this;
    }
    ~Rectangle() = default;
    double getArea() const override;
    rectangle_t getFrameRect() const override;
    void move(point_t) override;
    void move(double, double) override;
    void scale(double k) override;
  private:
      rectangle_t a;
  };
}

int main()
{
  using namespace goltsov;
  Rectangle a = {1, 5, {2, 3}};
  Rectangle b = a;
  b.scale(2);
  std::cout << a.getArea() << ' ' << b.getArea() << '\n';
  return 0;
}

double goltsov::Rectangle::getArea() const
{
  return a.width * a.height;
}
goltsov::rectangle_t goltsov::Rectangle::getFrameRect() const
{
  return a;
}
void goltsov::Rectangle::move(goltsov::point_t newPos)
{
  a.pos = newPos;
}
void goltsov::Rectangle::move(double dx, double dy)
{
  a.pos.x += dx;
  a.pos.y += dy;
}
void goltsov::Rectangle::scale(double k)
{
  a.height *= k;
  a.width *= k;
}
