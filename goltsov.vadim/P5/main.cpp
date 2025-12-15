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
    Rubber(const double r10, const point_t pos10, const double r20, const point_t pos20):
      r1(r10), r2(r20), pos1(pos10), pos2(pos20)
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
    Polygon(const point_t* mtx0, const size_t n0):
      n(n0), pos(polygonCentroid(mtx0, n))
    {
      if (n < 3)
      {
        throw std::logic_error("The polygon must have at least 3 vertices");
      }
      this->mtx = new point_t[n];
      for (size_t i = 0; i < n; ++i)
      {
        this->mtx[i] = mtx0[i];
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
  void scaleRelativePoint(goltsov::Shape* a, goltsov::point_t p, double k);
  void totalPrint(size_t count, Shape** mas2, std::ostream & outp);
}

void goltsov::scaleRelativePoint(goltsov::Shape* a, goltsov::point_t p, double k)
{
  point_t p1 = a->getFrameRect().pos;
  a->move(p);
  a->scale(k);
  point_t p2 = a->getFrameRect().pos;
  double dx = p1.x - p2.x;
  double dy = p1.y - p2.y;
  dx *= k;
  dy *= k;
  a->move(dx, dy);
}

void goltsov::totalPrint(size_t count, Shape** mas2, std::ostream & outp)
{
  double total_area = 0;
  outp << "Areas:\n";
  for (size_t i = 0; i < count; ++i)
  {
    outp << mas2[i]->getArea() << '\n';
    total_area += mas2[i]->getArea();
  }
  outp << "Total area:\n";
  outp << total_area << '\n' << '\n';

  rectangle_t total_rec = mas2[0]->getFrameRect();
  double left_x = total_rec.pos.x - total_rec.width/2;
  double right_x = total_rec.pos.x + total_rec.width/2;
  double down_y = total_rec.pos.y - total_rec.height/2;
  double up_y = total_rec.pos.y + total_rec.height/2;
  outp << "Frame rectangles:\n";
  for (size_t i = 0; i < count; ++i)
  {
    rectangle_t a = mas2[i]->getFrameRect();
    outp << a.width << ' ' << a.height << ' ' << a.pos.x << " - " << a.pos.y << '\n';

    double left_xi = a.pos.x - a.width/2;
    double right_xi = a.pos.x + a.width/2;
    double down_yi = a.pos.y - a.height/2;
    double up_yi = a.pos.y + a.height/2;
    left_x = left_xi < left_x ? left_xi : left_x;
    right_x = right_xi > right_x ? right_xi : right_x;
    down_y = down_yi < down_y ? down_yi : down_y;
    up_y = up_yi > up_y ? up_yi : up_y;

    total_rec = {right_x - left_x, up_y - down_y, {(left_x + right_x)/2, (up_y + down_y)/2}};
  }
  outp << "Total frame rectangle:\n";
  outp << total_rec.width << ' ' << total_rec.height << ' ' << total_rec.pos.x << " - " << total_rec.pos.y << '\n';
}

int main()
{
  using namespace goltsov;
  Rectangle a = {1, 5, {2, 3}};

  Rubber b = {4.4, {1, 1}, 1.1, {1.1, 1.1}};

  point_t mas[5] = {{0, 0}, {1, 0}, {2, 2}, {2, 3}, {1, 4}};
  Polygon c = {mas, 5};

  size_t count = 3;
  Shape* mas2[count] = {&a, &b, &c};
  point_t point_a;
  double k;

  std::cout << "Before changes:\n";
  totalPrint(count, mas2, std::cout);

  std::cout << "\n\n";

  bool f = 0;

  std::cout << "Enter x, y, k:\n";
  while(std::cin >> point_a.x >> point_a.y >> k)
  {
    if (std::cin.fail() || std::cin.bad())
    {
      std::cerr << "Bad input\n";
      return 1;
    }
    if (k <= 0)
    {
      std::cerr << "The zoom level must be greater than 0\n";
      return 1;
    }

    f = 1;

    std::cout << "For x = " << point_a.x << "; y = " << point_a.y << "; k = " << k << '\n';

    for (size_t i = 0; i < count; ++i)
    {
      scaleRelativePoint(mas2[i], point_a, k);
    }

    totalPrint(count, mas2, std::cout);
    std::cout << "\n\n";
    std::cout << "Enter x, y, k:\n";
  }
  if ((std::cin.fail() || std::cin.bad()) && !std::cin.eof())
  {
    std::cerr << "Bad input\n";
    return 1;
  }
  if (std::cin.eof() && !f)
  {
    std::cerr << "No input\n";
    return 1;
  }
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
  if (k <= 0)
  {
    throw std::logic_error("The zoom level must be greater than 0");
  }
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
  if (k <= 0)
  {
    throw std::logic_error("The zoom level must be greater than 0");
  }
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
  if (k <= 0)
  {
    throw std::logic_error("The zoom level must be greater than 0");
  }
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
