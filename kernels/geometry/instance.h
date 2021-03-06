// ======================================================================== //
// Copyright 2009-2018 Intel Corporation                                    //
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

#include "primitive.h"
#include "../common/scene_instance.h"

namespace embree
{
  struct InstancePrimitive
  {
    struct Type : public PrimitiveType 
    {
      Type ();
      size_t size(const char* This) const;
    };
    static Type type;

  public:

    /* primitive supports multiple time segments */
    static const bool singleTimeSegment = false;

    /* Returns maximum number of stored primitives */
    static __forceinline size_t max_size() { return 1; }

    /* Returns required number of primitive blocks for N primitives */
    static __forceinline size_t blocks(size_t N) { return N; }

  public:

    InstancePrimitive (const Instance* instance) 
    : instance(instance) {}

    __forceinline void fill(const PrimRef* prims, size_t& i, size_t end, Scene* scene)
    {
      assert(end-i == 1);
      const PrimRef& prim = prims[i]; i++;
      const unsigned int geomID = prim.geomID();
      const Instance* instance = scene->get<Instance>(geomID);
      new (this) InstancePrimitive(instance);
    }

    __forceinline LBBox3fa fillMB(const PrimRef* prims, size_t& i, size_t end, Scene* scene, size_t itime)
    {
      assert(end-i == 1);
      const PrimRef& prim = prims[i]; i++;
      const unsigned int geomID = prim.geomID();
      const Instance* instance = scene->get<Instance>(geomID);
      new (this) InstancePrimitive(instance);
      return instance->linearBounds(0,itime);
    }

    __forceinline LBBox3fa fillMB(const PrimRefMB* prims, size_t& i, size_t end, Scene* scene, const BBox1f time_range)
    {
      assert(end-i == 1);
      const PrimRefMB& prim = prims[i]; i++;
      const unsigned int geomID = prim.geomID();
      const Instance* instance = scene->get<Instance>(geomID);
      new (this) InstancePrimitive(instance);
      return instance->linearBounds(0,time_range);
    }

  public:
    const Instance* instance;
  };
}
