template <class FLOAT, size_t N>
AxisAlignedBoundingBox<FLOAT, N>::AxisAlignedBoundingBox(Vector<FLOAT,N> center, Vector<FLOAT,N> half_edge_length)
  : center(center), half_edge_length(half_edge_length)
{
}


template <class FLOAT, size_t N>
bool AxisAlignedBoundingBox<FLOAT, N>::intersects(AxisAlignedBoundingBox<FLOAT,N> aabb) const {
  bool intersects = true;
  for (size_t i = 0; i < N; i++) {
    intersects &= (center[i] - half_edge_length[i] - aabb.half_edge_length[i] <= aabb.center[i]) 
                     & (aabb.center[i] <= center[i] + half_edge_length[i] + aabb.half_edge_length[i]);
  }
  return intersects;
}

template <class FLOAT, size_t N>
bool AxisAlignedBoundingBox<FLOAT, N>::intersects(Ray<FLOAT,N> ray) const {
    FLOAT tmin;
    FLOAT tmax;
    FLOAT tminimum = -INFINITY;
    FLOAT tmaximum = INFINITY;
    
    for (size_t i = 0; i < N; i++) {
      tmin = (center[i] - ray.origin[i] - half_edge_length[i]) / ray.direction[i];
      tmax = (center[i] - ray.origin[i] + half_edge_length[i]) / ray.direction[i];
      tminimum = std::max(tminimum, std::min(tmin, tmax) );
      tmaximum = std::min(tmaximum, std::max(tmin, tmax) );      
    }
    return tmaximum >= tminimum;
}


template <class FLOAT, size_t N>
bool AxisAlignedBoundingBox<FLOAT, N>::intersects(AxisAlignedBoundingBox<FLOAT,N> aabb, Vector<FLOAT, N> direction) const {
  AxisAlignedBoundingBox<FLOAT,N> center_extended = AxisAlignedBoundingBox(center, half_edge_length + aabb.half_edge_length);

  return center_extended.intersects( Ray{ aabb.center, direction} );
}

template <class FLOAT, size_t N>
Vector<FLOAT, N> AxisAlignedBoundingBox<FLOAT, N>::sweep_intersects(AxisAlignedBoundingBox<FLOAT,N> aabb, Vector<FLOAT, N> direction) const {
    Vector<FLOAT, N> normal = {0.0, };
    FLOAT tmin[N];
    FLOAT tmax[N];
    
    for (size_t i = 0; i < N; i++) {
      tmin[i] = (center[i] - aabb.center[i] - (half_edge_length[i] + aabb.half_edge_length[i])) / direction[i];
      tmax[i] = (center[i] - aabb.center[i] + (half_edge_length[i] + aabb.half_edge_length[i])) / direction[i];
    }
    FLOAT tminimum = -INFINITY;
    FLOAT tmaximum = INFINITY;
    for (size_t i = 0; i < N; i++) { 
      tminimum = std::max(tminimum, std::min(tmin[i], tmax[i]) );
      tmaximum = std::min(tmaximum, std::max(tmin[i], tmax[i]) );
    }

    
    if (tmaximum >= tminimum) {    
      for (size_t i = 0; i < N; i++) {         
        normal[i] = - ( (tminimum == tmin[i]) | (tminimum == tmax[i]) ) * direction[i];
      }
    }
    return normal;
}

template <class FLOAT, size_t N>
Sphere<FLOAT,N>::Sphere(Vector<FLOAT,N> center, FLOAT radius)
 : center(center), radius(radius)
{
}

template <class FLOAT, size_t N>
bool Sphere<FLOAT,N>::intersects(Sphere<FLOAT, N> sphere) const {
  return (radius + sphere.radius) * (radius + sphere.radius) >= (center - sphere.center).square_of_length();
}

template <class FLOAT, size_t N>
bool Sphere<FLOAT,N>::inside(const Vector<FLOAT, N> p) const {
  return  (p - center) * (p - center) <= radius * radius;
}

