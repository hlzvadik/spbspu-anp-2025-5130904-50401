#include <iostream>
#include <cmath>

const double PI = std::acos(-1.0);

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
    virtual void move(const point_t) = 0;
    virtual void move(const double, const double) = 0;
    virtual void scale(const double k) = 0;
    virtual ~Shape() = default;
  };
  struct Rectangle : Shape
  {
    Rectangle(const double width, const double height, const point_t pos)
    {
      if (width <= 0 || height <= 0)
      {
        throw std::logic_error("The width and height must be greater than zero");
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
    void move(const point_t) override;
    void move(const double, const double) override;
    void scale(const double k) override;
  private:
      rectangle_t a;
  };
  struct Rubber : Shape
  {
    Rubber(const double r1, const point_t pos1, const double r2, const point_t pos2):
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
    Rubber(const Rubber& other):
      r1(other.r1), r2(other.r2), pos1(other.pos1), pos2(other.pos2)
    {}
    Rubber(Rubber&& other):
      r1(std::move(other.r1)), r2(std::move(other.r2)), pos1(std::move(other.pos1)), pos2(std::move(other.pos2))
    {}
    Rubber& operator=(const Rubber& other)
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
    ~Rubber() = default;
    double getArea() const override;
    rectangle_t getFrameRect() const override;
    void move(const point_t) override;
    void move(const double, const double) override;
    void scale(const double k) override;
  private:
    double r1, r2;
    point_t pos1, pos2;
  };
  point_t polygonCentroid(const point_t* mtx, const size_t n)
  {
    if (n < 3)
    {
        point_t center = {0, 0};
        if (n > 0) {
            for (size_t i = 0; i < n; i++)
            {
                center.x += mtx[i].x;
                center.y += mtx[i].y;
            }
            center.x /= n;
            center.y /= n;
        }
        return center;
    }
    double area = 0.0;
    double centroid_x = 0.0;
    double centroid_y = 0.0;
    for (size_t i = 0; i < n; i++)
    {
        size_t j = (i + 1) % n;
        double cross = (mtx[i].x * mtx[j].y - mtx[j].x * mtx[i].y);
        area += cross;
        centroid_x += (mtx[i].x + mtx[j].x) * cross;
        centroid_y += (mtx[i].y + mtx[j].y) * cross;
    }
    area *= 0.5;
    double factor = 1.0 / (6.0 * area);
    point_t centroid;
    centroid.x = centroid_x * factor;
    centroid.y = centroid_y * factor;
    return centroid;
  }
  struct Polygon : Shape
  {
    Polygon(const point_t* mtx, const size_t n):
      n(n), pos(polygonCentroid(mtx, n))
    {
      if (n < 3)
      {
        throw std::logic_error("The polygon must have at least 3 vertices");
      }
      this->mtx = new point_t[n];
      for (size_t i = 0; i < n; ++i)
      {
        this->mtx[i] = mtx[i];
      }
    }
    Polygon(const Polygon& other):
      n(other.n), pos(other.pos)
    {
      mtx = new point_t[other.n];
      for (size_t i = 0; i < n; ++i)
      {
        mtx[i] = other.mtx[i];
      }
    }
    Polygon(Polygon&& other):
      n(std::move(other.n)), pos(other.pos)
    {
      mtx = other.mtx;
      other.mtx = nullptr;
    }
    Polygon& operator=(const Polygon& other)
    {
      if (this != &other)
      {
        delete[] mtx;
        n = other.n;
        pos = other.pos;
        mtx = new point_t[n];
        for (size_t i = 0; i < n; ++i)
        {
          mtx[i] = other.mtx[i];
        }
      }
      return * this;
    }
    Polygon& operator=(Polygon&& other)
    {
      if (this != &other)
      {
        delete[] mtx;
        mtx = other.mtx;
        other.mtx = nullptr;
        n = other.n;
        pos = other.pos;
      }
      return * this;
    }
    ~Polygon()
    {
      delete[] mtx;
    }
    double getArea() const override;
    rectangle_t getFrameRect() const override;
    void move(const point_t) override;
    void move(const double, const double) override;
    void scale(const double k) override;
  private:
    point_t* mtx = nullptr;
    size_t n;
    point_t pos;
  };
}

int main()
{
  using namespace goltsov;
  Rectangle a = {1, 5, {2, 3}};
  double a1 = a.getArea();
  rectangle_t a2 = a.getFrameRect();
  a.move({1,1});
  a.move(2,2);
  a.scale(2.0);
  std::cout << a1 << ' ' << a2.width << '-' << a2.height << '-' << a2.pos.x << '-' << a2.pos.y << '\n';

  Rubber b = {4.4, {1, 1}, 1.1, {1.1, 1.1}};
  double b1 = b.getArea();
  rectangle_t b2 = b.getFrameRect();
  b.move({3,3});
  b.move(3,3);
  b.scale(2.5);
  std::cout << b1 << ' ' << b2.width << '-' << b2.height << '-' << b2.pos.x << '-' << b2.pos.y << '\n';

  point_t mas[5] = {{0, 0}, {1, 0}, {2, 2}, {2, 3}, {1, 4}};
  Polygon c = {mas, 5};
  double c1 = c.getArea();
  rectangle_t c2 = c.getFrameRect();
  c.move({1, 1});
  c.move(2,2);
  c.scale(1.1);
  std::cout << c1 << ' ' << c2.width << '-' << c2.height << '-' << c2.pos.x << '-' << c2.pos.y << '\n';
}

double goltsov::Rectangle::getArea() const
{
  return a.width * a.height;
}
goltsov::rectangle_t goltsov::Rectangle::getFrameRect() const
{
  return a;
}
void goltsov::Rectangle::move(const goltsov::point_t newPos)
{
  a.pos = newPos;
}
void goltsov::Rectangle::move(const double dx, const double dy)
{
  a.pos.x += dx;
  a.pos.y += dy;
}
void goltsov::Rectangle::scale(const double k)
{
  a.height *= k;
  a.width *= k;
}

double goltsov::Rubber::getArea() const
{
  return PI * (r1 * r1 - r2 * r2);
}
goltsov::rectangle_t goltsov::Rubber::getFrameRect() const
{
  return {r1 * 2, r1 * 2, pos1};
}
void goltsov::Rubber::move(const goltsov::point_t newPos)
{
  double dx = pos1.x - pos2.x;
  double dy = pos1.y - pos2.y;
  pos2 = newPos;
  pos1.x = pos2.x + dx;
  pos1.y = pos2.y + dy;
}
void goltsov::Rubber::move(const double dx, const double dy)
{
  pos1.x += dx;
  pos2.x += dx;
  pos1.y += dy;
  pos2.y += dy;
}
void goltsov::Rubber::scale(const double k)
{
  r1 *= k;
  r2 *= k;
  double dx = pos1.x - pos2.x;
  double dy = pos1.y - pos2.y;
  pos1.x = pos2.x + k * dx;
  pos1.y = pos2.y + k * dy;
}

double goltsov::Polygon::getArea() const
{
    if (n < 3) return 0.0;
    double area = 0.0;
    for (size_t i = 0; i < n; i++)
    {
        size_t j = (i + 1) % n;
        area += mtx[i].x * mtx[j].y - mtx[j].x * mtx[i].y;
    }
    return fabs(area) * 0.5;
}
goltsov::rectangle_t goltsov::Polygon::getFrameRect() const
{
  double max_x = mtx[0].x;
  double min_x = mtx[0].x;
  double max_y = mtx[0].y;
  double min_y = mtx[0].y;
  for (size_t i = 0; i < n; ++i)
  {
    max_x = (max_x < mtx[i].x ? mtx[i].x : max_x);
    min_x = (min_x > mtx[i].x ? mtx[i].x : min_x);
    max_y = (max_y < mtx[i].y ? mtx[i].y : max_y);
    min_y = (min_y > mtx[i].y ? mtx[i].y : min_y);
  }
  return {max_x - min_x, max_y - min_y, {(max_x + min_x)/2, (max_y + min_y)/2}};
}
void goltsov::Polygon::move(const goltsov::point_t newPos)
{
  double dx = newPos.x - pos.x;
  double dy = newPos.y - pos.y;
  for (size_t i = 0; i < n; ++i)
  {
    mtx[i].x += dx;
    mtx[i].y += dy;
  }
  pos = newPos;
}
void goltsov::Polygon::move(const double dx, const double dy)
{
  for (size_t i = 0; i < n; ++i)
  {
    mtx[i].x += dx;
    mtx[i].y += dy;
  }
  pos.x += dx;
  pos.y += dy;
}
void goltsov::Polygon::scale(const double k)
{
  for (size_t i = 0; i < n; ++i)
  {
    double dx = mtx[i].x - pos.x;
    double dy = mtx[i].y - pos.y;
    dx *= k;
    dy *= k;
    mtx[i].x = pos.x + dx;
    mtx[i].y = pos.y + dy;
  }
}
