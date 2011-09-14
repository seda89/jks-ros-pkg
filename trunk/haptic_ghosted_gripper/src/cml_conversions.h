#ifndef _CML_CONVERSIONS_H_
#define _CML_CONVERSIONS_H_

#include <cml/cml.h>
#include <tf/tf.h>


namespace cml_tools
{

inline tf::Vector3 cmlVectorToTF(const cml::vector3d &v)
{
  tf::Vector3 vtf;
  vtf.setX(v[0]);
  vtf.setY(v[1]);
  vtf.setZ(v[2]);
  return vtf;
}

inline btMatrix3x3 cmlMatrixToTF(const cml::matrix33d &rot)
{
  return btMatrix3x3( rot.data()[0], rot.data()[1], rot.data()[2],
                      rot.data()[3], rot.data()[4], rot.data()[5],
                      rot.data()[6], rot.data()[7], rot.data()[8] );
}

inline btQuaternion cmlMatrixToTFQuaternion(const cml::matrix33d &m)
{
  tf::Quaternion q;
  cmlMatrixToTF(m).getRotation(q);
  return q;
}

} // namespace cml_tools

inline void setBTMatrixColumns(btMatrix3x3 &mat, const tf::Vector3 &X, const tf::Vector3 &Y, const tf::Vector3 &Z )
{
  mat.setValue(X.x(), Y.x(), Z.x(),
               X.y(), Y.y(), Z.y(),
               X.z(), Y.z(), Z.z());

}


#endif