// solution via
// (g(t) - center )^2  = ( (ray.origin - center) + t ray.direction)^2 = r^2 
// and abc-formula
template <class FLOAT, size_t N>
FLOAT Sphere<FLOAT,N>::intersects(const Ray<FLOAT, N> &ray) const {
  Vector<FLOAT,N> om = ray.origin - center;
  FLOAT  a = ray.direction * ray.direction,
         b = 2.0 * (om * ray.direction),
         c = om * om - radius * radius,
         d = b * b - 4.0 * a * c;
  if (d < 0) {
   return 0;
  }
  d = sqrt(d);
  if ( inside( ray.origin ) ) {
    return 0.5 * std::max(-b + d, -b - d) / a;
  }
  return 0.5 * std::min( std::max<FLOAT>(0.0, (-b + d)) , (-b - d) ) / a; 
}

template <class FLOAT, size_t N>
bool Sphere<FLOAT,N>::intersects(const Ray<FLOAT, N> &ray, Intersection_Context<FLOAT, N> & context) const {
  FLOAT t = intersects(ray);
  if (t <= 0.0) {
    return false;
  }
  context.t = t;
  context.intersection = ray.origin + t * ray.direction;
  context.normal = context.intersection - center;
  context.normal.normalize();
  if ( inside( ray.origin ) ) {
    context.normal = static_cast<FLOAT>(-1.0) * context.normal; // ray starts inside sphere, normal points to the inside;
  }
  return true;
}

template <class FLOAT, size_t N>
Triangle<FLOAT, N>::Triangle(Vector<FLOAT, N> a, Vector<FLOAT, N> b, Vector<FLOAT, N> c, Vector<FLOAT, N> na, Vector<FLOAT, N> nb, Vector<FLOAT, N> nc)
 : a(a), b(b), c(c), na(na), nb(nb), nc(nc) { }


template <class FLOAT, size_t N>
Triangle<FLOAT, N>::Triangle(Vector<FLOAT, N> a, Vector<FLOAT, N> b, Vector<FLOAT, N> c, Vector<FLOAT, N> normal)
 : Triangle<FLOAT, N>::Triangle(a, b, c, normal, normal, normal) { }

template <class FLOAT, size_t N>
Triangle<FLOAT, N>::Triangle(Vector<FLOAT, N> a, Vector<FLOAT, N> b, Vector<FLOAT, N> c)
  :  Triangle<FLOAT, N>::Triangle(a, b, c, (c - a).cross_product(b - a) ) { }

  
template <class FLOAT, size_t N>
bool Triangle<FLOAT, N>::intersects(const Ray<FLOAT, N> &ray, Intersection_Context<FLOAT, N> & context) const {
  return intersects(ray, context.normal, context.intersection, context.u, context.v, context.t);
}


template <class FLOAT, size_t N>
bool Triangle<FLOAT, N>::intersects(const Ray<FLOAT, N> &ray, Vector<FLOAT, N> & normal, Vector<FLOAT, N> & p, FLOAT & u, FLOAT & v, FLOAT & t) const {
    const FLOAT EPSILON = 10e-7;
    normal =  (b-a).cross_product(c-a);  // points away from triangle surface (clockwise order)

    FLOAT normalRayProduct =  normal * ray.direction;
    FLOAT area = normal.length(); // used for u-v-parameter calculation


    if ( fabs(normalRayProduct) < EPSILON ) { // backface culling off
      return false;
    }

    FLOAT d = normal * a;
    t = (d - normal * ray.origin) / normalRayProduct;

    if ( t < 0.0 ) {
      return false;
    }
   
    p = ray.origin + t * ray.direction;
   
    Vector<FLOAT, N> vector = (b - a).cross_product(p - a );
    if ( normal * vector < 0.0 ) { 
      return false;
    }

    
    vector = (c - b).cross_product(p - b );
    if ( normal * vector < 0.0 ) { 
      return false;
    }

    u = vector.length()  / area;

    vector = (a-c).cross_product(p - c );
    if (normal * vector < 0.0 ) {
      return false;
    }

    v = vector.length() / area;

    return true;
}



