// ======================================================================== //
// Copyright 2009-2017 Intel Corporation                                    //
//                                                                          //
// Licensed under the Apache License, Version 2.0 (the "License");          //
// you may not use this file except in compliance with the License.         //
// You may obtain a copy of the License at                                  //
//                                                                          //
//     http://www.apache.org/licenses/LICENSE-2.0                           //
//                                                                          //
// Unless required by applicable law or agreed to in writing, software      //
// distributed under the License is distributed on an "AS IS" BASIS,        //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. //
// See the License for the specific language governing permissions and      //
// limitations under the License.                                           //
// ======================================================================== //

#pragma once

/*! \ingroup embree_kernel_api */
/*! \{ */

#if defined(__cplusplus)
extern "C" {
#endif
  
/*! invalid geometry ID */
#define RTC_INVALID_GEOMETRY_ID ((unsigned)-1)

/*! maximal number of time steps */
#define RTC_MAX_TIME_STEPS 129

/*! maximal number of user vertex buffers */
#define RTC_MAX_USER_VERTEX_BUFFERS 16

/*! maximal number of index buffers for subdivision surfaces */
#define RTC_MAX_INDEX_BUFFERS 16

/*! \brief Specifies the type of buffers when mapping buffers */
enum RTCBufferType 
{
  RTC_INDEX_BUFFER         = 0x01000000,
  RTC_INDEX_BUFFER0        = 0x01000000,
  RTC_INDEX_BUFFER1        = 0x01000001,
  /* ... */
  
  RTC_VERTEX_BUFFER        = 0x02000000,
  RTC_VERTEX_BUFFER0       = 0x02000000,
  RTC_VERTEX_BUFFER1       = 0x02000001,
  /* ... */

  RTC_USER_VERTEX_BUFFER   = 0x02100000,
  RTC_USER_VERTEX_BUFFER0  = 0x02100000,
  RTC_USER_VERTEX_BUFFER1  = 0x02100001,
  /* ... */

  RTC_FACE_BUFFER          = 0x03000000,
  RTC_LEVEL_BUFFER         = 0x04000000,

  RTC_EDGE_CREASE_INDEX_BUFFER = 0x05000000,
  RTC_EDGE_CREASE_WEIGHT_BUFFER = 0x06000000,

  RTC_VERTEX_CREASE_INDEX_BUFFER = 0x07000000,
  RTC_VERTEX_CREASE_WEIGHT_BUFFER = 0x08000000,

  RTC_HOLE_BUFFER          = 0x09000000,
};

#define RTC_INDEX_BUFFER_(i)  (enum RTCBufferType)(RTC_INDEX_BUFFER+(i))
#define RTC_VERTEX_BUFFER_(i) (enum RTCBufferType)(RTC_VERTEX_BUFFER+(i))
#define RTC_USER_VERTEX_BUFFER_(i) (enum RTCBufferType)(RTC_USER_VERTEX_BUFFER+(i))

/*! \brief Supported types of matrix layout for functions involving matrices */
enum RTCMatrixType {
  RTC_MATRIX_ROW_MAJOR = 0,
  RTC_MATRIX_COLUMN_MAJOR = 1,
  RTC_MATRIX_COLUMN_MAJOR_ALIGNED16 = 2,
};

/*! \brief Interpolation mode for subdivision surfaces. The modes are
 *  ordered to interpolate successively more linear. */
enum RTCSubdivisionMode
{
  RTC_SUBDIV_NO_BOUNDARY = 0,          //!< ignores border patches
  RTC_SUBDIV_SMOOTH_BOUNDARY = 1,      //!< smooth border (default)
  RTC_SUBDIV_PIN_CORNERS = 2,          //!< smooth border with fixed corners
  RTC_SUBDIV_PIN_BOUNDARY = 3,         //!< linearly interpolation along border
  RTC_SUBDIV_PIN_ALL = 4,              //!< pin every vertex (interpolates every patch linearly)
};

/*! Curve Basis */
enum RTCCurveBasis
{
  RTC_BASIS_LINEAR = 0,
  RTC_BASIS_BEZIER = 1,
  RTC_BASIS_BSPLINE = 2
};

/*! Curve type */
enum RTCCurveType
{
  RTC_CURVE_RIBBON,    //!< render curves as ray facing ribbons
  RTC_CURVE_SURFACE    //!< render curves as real geometric surface
};

/*! Arguments for RTCBoundsFunction */
struct RTCBoundsFunctionArguments
{
  void* userPtr;         /*!< pointer to user data */
  void* geomUserPtr;     /*!< pointer to geometry user data */
  unsigned int item;           /*!< item to calculate bounds for */
  unsigned int time;           /*!< time to calculate bounds for */
  struct RTCBounds* bounds_o;    /*!< returns calculated bounds */
};
  
/*! Type of bounding function. */
typedef void (*RTCBoundsFunction)(const struct RTCBoundsFunctionArguments* const args);

/*! Arguments for RTCIntersectFunctionN */
struct RTCIntersectFunctionNArguments
{
  const int* valid;                          /*!< pointer to valid mask */
  void* geomUserPtr;                                 /*!< pointer to geometry user data */
  struct RTCIntersectContext* context;       /*!< intersection context as passed to rtcIntersect/rtcOccluded */
  struct RTCRayN* rays;                      /*!< ray packet to intersect */
  unsigned int N;                            /*!< number of rays in packet */
  unsigned int item;                          /*!< item to intersect */
};
  
/*! Type of intersect function pointer for ray packets of size N. */
typedef void (*RTCIntersectFunctionN)(const struct RTCIntersectFunctionNArguments* const args);

/*! Arguments for RTCOccludedFunctionN */
struct RTCOccludedFunctionNArguments
{
  const int* valid;                          /*!< pointer to valid mask */
  void* geomUserPtr;                                 /*!< pointer to geometry user data */
  struct RTCIntersectContext* context;       /*!< intersection context as passed to rtcIntersect/rtcOccluded */
  struct RTCRayN* rays;                      /*!< ray packet to intersect */
  unsigned int N;                            /*!< number of rays in packet */
  unsigned int item;                          /*!< item to intersect */
};
  
/*! Type of occlusion function pointer for ray packets of size N. */
typedef void (*RTCOccludedFunctionN) (const struct RTCOccludedFunctionNArguments* const args);

/*! Arguments for RTCDisplacementFunction callback */
struct RTCDisplacementFunctionArguments
{
  void* geomUserPtr;      /*!< pointer to user data of geometry */
  unsigned int geomID;    /*!< ID of geometry to displace */
  unsigned int primID;    /*!< ID of primitive of geometry to displace */
  unsigned int time;      /*!< time step to calculate displacement for */
  const float* u;         /*!< u coordinates (source) */
  const float* v;         /*!< v coordinates (source) */
  const float* nx;        /*!< x coordinates of normalized normal at point to displace (source) */
  const float* ny;        /*!< y coordinates of normalized normal at point to displace (source) */
  const float* nz;        /*!< z coordinates of normalized normal at point to displace (source) */
  float* px;              /*!< x coordinates of points to displace (source and target) */
  float* py;              /*!< y coordinates of points to displace (source and target) */
  float* pz;              /*!< z coordinates of points to displace (source and target) */
  unsigned int N;         /*!< number of points to displace */
};
 
/*! Displacement mapping function. */
typedef void (*RTCDisplacementFunction)(const struct RTCDisplacementFunctionArguments* const args);

/*! \brief Defines an opaque geometry type */
typedef struct __RTCGeometry* RTCGeometry;

/*! Creates a new user geometry object. This feature makes it possible
 *  to add arbitrary types of geometry to the scene by providing
 *  appropiate bounding, intersect and occluded functions. A user
 *  geometry object is a set of user geometries. As the rtcIntersect
 *  and rtcOccluded functions support different ray packet sizes, the
 *  user also has to provide different versions of intersect and
 *  occluded function pointers for these packet sizes. However, the
 *  ray packet size of the called function pointer always matches the
 *  packet size of the originally invoked rtcIntersect and rtcOccluded
 *  functions. A user data pointer, that points to a user specified
 *  representation of the geometry, is passed to each intersect and
 *  occluded function invokation, as well as the index of the geometry
 *  of the set to intersect. */
RTCORE_API RTCGeometry rtcNewUserGeometry (RTCDevice device,
                                           unsigned int numGeometries,    /*!< the number of geometries contained in the set */
                                           unsigned int numTimeSteps  /*!< number of motion blur time steps */
  );

/*! Sets the bounding function to calculate bounding boxes of the user
 *  geometry items when building spatial index structures. The
 *  calculated bounding box have to be conservative and should be
 *  tight. */
RTCORE_API void rtcSetBoundsFunction (RTCGeometry hgeometry, RTCBoundsFunction bounds, void* userPtr);

/*! Set intersect function for ray packets of size N. The rtcIntersectN function
 *  will call the passed function for intersecting the user
 *  geometry. */
RTCORE_API void rtcSetIntersectFunction(RTCGeometry hgeometry, RTCIntersectFunctionN intersect);

/*! Set occlusion function for ray packets of size N. The rtcOccludedN function
 *  will call the passed function for intersecting the user
 *  geometry. */
RTCORE_API void rtcSetOccludedFunction(RTCGeometry hgeometry, RTCOccludedFunctionN occluded);

/*! \brief Creates a new scene instance. 

  A scene instance contains a reference to a scene to instantiate and
  the transformation to instantiate the scene with. For motion blurred
  instances, a number of timesteps can get specified. An
  implementation will typically transform the ray with the inverse of
  the provided transformation (or inverse of linearly interpolated
  transformation in case of multi-segment motion blur) and continue
  traversing the ray through the provided scene. If any geometry is
  hit, the instance ID (instID) member of the ray will get set to the
  geometry ID of the instance. */
RTCORE_API RTCGeometry rtcNewInstance (RTCDevice device,
                                       RTCScene source,                  //!< the scene to instantiate
                                       unsigned int numTimeSteps);         //!< number of timesteps, one matrix per timestep

/*! \brief Creates a new geometry instance. 

  WARNING: This function is deprecated, do not use it.

  A geometry instance contains a reference to some geometry to
  instantiate and the transformation to instantiate that geometry
  with. An implementation will typically transform the ray with the
  inverse of the provided transformation and continue traversing the
  ray through the provided scene. If any geometry is hit, the geometry
  ID (geomID) member of the ray will get set to the geometry ID of the
  instance. */
RTCORE_API RTCORE_DEPRECATED RTCGeometry rtcNewGeometryInstance(RTCDevice device, RTCScene scene, unsigned geomID);
RTCORE_API RTCORE_DEPRECATED RTCGeometry rtcNewGeometryGroup   (RTCDevice device, RTCScene scene, unsigned* geomIDs, unsigned int N);

/*! \brief Sets transformation of the instance for specified timestep */
RTCORE_API void rtcSetTransform ( RTCGeometry geometry,                   //!< ID of geometry 
                                  enum RTCMatrixType layout,                   //!< layout of transformation matrix
                                  const float* xfm,                       //!< pointer to transformation matrix
                                  unsigned int timeStep                     //!< timestep to set the matrix for 
  );

/*! \brief Creates a new triangle mesh. The number of triangles
  (numTriangles), number of vertices (numVertices), and number of time
  steps (1 for normal meshes, and up to RTC_MAX_TIME_STEPS for multi
  segment motion blur), have to get specified. The triangle indices
  can be set by mapping and writing to the index buffer
  (RTC_INDEX_BUFFER) and the triangle vertices can be set by mapping
  and writing into the vertex buffer (RTC_VERTEX_BUFFER). In case of
  multi-segment motion blur, multiple vertex buffers have to get filled
  (RTC_VERTEX_BUFFER0, RTC_VERTEX_BUFFER1, etc.), one for each time
  step. The index buffer has the default layout of three 32 bit
  integer indices for each triangle. An index points to the ith
  vertex. The vertex buffer stores single precision x,y,z floating
  point coordinates aligned to 16 bytes. The value of the 4th float
  used for alignment can be arbitrary. */
RTCORE_API RTCGeometry rtcNewTriangleMesh (RTCDevice device);

/*! \brief Creates a new quad mesh. The number of quads (numQuads),
  number of vertices (numVertices), and number of time steps (1 for
  normal meshes, and up to RTC_MAX_TIME_STEPS for multi-segment motion
  blur), have to get specified. The quad indices can be set by mapping
  and writing to the index buffer (RTC_INDEX_BUFFER) and the quad
  vertices can be set by mapping and writing into the vertex buffer
  (RTC_VERTEX_BUFFER). In case of multi-segment motion blur, multiple
  vertex buffers have to get filled (RTC_VERTEX_BUFFER0,
  RTC_VERTEX_BUFFER1, etc.), one for each time step. The index buffer has
  the default layout of three 32 bit integer indices for each quad. An
  index points to the ith vertex. The vertex buffer stores single
  precision x,y,z floating point coordinates aligned to 16 bytes. The
  value of the 4th float used for alignment can be arbitrary. */
RTCORE_API RTCGeometry rtcNewQuadMesh (RTCDevice device);

/*! \brief Creates a new subdivision mesh. The number of faces
 (numFaces), edges/indices (numEdges), vertices (numVertices), edge
 creases (numEdgeCreases), vertex creases (numVertexCreases), holes
 (numHoles), and time steps (numTimeSteps) have to get speficied at
 construction time.

 The following buffers have to get filled by the application: the face
 buffer (RTC_FACE_BUFFER) contains the number edges/indices (3 or 4)
 of each of the numFaces faces, the index buffer (RTC_INDEX_BUFFER)
 contains multiple (3 or 4) 32bit vertex indices for each face and
 numEdges indices in total, the vertex buffer (RTC_VERTEX_BUFFER)
 stores numVertices vertices as single precision x,y,z floating point
 coordinates aligned to 16 bytes. The value of the 4th float used for
 alignment can be arbitrary. In case of multi-segment motion blur,
 multiple vertex buffers have to get filled (RTC_VERTEX_BUFFER0,
 RTC_VERTEX_BUFFER1, etc.), one for each time step.

 Optionally, the application can fill the hole buffer
 (RTC_HOLE_BUFFER) with numHoles many 32 bit indices of faces that
 should be considered non-existing.

 Optionally, the application can fill the level buffer
 (RTC_LEVEL_BUFFER) with a tessellation level for each of the numEdges
 edges. The subdivision level is a positive floating point value, that
 specifies how many quads along the edge should get generated during
 tessellation. The tessellation level is a lower bound, thus the
 implementation is free to choose a larger level. If no level buffer
 is specified a level of 1 is used.

 Optionally, the application can fill the sparse edge crease buffers
 to make some edges appear sharper. The edge crease index buffer
 (RTC_EDGE_CREASE_INDEX_BUFFER) contains numEdgeCreases many pairs of
 32 bit vertex indices that specify unoriented edges. The edge crease
 weight buffer (RTC_EDGE_CREASE_WEIGHT_BUFFER) stores for each of
 theses crease edges a positive floating point weight. The larger this
 weight, the sharper the edge. Specifying a weight of infinify is
 supported and marks an edge as infinitely sharp. Storing an edge
 multiple times with the same crease weight is allowed, but has lower
 performance. Storing the an edge multiple times with different
 crease weights results in undefined behaviour. For a stored edge
 (i,j), the reverse direction edges (j,i) does not have to get stored,
 as both are considered the same edge.

 Optionally, the application can fill the sparse vertex crease buffers
 to make some vertices appear sharper. The vertex crease index buffer
 (RTC_VERTEX_CREASE_INDEX_BUFFER), contains numVertexCreases many 32
 bit vertex indices to speficy a set of vertices. The vertex crease
 weight buffer (RTC_VERTEX_CREASE_WEIGHT_BUFFER) specifies for each of
 these vertices a positive floating point weight. The larger this
 weight, the sharper the vertex. Specifying a weight of infinity is
 supported and makes the vertex infinitely sharp. Storing a vertex
 multiple times with the same crease weight is allowed, but has lower
 performance. Storing a vertex multiple times with different crease
 weights results in undefined behaviour.

*/
RTCORE_API RTCGeometry rtcNewSubdivisionMesh (RTCDevice device);

/*! \brief Creates a new hair geometry, consisting of multiple hairs
  represented as cubic bezier curves with varying radii. The number of
  curves (numCurves), number of vertices (numVertices), and number of
  time steps have to get specified at construction time (1 for normal
  meshes, and up to RTC_MAX_TIME_STEPS for multi-segment motion
  blur). Further, the curve index buffer (RTC_INDEX_BUFFER) and the
  curve vertex buffer (RTC_VERTEX_BUFFER) have to get set by mapping
  and writing to the appropiate buffers. In case of multi-segment
  motion blur multiple vertex buffers have to get filled
  (RTC_VERTEX_BUFFER0, RTC_VERTEX_BUFFER1, etc.), one for each time
  step. The index buffer has the default layout of a single 32 bit
  integer index for each curve, that references the start vertex of
  the curve. The vertex buffer stores 4 control points per curve, each
  such control point consists of a single precision (x,y,z) position
  and radius, stored in that order in memory. Individual hairs are
  considered to be subpixel sized which allows the implementation to
  approximate the intersection calculation. This in particular means
  that zooming onto one hair might show geometric artefacts. */

RTCORE_API RTCGeometry rtcNewCurveGeometry (RTCDevice device,
                                            enum RTCCurveType type,
                                            enum RTCCurveBasis basis
  );


/*! Sets the type of the curve geometry */
RTCORE_API void rtcSetCurveType(RTCGeometry geometry, enum RTCCurveType type);

/*! Sets a uniform tessellation rate for subdiv meshes and hair
 *  geometry. For subdivision meshes the RTC_LEVEL_BUFFER can also be used
 *  optionally to set a different tessellation rate per edge.*/
RTCORE_API void rtcSetTessellationRate (RTCGeometry geometry, float tessellationRate);

/*! sets the build quality of the geometry */
RTCORE_API void rtcSetGeometryBuildQuality(RTCGeometry geometry, enum RTCBuildQuality quality);

/*! \brief Sets 32 bit ray mask. */
RTCORE_API void rtcSetMask (RTCGeometry geometry, int mask);

/*! \brief Sets subdivision interpolation mode for specified subdivision surface topology */
RTCORE_API void rtcSetSubdivisionMode(RTCGeometry geometry, unsigned topologyID, enum RTCSubdivisionMode mode);

/*! \brief Binds a user vertex buffer to some index buffer topology. */
RTCORE_API void rtcSetIndexBuffer(RTCGeometry geometry, enum RTCBufferType vertexBuffer, enum RTCBufferType indexBuffer);

RTCORE_API void* rtcNewBuffer(RTCGeometry geometry, enum RTCBufferType type, size_t byteStride, unsigned int numItems);

/*! \brief Shares a data buffer between the application and
 *  Embree. The data has to remain valid as long as the mesh exists,
 *  and the user is responsible to free the data when the mesh gets
 *  deleted. For sharing the buffer, one has to specify the number of
 *  elements of the buffer, a byte offset to the first element, and
 *  byte stride of elements stored inside the buffer. The addresses
 *  ptr+offset+i*stride have to be aligned to 4 bytes. For vertex
 *  buffers and user vertex buffers the buffer has to be padded with 0
 *  to a size of a multiple of 16 bytes, as Embree always accesses
 *  vertex buffers and user vertex buffers using SSE instructions. If
 *  this function is not called, Embree will allocate and manage
 *  buffers of the default layout. */
RTCORE_API void rtcSetBuffer(RTCGeometry geometry, enum RTCBufferType type, 
                              const void* ptr, size_t byteOffset, size_t byteStride, unsigned int size);

/*! \brief Gets pointer to specified buffer. */
RTCORE_API void* rtcGetBuffer(RTCGeometry geometry, enum RTCBufferType type);

/*! \brief Enable geometry. Enabled geometry can be hit by a ray. */
RTCORE_API void rtcEnable (RTCGeometry geometry);

/*! \brief Update spefific geometry buffer. 

  Each time geometry buffers got modified, the user has to call some
  update function to tell the ray tracing engine which buffers got
  modified. The rtcUpdateBuffer function taggs a specific buffer of
  some geometry as modified. */
RTCORE_API void rtcUpdateBuffer (RTCGeometry geometry, enum RTCBufferType type);

/*! \brief Disable geometry. 

  Disabled geometry is not hit by any ray. Disabling and enabling
  geometry gives higher performance than deleting and recreating
  geometry. */
RTCORE_API void rtcDisable (RTCGeometry geometry);

/*! \brief Sets the displacement function. */
RTCORE_API void rtcSetDisplacementFunction (RTCGeometry geometry, RTCDisplacementFunction func, struct RTCBounds* bounds);

/*! \brief Sets the intersection filter function for single rays. */
RTCORE_API void rtcSetIntersectionFilterFunction (RTCGeometry geometry, RTCFilterFunctionN func);

/*! \brief Sets the occlusion filter function for single rays. */
RTCORE_API void rtcSetOcclusionFilterFunction (RTCGeometry geometry, RTCFilterFunctionN func);

/*! Set pointer for user defined data per geometry. Invokations
 *  of the various user intersect and occluded functions get passed
 *  this data pointer when called. */
RTCORE_API void rtcSetUserData (RTCGeometry geometry, void* ptr);

/*! Get pointer for user defined data per geometry based on geomID. */
RTCORE_API void* rtcGetUserData (RTCGeometry geometry);

/*! Interpolates user data to some u/v location. The data buffer
 *  specifies per vertex data to interpolate and can be one of the
 *  RTC_VERTEX_BUFFER0/1 or RTC_USER_VERTEX_BUFFER0/1 and has to
 *  contain numFloats floating point values to interpolate for each
 *  vertex of the geometry. The P array will get filled with the
 *  interpolated datam the dPdu and dPdv arrays with the u and v
 *  derivative of the interpolation, and the ddPdudu, ddPdvdv, and
 *  ddPdudv arrays with the respective second derivatives. One can
 *  disable 1) the calculation of the interpolated value by setting P
 *  to NULL, 2) the calculation of the 1st order derivatives by
 *  setting dPdu and dPdv to NULL, 3) the calculation of the second
 *  order derivatives by setting ddPdudu, ddPdvdv, and ddPdudv to
 *  NULL. The buffers have to be padded at the end such that the last
 *  element can be read or written safely using SSE instructions. */
RTCORE_API void rtcInterpolate(RTCGeometry geometry, unsigned int primID, float u, float v, enum RTCBufferType buffer, 
                               float* P, float* dPdu, float* dPdv, float* ddPdudu, float* ddPdvdv, float* ddPdudv, unsigned int numFloats);

/*! Interpolates user data to an array of u/v locations. The valid
 *  pointer points to an integer array that specified which entries in
 *  the u/v arrays are valid (-1 denotes valid, and 0 invalid). If the
 *  valid pointer is NULL all elements are considers valid. The data
 *  buffer specifies per vertex data to interpolate and can be one of
 *  the RTC_VERTEX_BUFFER0/1 or RTC_USER_VERTEX_BUFFER0/1 and has to
 *  contain numFloats floating point values to interpolate for each
 *  vertex of the geometry. The P array will get filled with the
 *  interpolated datam the dPdu and dPdv arrays with the u and v
 *  derivative of the interpolation, and the ddPdudu, ddPdvdv, and
 *  ddPdudv arrays with the respective second derivatives. One can
 *  disable 1) the calculation of the interpolated value by setting P
 *  to NULL, 2) the calculation of the 1st order derivatives by
 *  setting dPdu and dPdv to NULL, 3) the calculation of the second
 *  order derivatives by setting ddPdudu, ddPdvdv, and ddPdudv to
 *  NULL. These destination arrays are filled in structure of array
 *  (SoA) layout. The buffer has to be padded at the end such that
 *  the last element can be read safely using SSE
 *  instructions. */
RTCORE_API void rtcInterpolateN(RTCGeometry geometry, 
                                const void* valid, const unsigned* primIDs, const float* u, const float* v, unsigned int numUVs, 
                                enum RTCBufferType buffer, 
                                float* P, float* dPdu, float* dPdv, float* ddPdudu, float* ddPdvdv, float* ddPdudv, unsigned int numFloats);

/*! Commits the geometry. */
RTCORE_API void rtcCommitGeometry (RTCGeometry geom);

/*! \brief Attaches the geometry to some scene. */
RTCORE_API unsigned int rtcAttachGeometry (RTCScene scene, RTCGeometry geometry);

/*! \brief Attaches the geometry to some scene and releases geometry handle afterwards. */
RTCORE_API unsigned int rtcAttachAndReleaseGeometry (RTCScene scene, RTCGeometry geometry);

/*! \brief Attaches the geometry to some scene using the specified geometry ID. */
RTCORE_API unsigned int rtcAttachGeometryByID (RTCScene scene, RTCGeometry geometry, unsigned int geomID);

/*! \brief Attaches the geometry to some scene using the specified geometry ID and releases the geometry handle afterwards. */
RTCORE_API unsigned int rtcAttachAndReleaseGeometryByID (RTCScene scene, RTCGeometry geometry, unsigned int geomID);

/*! \brief Detaches the geometry from the scene. */
RTCORE_API void rtcDetachGeometry (RTCScene scene, unsigned int geomID);

/*! Releases the geometry handle. */
RTCORE_API void rtcReleaseGeometry (RTCGeometry geometry);

/*! Returns RTCGeometry from scene and geomID. */
RTCORE_API RTCGeometry rtcGetGeometry (RTCScene scene, unsigned int geomID);

#if defined(__cplusplus)
}
#endif
  
/*! @} */