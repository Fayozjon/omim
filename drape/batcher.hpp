#pragma once

#include "drape/pointers.hpp"
#include "drape/glstate.hpp"
#include "drape/render_bucket.hpp"
#include "drape/attribute_provider.hpp"
#include "drape/overlay_handle.hpp"

#include "std/map.hpp"
#include "std/function.hpp"

namespace dp
{

class RenderBucket;
class AttributeProvider;
class OverlayHandle;

class Batcher
{
public:
  Batcher(uint32_t indexBufferSize = 9000, uint32_t vertexBufferSize = 10000);
  ~Batcher();

  void InsertTriangleList(GLState const & state, RefPointer<AttributeProvider> params);
  void InsertTriangleList(GLState const & state, RefPointer<AttributeProvider> params,
                          TransferPointer<OverlayHandle> handle);

  void InsertTriangleStrip(GLState const & state, RefPointer<AttributeProvider> params);
  void InsertTriangleStrip(GLState const & state, RefPointer<AttributeProvider> params,
                           TransferPointer<OverlayHandle> handle);

  void InsertTriangleFan(GLState const & state, RefPointer<AttributeProvider> params);
  void InsertTriangleFan(GLState const & state, RefPointer<AttributeProvider> params,
                         TransferPointer<OverlayHandle> handle);

  void InsertListOfStrip(GLState const & state, RefPointer<AttributeProvider> params, uint8_t vertexStride);
  void InsertListOfStrip(GLState const & state, RefPointer<AttributeProvider> params,
                         TransferPointer<OverlayHandle> handle, uint8_t vertexStride);


  typedef function<void (GLState const &, TransferPointer<RenderBucket> )> flush_fn;
  void StartSession(flush_fn const & flusher);
  void EndSession();

private:

  template<typename TBacher>
  void InsertTriangles(GLState const & state,
                       RefPointer<AttributeProvider> params,
                       TransferPointer<OverlayHandle> handle,
                       uint8_t vertexStride = 0);

  class CallbacksWrapper;
  void ChangeBuffer(RefPointer<CallbacksWrapper> wrapper, bool checkFilledBuffer);
  RefPointer<RenderBucket> GetBucket(GLState const & state);

  void FinalizeBucket(GLState const & state);
  void Flush();

private:
  flush_fn m_flushInterface;

private:
  typedef map<GLState, MasterPointer<RenderBucket> > buckets_t;
  buckets_t m_buckets;

  uint32_t m_indexBufferSize;
  uint32_t m_vertexBufferSize;
};

class BatcherFactory
{
public:
  Batcher * GetNew() const { return new Batcher(); }
};

} // namespace dp