/*    
template <class FLOAT, size_t N>
bool Triangle<FLOAT, N>::intersects(const Ray<FLOAT, N> &ray, Vector<FLOAT, N> & normal, Vector<FLOAT, N> & p, FLOAT & u, FLOAT & v, FLOAT & t) const {
  const FLOAT EPSILON = 10e-7;
  Vector<FLOAT, N> ca = a - c;
  Vector<FLOAT, N> cb = b - c;
  normal = ca.cross_product(cb);

  FLOAT d = normal * a;
  FLOAT normal_direction = normal * ray.direction;
  
//  std::cerr << "normal_direction = " << normal_direction << std::endl;

  if ( 
    //normal_direction > -EPSILON && // backface culling off 
    normal_direction < EPSILON
    ) { 
    return false;
  }
  
  t = (d - normal * ray.origin) / normal_direction;

  //std::cerr << "t = " << t << std::endl;

  if (t < 0.0) {  
    return false;
  }
  
  p = ray.origin + t * ray.direction;
  
  Vector<FLOAT, N> cp = p - c;

  size_t c1 = 0,
         c2 = 1;
  FLOAT d_inv = (ca[0] * cb[1] - ca[1] * cb[0]);

  if ( fabs(d_inv) > EPSILON ) {
    c1 = 0;
    c2 = 1;
  } else if (fabs(d_inv = (ca[0] * cb[2] - ca[2] * cb[0])) > EPSILON ) {
    c1 = 0;
    c2 = 2;
  } else if (fabs(d_inv = (ca[1] * cb[2] - ca[2] * cb[1])) > EPSILON ) {
    c1 = 1;
    c2 = 2;
  } else {
    std::cerr << "degenerated case of triangle" << std::endl;
  }
  
  d_inv = 1.0 / d_inv;
    
  u = d_inv * (cp[c1] * cb[c2] - cp[c2] * cb[c1]);

  //std::cerr << "u = " << u << std::endl;

  if ( (u < 0.0) || (u > 1.0) ) {
    return false;
  }

  v = d_inv * (ca[c1] * cp[c2] - ca[c2] * cp[c1]);

  //std::cerr << "v = " << v << std::endl;

  if ( (v < 0.0) || ((u + v) > 1.0) ) {
    return false;
  }
    
  return t > 0.0;
}
*/


// Möller-Trumbore, still BUGGY
/*
template <class FLOAT, size_t N>
bool Triangle<FLOAT, N>::intersects(const Ray<FLOAT, N> &ray, Vector<FLOAT, N> & normal, Vector<FLOAT, N> & p, FLOAT & u, FLOAT & v, FLOAT & t) const {
  const FLOAT EPSILON = 10e-7;
  Vector<FLOAT, N> ca = a - c;
  Vector<FLOAT, N> cb = b - c;

  normal = ray.direction.cross_product( cb );

  
//  std::cerr << "ca = ( " << ca[0] << ", " << ca[1] << ", " << ca[2] << ") " << std::endl; 
//  std::cerr << "cb = ( " << cb[0] << ", " << cb[1] << ", " << cb[2] << ") " << std::endl; 
//  std::cerr << "normal = ( " << normal[0] << ", " << normal[1] << ", " << normal[2] << ") " << std::endl; 
  FLOAT det = normal * ca; 

//  std::cout << "det = " << det << std::endl;

  if ( 
      // det > -EPSILON &&  // backface culling off
      fabs(det) < EPSILON) { 
    std::cerr << "|det| < epsilon" << std::endl;
    return false;
  }

  det = 1.0 / det;  

  Vector<FLOAT, N> co = ray.origin - c;
  std::cerr << "co = ( " << co[0] << ", " << co[1] << ", " << co[2] << ") " << std::endl; 
  
  
  u = (normal * co) * det;

  std::cerr << "u = " << u << std::endl;
  
  if ( (u < 0.0) || (u > 1.0) ) {
    return false;
  }
  
  Vector<FLOAT, N> f = co.cross_product(ca);
  v = (ray.direction * f) * det;

  std::cerr << "v = " << v << std::endl;
  
  if ( (v < 0.0) || ((u + v) > 1.0) ) {
    return false;
  }

  t = ( f * cb ) * det;
  
  std::cerr << "t = " << t << std::endl;
  if (t > 0.0) {
    p = ray.origin + t * ray.direction;
    return true;
  }
  
  return false;
}
*/

