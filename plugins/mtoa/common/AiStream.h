#pragma once

#include <iostream>
inline std::ostream& 
operator<<( std::ostream& s, const AtVector& v )
{
   return s << "(" << v.x << ", " << v.y << ", " << v.z << ")";
}
inline std::ostream& 
operator<<( std::ostream& s, const AtVector2& v )
{
   return s << "(" << v.x << ", " << v.y << ")";
}

inline std::ostream& 
operator<<( std::ostream& s, const AtBBox& b )
{
   return s << "(" << b.min << ", " << b.max << ")";
}

inline std::ostream& 
operator<<( std::ostream& s, const AtBBox2& b )
{
   return s << "((" << b.minx << ", " << b.miny << ") ("<< b.maxx << ", " << b.maxy << "))";
}

inline std::ostream& 
operator<<( std::ostream& s, const AtMatrix& m )
{
    return s << "(" << m[0][0] << " " << m[0][1] << " " << m[0][2] << " " << m[0][3] << "\n" <<
            " " << m[1][0] << " " << m[1][1] << " " << m[1][2] << " " << m[1][3] << "\n" <<
            " " << m[2][0] << " " << m[2][1] << " " << m[2][2] << " " << m[2][3] << "\n" <<
            " " << m[3][0] << " " << m[3][1] << " " << m[3][2] << " " << m[3][3] << ")\n";
}




