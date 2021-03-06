#pragma once

#include "geometry/point2d.hpp"
#include "geometry/rect2d.hpp"
#include "geometry/any_rect2d.hpp"

#include "base/matrix.hpp"


class ScreenBase
{
public:
  typedef math::Matrix<double, 3, 3> MatrixT;

private:
  m2::RectD m_PixelRect;

  double m_Scale;
  ang::AngleD m_Angle;
  m2::PointD m_Org;

protected:
  /// @group Dependent parameters
  /// Global to Pixel conversion matrix.
  ///             |a11 a12 0|
  /// |x, y, 1| * |a21 a22 0| = |x', y', 1|
  ///             |a31 a32 1|
  /// @{
  MatrixT m_GtoP;

  /// Pixel to Global conversion matrix. Inverted GtoP matrix.
  MatrixT m_PtoG;

  /// Global Rect
  m2::AnyRectD m_GlobalRect;

  /// X-axis aligned global rect used for clipping
  m2::RectD m_ClipRect;

  /// @}

  // Update dependent parameters from base parameters.
  // Must be called when base parameters changed.
  void UpdateDependentParameters();

public:
  ScreenBase();
  ScreenBase(m2::RectI const & pxRect, m2::AnyRectD const & glbRect);
  ScreenBase(ScreenBase const & s,
             m2::PointD const & org, double scale, double angle);

  void SetFromRect(m2::AnyRectD const & rect);
  void SetFromRects(m2::AnyRectD const & glbRect, m2::RectD const & pxRect);
  void SetOrg(m2::PointD const & p);

  void Move(double dx, double dy);
  void MoveG(m2::PointD const & p);

  /// scale global rect
  void Scale(double scale);
  void Rotate(double angle);

  void OnSize(m2::RectI const & r);
  void OnSize(int x0, int y0, int w, int h);

  double GetScale() const { return m_Scale; }
  void SetScale(double scale);

  double GetAngle() const { return m_Angle.val(); }
  void SetAngle(double angle);

  m2::PointD const & GetOrg() const { return m_Org; }

  int GetWidth() const;
  int GetHeight() const;

  inline m2::PointD GtoP(m2::PointD const & pt) const
  {
    return pt * m_GtoP;
  }

  inline m2::PointD PtoG(m2::PointD const & pt) const
  {
    return pt * m_PtoG;
  }

  inline void GtoP(double & x, double & y) const
  {
    double tempX = x;
    x = tempX * m_GtoP(0, 0) + y * m_GtoP(1, 0) + m_GtoP(2, 0);
    y = tempX * m_GtoP(1, 0) + y * m_GtoP(1, 1) + m_GtoP(2, 1);
  }

  inline void PtoG(double & x, double & y) const
  {
    double tempX = x;
    x = tempX * m_PtoG(0, 0) + y * m_PtoG(1, 0) + m_PtoG(2, 0);
    y = tempX * m_PtoG(0, 1) + y * m_PtoG(1, 1) + m_PtoG(2, 1);
  }

  void GtoP(m2::RectD const & gr, m2::RectD & sr) const;
  void PtoG(m2::RectD const & pr, m2::RectD & gr) const;

  void GetTouchRect(m2::PointD const & pixPoint, double pixRadius, m2::AnyRectD & glbRect) const;
  void GetTouchRect(m2::PointD const & pixPoint, double const pxWidth,
                    double const pxHeight, m2::AnyRectD & glbRect) const;

  MatrixT const & GtoPMatrix() const { return m_GtoP; }
  MatrixT const & PtoGMatrix() const { return m_PtoG; }

  m2::RectD const & PixelRect() const { return m_PixelRect; }
  m2::AnyRectD const & GlobalRect() const { return m_GlobalRect; }
  m2::RectD const & ClipRect() const { return m_ClipRect; }

  double GetMinPixelRectSize() const;

  /// Compute arbitrary pixel transformation, that translates the (oldPt1, oldPt2) -> (newPt1, newPt2)
  static MatrixT const CalcTransform(m2::PointD const & oldPt1, m2::PointD const & oldPt2,
                                     m2::PointD const & newPt1, m2::PointD const & newPt2);

  /// Setting GtoP matrix extracts the Angle and m_Org parameters, leaving PixelRect intact
  void SetGtoPMatrix(MatrixT const & m);

  /// Extracting parameters from matrix, that is supposed to represent GtoP transformation
  static void ExtractGtoPParams(MatrixT const & m, double & a, double & s, double & dx, double & dy);

  bool operator != (ScreenBase const & src) const
  {
    return !(*this == src);
  }

  bool operator == (ScreenBase const & src) const
  {
    return (m_GtoP == src.m_GtoP) && (m_PtoG == src.m_PtoG);
  }
};

/// checking whether the s1 transforms into s2 without scaling, only with shift and rotation
bool IsPanningAndRotate(ScreenBase const & s1, ScreenBase const & s2);