/*
template <class FLOAT, size_t N>
bool Triangle<FLOAT, N>::intersects(const Ray<FLOAT, N> &ray, Vector<FLOAT, N> & normal, Vector<FLOAT, N> & p, FLOAT & u, FLOAT & v, FLOAT & t) const {
  const FLOAT EPSILON = 10e-7;
  Vector<FLOAT, N> ca = a - c;
  Vector<FLOAT, N> cb = b - c;
  normal = cb.cross_product( ca );

  //this->print(std::cerr);
  //std::cerr << "ray.origin = ( " << ray.origin[0] << ", " << ray.origin[1] << ", " << ray.origin[2] << ")" << std::endl;  
  //std::cerr << "ray.direction = ( " << ray.direction[0] << ", " << ray.direction[1] << ", " << ray.direction[2] << ")" << std::endl;  
  //std::cerr << "ca = ( " << ca[0] << ", " << ca[1] << ", " << ca[2] << ") " << std::endl; 
  //std::cerr << "cb = ( " << cb[0] << ", " << cb[1] << ", " << cb[2] << ") " << std::endl; 
  //std::cerr << "normal = ( " << normal[0] << ", " << normal[1] << ", " << normal[2] << ") " << std::endl; 

  FLOAT det = ray.direction * normal; 
  
  //std::cerr << "det = " << det << std::endl;

  if ( -det  < EPSILON ) { // backface culling on
    //std::cerr << "|det| < epsilon" << std::endl;
    return false;
  }

  det = 1.0 / det;  

  Vector<FLOAT, N> co = ray.origin - c;
  //std::cerr << "co = ( " << co[0] << ", " << co[1] << ", " << co[2] << ") " << std::endl; 
  
  
  u = (ray.direction * ( cb.cross_product(co) ) ) * det;


  //std::cerr << "u = " << u << std::endl;
  
  if ( (u < 0.0) || (u > 1.0) ) {
    return false;
  } 
  
  v = (ray.direction * (co.cross_product(ca)) ) * det;

  //std::cerr << "v = " << v << std::endl;
  
  if ( (v < 0.0) || ((u + v) > 1.0) ) {
    return false;
  }

  t = -( co * normal ) * det;
  
  //std::cerr << "t = " << t << std::endl;
  if (t > 0.0) {
    p = ray.origin + t * ray.direction;
    //Vector<FLOAT, N> pp = u * a + v * b + ( static_cast<FLOAT>(1.0) - u - v) * c;
    
    //std::cerr << "p = (" << p[0] << ", " << p[1] << ", " << p[2] << ")" << std::endl;
    //std::cerr << "pp = (" << pp[0] << ", " << pp[1] << ", " << pp[2] << ")" << std::endl;
    normal = static_cast<FLOAT>(-1.0) * normal;
    return true;
  }
  
  return false;
}
*/


template <class FLOAT, size_t N>
bool refract(FLOAT refraction_index, Vector<FLOAT, N> normal, Vector<FLOAT, N> direction, Vector<FLOAT, N> & transmission) {
   FLOAT cos_theta = direction * normal; // both vectors need to be normalized
   FLOAT sin_phi_squared = refraction_index * refraction_index * (static_cast<FLOAT>(1.0) - cos_theta * cos_theta);
     
   if ( sin_phi_squared > static_cast<FLOAT>(1.0f) ) {
     return false; // total internal reflection
   }
   FLOAT cos_phi = sqrt( static_cast<FLOAT>(1.0f) - sin_phi_squared );
   transmission = refraction_index * (direction - cos_theta * normal  ) - cos_phi * normal;
   
   return true;
}

