#include <iostream>
#include <cmath>

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
      if (width <= 0 || height <= 0)
      {
        throw std::logic_error("The width and height must be greater than zero.");
      }
      a.width = width;
      a.height = height;
      a.pos = pos;
    }
    Rectangle(const Rectangle& other):
      a(other.a)
    {}
    Rectangle(Rectangle&& other):
      a(std::move(other.a))
    {}
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
  struct Rubber : Shape
  {
    Rubber(double r1, point_t pos1, double r2, point_t pos2):
      r1(r1), r2(r2), pos1(pos1), pos2(pos2)
    {
      if (r1 <= 0 || r2 <= 0)
      {
        throw std::logic_error("The radii of the circles must be greater than 0");
      }
      if (pos1.x == pos2.x && pos1.y == pos2.y)
      {
        throw std::logic_error("The centers of the circles should not coincide");
      }
      if ((sqrt((pos1.x - pos2.x) * (pos1.x - pos2.x) + (pos1.y - pos2.y) * (pos1.y - pos2.y)) + r2) > r1)
      {
        throw std::logic_error("The smaller circle should lie completely inside the larger one");
      }
    }
    Rubber(Rubber& other):
      r1(other.r1), r2(other.r2), pos1(other.pos1), pos2(other.pos2)
    {}
    Rubber(Rubber&& other):
      r1(std::move(other.r1)), r2(std::move(other.r2)), pos1(std::move(other.pos1)), pos2(std::move(other.pos2))
    {}
    Rubber& operator=(Rubber& other)
    {
      if (this != &other)
      {
        r1 = other.r1;
        r2 = other.r2;
        pos1 = other.pos1;
        pos2 = other.pos2;
      }
      return * this;
    }
    Rubber& operator=(Rubber&& other)
    {
      if (this != &other)
      {
        r1 = other.r1;
        r2 = other.r2;
        pos1 = other.pos1;
        pos2 = other.pos2;
      }
      return * this;
    }
    ~Rubber() override = default;
    double getArea() const override;
    rectangle_t getFrameRect() const override;
    void move(point_t) override;
    void move(double, double) override;
    void scale(double k) override;
  private:
    double r1, r2;
    point_t pos1, pos2;
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

double goltsov::Rubber::getArea() const
{
  return M_PI * (r1 * r1 - r2 * r2);
}
goltsov::rectangle_t goltsov::Rubber::getFrameRect() const
{
  return {r1 * 2, r1 * 2, pos1};
}
void goltsov::Rubber::move(goltsov::point_t newPos)
{
  double dx = pos1.x - pos2.x;
  double dy = pos1.y - pos2.y;
  pos2 = newPos;
  pos1.x = pos2.x + dx;
  pos1.y = pos2.y + dy;
}
void goltsov::Rubber::move(double dx, double dy)
{
  pos1.x += dx;
  pos2.x += dx;
  pos1.y += dy;
  pos2.y += dy;
}
void goltsov::Rubber::scale(double k)
{
  r1 *= k;
  r2 *= k;
  double dx = pos1.x - pos2.x;
  double dy = pos1.y - pos2.y;
  pos1.x = pos2.x + k * dx;
  pos1.y = pos2.y + k * dy;
}
