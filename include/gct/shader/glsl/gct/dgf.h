// DGF decompressor was originally implemented by Advanced Micro Devices as a part of DGF-SDK using HLSL.
// https://github.com/GPUOpen-LibrariesAndSDKs/DGF-SDK/
// This is a GLSL port of DGF decompressor ported by Naomasa Matsubayashi. 

#ifndef GCT_SHADER_DGF_H
#define GCT_SHADER_DGF_H

#define GCT_ENABLE_8BIT_16BIT_STORAGE
#include <gct/scene_graph/byte_address_buffer.h>

#define DGF_CTRL_RESTART   0
#define DGF_CTRL_EDGE1     1
#define DGF_CTRL_EDGE2     2
#define DGF_CTRL_BACKTRACK 3
#define DGF_MAX_TRIS       64
#define DGF_HEADER_SIZE    20

struct DGFHeader {
    uvec3 bitsPerComponent;
    uint numTriangles;
    uint numVerts;
    uint bitsPerIndex;
    ivec3 anchor;
    float scale;
    uint primIDBase;
    uint userData;
    uint bitSize;
    uint geomIDMeta; 
    bool haveGeomIDPalette;
};

struct DGFBlockInfo {
  DGFHeader header;
  uint dgfBuffer;
  uint blockStartOffset;
  uint bitsPerVertex;
  uint vertexBitStart;
  uint geomIDBitStart;
  uint indexBitStart;
};

DGFHeader DGFLoadHeader( uint dgfBuffer, uint offset ) {
  DGFHeader result;
  const uvec4 H = vertex_buffer_load4( dgfBuffer, offset );
  const uint h0 = H.x;
  const uint h1 = H.y;
  const uint h2 = H.z;
  const uint h3 = H.w;
  const uvec2 H2 = vertex_buffer_load2( dgfBuffer, offset + 16 );
  const uint h4 = H2.x;
  const uint h5 = H2.y;
  result.numTriangles = ((h0 >> 16) & 0x3f) + 1;
  result.numVerts = ((h0 >> 10) & 0x3f) + 1;
  result.bitsPerIndex = ((h0 >> 8) & 3) + 3;
  result.bitsPerComponent.x = (h2 & 0xf) + 1;
  result.bitsPerComponent.y = ((h2 >> 4) & 0xf) + 1;
  result.bitsPerComponent.z = (h3 & 0xf) + 1;
  result.anchor.x = int(h1) >> 8;
  result.anchor.y = int(h2) >> 8;
  result.anchor.z = int(h3) >> 8;
  result.scale = float((h1 & 0xff) << 23);
  result.primIDBase = h4 & ((1 << 29) - 1);
  
  uint haveUserData = ((h4 >> 29) & 1);
  result.userData = haveUserData * h5;
  result.bitSize = 32 * (5 + haveUserData);
  
  result.haveGeomIDPalette = (h3 & 0x80) != 0;
  result.geomIDMeta        = h0 >> 22;
  return result;
}

uint ByteAlign( uint numBits ) {
  return (numBits + 7) & ~7;
}

uint ComputeGeomIDPaletteSize( DGFHeader header ) {
  uint geomIDMeta     = header.geomIDMeta;
  uint numGeomIDs     = (geomIDMeta >> 5) + 1;
  uint prefixBitSize  = (geomIDMeta & 0x1F);
  uint payloadBitSize = 25 - prefixBitSize;
  uint indexBitSize   = findMSB(numGeomIDs - 1) + 1;
  uint paletteSize    = ByteAlign(numGeomIDs * payloadBitSize + header.numTriangles * indexBitSize + prefixBitSize);
  return (header.haveGeomIDPalette) ? paletteSize : 0;
}

DGFBlockInfo DGFLoadBlockInfo(uint dgfBuffer, in uint dgfBlockIndex) {
  DGFBlockInfo result;
  result.blockStartOffset = dgfBlockIndex * 128;
  result.dgfBuffer = dgfBuffer;
  result.header = DGFLoadHeader(result.dgfBuffer, result.blockStartOffset);
  result.bitsPerVertex  = result.header.bitsPerComponent.x + result.header.bitsPerComponent.y + result.header.bitsPerComponent.z;

  uint vertexBitSize     = ByteAlign(result.header.numVerts * result.bitsPerVertex);
  uint geomIDPaletteSize = ComputeGeomIDPaletteSize(result.header);

  result.vertexBitStart  = result.header.bitSize;
  result.geomIDBitStart  = result.vertexBitStart + vertexBitSize;
  result.indexBitStart   = result.geomIDBitStart + geomIDPaletteSize;

  return result;
}

uint Align(uint dw0, uint dw1, uint misalign) {
  uint64_t pack = dw1;
  pack = (pack << 32) | dw0;
  return uint(pack >> (misalign & 31));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////   Index fetch        
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint LoadIndex(uint dgfBuffer, uint blockStartOffset, uint bitPos, uint bitsPerIndex) {
  // indices are up to 6b, potentially spread across 2 dwords
  uint dwordPos = bitPos / 32;
  uvec2 dw = vertex_buffer_load2(dgfBuffer, blockStartOffset + 4 * dwordPos);
  uint M = uint( (1ul << bitsPerIndex) - 1ul );
  return Align(dw.x, dw.y, bitPos) & M;
}

struct IsFirstScanState {
  uvec3 prefixCount;
  bvec3 isFirst;
};

IsFirstScanState InitIsFirstScanState(const uvec3 ibAddress) {
  IsFirstScanState state;
  state.prefixCount.x = min(ibAddress.x, 3); // first 3 'isFirst' bits are implicit ones
  state.prefixCount.y = min(ibAddress.y, 3); // first 3 'isFirst' bits are implicit ones
  state.prefixCount.z = min(ibAddress.z, 3); // first 3 'isFirst' bits are implicit ones
  state.isFirst.x = ibAddress.x < 3;
  state.isFirst.y = ibAddress.y < 3;
  state.isFirst.z = ibAddress.z < 3;
  return state;
}

void IsFirstScan(inout IsFirstScanState state, uint indexStart, const uvec3 ibAddress, uint bits) {
  uint bitsX = (ibAddress.x < indexStart) ? 0 : bits; // don't count bits beyond the query position
  uint bitsY = (ibAddress.y < indexStart) ? 0 : bits;
  uint bitsZ = (ibAddress.z < indexStart) ? 0 : bits;

  uint offsetX = (ibAddress.x - indexStart);
  uint offsetY = (ibAddress.y - indexStart);
  uint offsetZ = (ibAddress.z - indexStart);
  uint maskX = (offsetX < 32) ? uint(1ul << offsetX) : 0;
  uint maskY = (offsetY < 32) ? uint(1ul << offsetY) : 0;
  uint maskZ = (offsetZ < 32) ? uint(1ul << offsetZ) : 0;

  // count number of 'first' indices that precede this IB position
  uint popCountX = bitCount(bitsX & (maskX - 1));
  uint popCountY = bitCount(bitsY & (maskY - 1));
  uint popCountZ = bitCount(bitsZ & (maskZ - 1));
  state.prefixCount.x += popCountX;
  state.prefixCount.y += popCountY;
  state.prefixCount.z += popCountZ;

  // when we pass the query position, note whether or not it is an 'isFirst' index
  state.isFirst.x = bool(bitsX & maskX) || state.isFirst.x;
  state.isFirst.y = bool(bitsY & maskY) || state.isFirst.y;
  state.isFirst.z = bool(bitsZ & maskZ) || state.isFirst.z;
}

uvec3 DemuxIndices(uvec3 indices, DGFBlockInfo s) {
  // maximum 'isFirst' size is 189b, potentially spread across 6 dwords
  //  This is an array of 1bit values ordered back to front
  uint numControlBits = 2 * (s.header.numTriangles - 1);
  uint isFirstStart = numControlBits / 32;
  uint ifBase = s.blockStartOffset + 4 * (31 - isFirstStart - 5);
  uvec4 IF0 = vertex_buffer_load4(s.dgfBuffer, ifBase);
  uvec2 IF1 = vertex_buffer_load2(s.dgfBuffer, ifBase + 16);
  uint f0 = bitfieldReverse(IF1.y);
  uint f1 = bitfieldReverse(IF1.x);
  uint f2 = bitfieldReverse(IF0.w);
  uint f3 = bitfieldReverse(IF0.z);
  uint f4 = bitfieldReverse(IF0.y);
  uint f5 = bitfieldReverse(IF0.x);
 
  // align the "is-first" bits
  uint if0 = Align(f0, f1, numControlBits);
  uint if1 = Align(f1, f2, numControlBits);
  uint if2 = Align(f2, f3, numControlBits);
  uint if3 = Align(f3, f4, numControlBits);
  uint if4 = Align(f4, f5, numControlBits);

  // scan the is-first bits to find the compressed index buffer lookup positions
  IsFirstScanState ifScan = InitIsFirstScanState(indices);
  IsFirstScan(ifScan, 3, indices, if0);
  IsFirstScan(ifScan, 35, indices, if1);
  IsFirstScan(ifScan, 67, indices, if2);
  IsFirstScan(ifScan, 99, indices, if3);
  IsFirstScan(ifScan, 131, indices, if4);
  
  // load any "non-first" indices
  uvec3 localIndices;
  uint ibBase  = s.indexBitStart;
  uint bitPosX = ibBase + (indices.x - ifScan.prefixCount.x) * s.header.bitsPerIndex;
  uint bitPosY = ibBase + (indices.y - ifScan.prefixCount.y) * s.header.bitsPerIndex;
  uint bitPosZ = ibBase + (indices.z - ifScan.prefixCount.z) * s.header.bitsPerIndex;
  localIndices.x = LoadIndex(s.dgfBuffer, s.blockStartOffset, bitPosX, s.header.bitsPerIndex);
  localIndices.y = LoadIndex(s.dgfBuffer, s.blockStartOffset, bitPosY, s.header.bitsPerIndex);
  localIndices.z = LoadIndex(s.dgfBuffer, s.blockStartOffset, bitPosZ, s.header.bitsPerIndex);
  localIndices.x = ifScan.isFirst.x ? ifScan.prefixCount.x : localIndices.x;
  localIndices.y = ifScan.isFirst.y ? ifScan.prefixCount.y : localIndices.y;
  localIndices.z = ifScan.isFirst.z ? ifScan.prefixCount.z : localIndices.z;
  return localIndices;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////    Connectivity decode
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint Compact(uint x) {
  x &= 0x55555555u;
  x = (x ^ (x >> 1ul)) & 0x33333333u;
  x = (x ^ (x >> 2ul)) & 0x0f0f0f0fu;
  x = (x ^ (x >> 4ul)) & 0x00ff00ffu;
  x = (x ^ (x >> 8ul)) & 0x0000ffffu;
  return x;
}

#define bits_t uint64_t

uint64_t Pack64(uint a, uint b, uint c, uint d) {
    uint64_t q0 = (a | (b << 16ul));
    uint64_t q1 = (c | (d << 16ul));
    return (q1 << 32ul) | q0;
}

void ExtractBits(DGFBlockInfo s, out uint64_t evenBits, out uint64_t oddBits) {
  uvec4 Ctrl = vertex_buffer_load4( s.dgfBuffer, s.blockStartOffset + 28u * 4u);
  uint ctrl0 = bitfieldReverse(Ctrl.w);
  uint ctrl1 = bitfieldReverse(Ctrl.z);
  uint ctrl2 = bitfieldReverse(Ctrl.y);
  uint ctrl3 = bitfieldReverse(Ctrl.x);
  evenBits = Pack64(Compact(ctrl0), Compact(ctrl1), Compact(ctrl2), Compact(ctrl3));
  oddBits = Pack64(Compact(ctrl0 >> 1), Compact(ctrl1 >> 1), Compact(ctrl2 >> 1), Compact(ctrl3 >> 1));
  evenBits <<= 1;
  oddBits <<= 1;
}

uint LoadTriangleControlValues(DGFBlockInfo s, uint triangleId) {
  if (triangleId == 0) {
    return DGF_CTRL_RESTART;
  }

  // The first triangle control code is never stored, so we we need to " - 1" the triangleId to account for that.
  const uint storedTriangleId = triangleId - 1;
  const uint ctrlDWord = vertex_buffer_load(
    s.dgfBuffer,
    // start-offset of each block:
    s.blockStartOffset +
    // the control bytes start here within each block:
    28 * 4 +
    // From the storedTriangleId, we compute the dword index of the code
    // (" / 16 ", because there are 16 codes per dword),
    // reverse it (that is the "3 - "),
    // and compute the byte offset (* 4)
    (3 - storedTriangleId / 16) * 4
  );
  // Within the ctrlDWord, we compute the index position of the triangle control value:
  // 2 *  : because there are 2 bits code
  // 15 - : because the codes are stored in reverse
  // storedTriangleId & 15: that is storedTriangleId % 16 which gives us triangleId within the DWord.
  const uint bitIdx = 2 * (15 - (storedTriangleId & 15));

  // >> bitIdx: Shift the bits of the control values into the lower two bits.
  // & 3: leave only the lower two bits.
  return (ctrlDWord >> bitIdx) & 3u;
}

bits_t SelectMask(const uint i) {
  return (bits_t(-1)) >> (~i);
}

bits_t ComputeVirtualCodeChanges(bits_t isRestart, bits_t isEdge2, bits_t isBacktrack) {
  const bits_t previousIsRestart = isRestart << 1u;
  const bits_t thisIsCopy = isBacktrack >> 1;
  const bits_t nextIsEdge2 = isEdge2 >> 1u;
  const bits_t c = (thisIsCopy & nextIsEdge2) | ((~thisIsCopy) & isEdge2);
  const bits_t p = c << 1;
  
  return previousIsRestart | (p ^ c);
}

ivec3 InitAccu(bool prevIsCopy, bool currentIsRestart) {
  return ivec3(-1 + ( currentIsRestart ? 1: 0 ), -1 + 2 * ( currentIsRestart ? 1: 0 ), 2 * ( currentIsRestart ? 1: 0 ) + ( prevIsCopy ? 1: 0 ) );
}

uint ComputeNRestarts(uint triIdx, bits_t isRestart) {
  const bits_t m = (isRestart << 1) & SelectMask(triIdx);

  uint bitCount = bitCount(uint(m)) + bitCount(uint(m>>32));
  return bitCount;
}

ivec3 ComputeDSum(uint triIdx, bool prevIsCopy, bits_t isRestart) {
  const int nRestarts = int( ComputeNRestarts(triIdx, isRestart) );
  return ivec3(nRestarts, 2 * nRestarts, 2 * nRestarts + triIdx - ( prevIsCopy ? 1 : 0 ) );
}

int firstbithigh64(uint64_t v) {
  int high = findMSB(uint(v >> 32));
  if (high != -1) {
    return high + 32;
  }

  return findMSB(uint(v));
}

int FindFirstCodeChange(bits_t isCodeChange, uint triIdx) {
  const bits_t x = isCodeChange & SelectMask(triIdx);
  return firstbithigh64(x);
}

ivec2 ComputeRSum(uint triIdx, bits_t isBacktrack, bits_t isRestart) {
  triIdx--;
  const int nRestarts = int( ComputeNRestarts(triIdx, isRestart) );
  const bool prevIsCopy = bool( uint(isBacktrack >> triIdx) & 1u );
  return ivec2(nRestarts + int( triIdx ) - ( prevIsCopy ? 1 : 0 ), int( triIdx ) - ( prevIsCopy ? 1 : 0 ) );
}

uvec3 ControlScan(uint triIdx, const bits_t isRestart, const bits_t isEdge2, const bits_t isBacktrack) {
  const bits_t isEdge2Ear = (isBacktrack & (~(isEdge2 << 1u))) | ((~isBacktrack) & isEdge2);
  const bits_t isCodeChange = ComputeVirtualCodeChanges(isRestart, isEdge2Ear, isBacktrack);
  
  const bool copy = bool( uint(isBacktrack >> (triIdx + 1)) & 1u );
  const uint i = triIdx - ( copy ? 1 : 0 );
	
  const bool restart = bool( uint(isRestart >> i) & 1u );
  const bool edge2 = bool( uint(isEdge2Ear >> i) & 1 );
  const bool prevIsCopy = bool( uint(isBacktrack >> i) & 1u );
  const bool nextIsEdge2 = bool( uint(isEdge2Ear >> (triIdx + 1)) & 1u );
	
  ivec3 result = InitAccu(prevIsCopy, restart);
  const ivec3 dSum = ComputeDSum(i, prevIsCopy, isRestart);
  const ivec2 rSum = ComputeRSum(FindFirstCodeChange(isCodeChange, i), isBacktrack, isRestart);

  result.x += (restart || !edge2) ? dSum.z : rSum.x + dSum.x;
  result.y += (restart || edge2) ? dSum.z : rSum.y + dSum.y;
  result.z += dSum.z;

  result.x = (copy && nextIsEdge2) ? result.z : result.x;
  result.y = (copy && !nextIsEdge2) ? result.z : result.y;
  result.z += ( copy ? 1 : 0 );

  return result;
}

DGFBlockInfo DGFInit(uint dgfBuffer, in uint dgfBlockIndex) {
    return DGFLoadBlockInfo(dgfBuffer, dgfBlockIndex);
}

struct TriangleBits {
  bits_t isRestart;
  bits_t isBacktrack;
  bits_t isEdge2;
};

uint64_t Wave64ActiveBallot(bool expr) {
  uvec4 b = subgroupBallot(expr);
  uint64_t r = b.y;
  r <<= 32;
  r |= b.x;
  return r;
}

TriangleBits GetTriangleBitsWaveBallot(DGFBlockInfo s) {
  TriangleBits result;
  result.isRestart = 0;
  result.isBacktrack = 0;
  result.isEdge2 = 0;

  const uint WAVE_SIZE = gl_SubgroupSize;
  for (uint waveBase = 0; waveBase < DGF_MAX_TRIS; waveBase += WAVE_SIZE) {
    uint i = waveBase + gl_SubgroupID;
    const uint ctrl = LoadTriangleControlValues(s, i);

    // DGF blocks have a limit of 64 triangles.
    //  On a Wave128 architecture we only need the first 64
    result.isRestart |= (Wave64ActiveBallot(ctrl == DGF_CTRL_RESTART) << waveBase);
    result.isBacktrack |= (Wave64ActiveBallot(ctrl == DGF_CTRL_BACKTRACK) << waveBase);
    result.isEdge2 |= (Wave64ActiveBallot(ctrl == DGF_CTRL_EDGE2) << waveBase);
  }

  const bits_t mask = (bits_t(1) << s.header.numTriangles) - bits_t(1);
  result.isBacktrack &= mask;
  return result;
}

// Decode a triangle using a single lane
TriangleBits GetTriangleBitsSingleLane(DGFBlockInfo s) {
  TriangleBits result;
  bits_t evenBits, oddBits;
  ExtractBits(s, evenBits, oddBits);
  const bits_t mask = (bits_t(1) << s.header.numTriangles) - bits_t(1);
  result.isRestart = ((~evenBits) & (~oddBits));
  result.isBacktrack = ((evenBits) & (oddBits)) & mask;
  result.isEdge2 = ((evenBits) & (~oddBits));
  return result;
}

// Decode a triangle using the full wave.
//  Must be called within wave-uniform control flow with a wave-uniform DGF block
uvec3 DGFGetTriangle_BitScan_Wave(DGFBlockInfo s, uint triangleIndexInBlock) {
  TriangleBits triangleBits = GetTriangleBitsWaveBallot(s);
  uvec3 indices = ControlScan(triangleIndexInBlock, triangleBits.isRestart, triangleBits.isEdge2, triangleBits.isBacktrack);
  return DemuxIndices(indices, s);
}

// Decode a triangle using a single lane
uvec3 DGFGetTriangle_BitScan_Lane(DGFBlockInfo s, uint triangleIndexInBlock) {
  TriangleBits triangleBits = GetTriangleBitsSingleLane(s);
  uvec3 indices = ControlScan(triangleIndexInBlock, triangleBits.isRestart, triangleBits.isEdge2, triangleBits.isBacktrack);
  return DemuxIndices(indices, s);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////    Vertex Fetch
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

vec3 DGFGetVertex(DGFBlockInfo s, uint vertexIndex) {
  // vertices are up to 48b, potentially spanning 3 dwords       
  uint bitPos = s.vertexBitStart + vertexIndex * s.bitsPerVertex;
  uint dwordPos = bitPos / 32;
  uvec3 f = vertex_buffer_load3(s.dgfBuffer, s.blockStartOffset + 4 * dwordPos);
  uint dw0 = Align(f.x, f.y, bitPos); // align the fetch
  uint dw1 = Align(f.y, f.z, bitPos);
  
  // x and y are guaranteed to be in the lower dword.  Z may be straddling the boundary
  uint64_t vert = (uint64_t(dw1) << 32) | dw0;
  ivec3 v = ivec3(dw0, dw0 >> s.header.bitsPerComponent.x, uint(vert >> (s.header.bitsPerComponent.x + s.header.bitsPerComponent.y)));
  
  const ivec3 mask = (ivec3(1, 1, 1) << s.header.bitsPerComponent) - ivec3(1, 1, 1);
  v &= mask;
  v += s.header.anchor;
  return vec3(v * s.header.scale);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////    Serial Connectivity Decode ( for reference )
//////////////      This algorithm is much slower than the bit-scan algorithm and should probably not be used
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct ControlScanState {
  uvec3 indices; // index buffer positions for last decoded triangle
  uvec3 prev; // index buffer positions for the triangle before last
  uint lastCtrl; // control bit value of the last decoded triangle
  uint indexPos; // position in the index buffer
  uint restartCount;
};

ControlScanState InitControlScanState() {
  // first triangle is an implicit restart.. indices are 0,1,2
  ControlScanState state;
  state.indices = uvec3(0, 1, 2);
  state.indexPos = 3;
  state.lastCtrl = 0;
  state.prev = uvec3(0, 0, 0);
  state.restartCount = 1;
  return state;
}

void ControlScan(inout ControlScanState s, uint triStart, uint triIndex, uint bits) {
  uint i = 30; // control bit pairs are stored back to front
  for (uint tri = triStart; tri <= min(triIndex, triStart + 15); tri++) {
    uint ctrl = (bits >> i) & 3;
    i -= 2;
    uvec3 prevPrev = s.prev;
    uvec3 prev = s.indices;
    
    bool restart = (ctrl == DGF_CTRL_RESTART);
    bool backtrack = (ctrl == DGF_CTRL_BACKTRACK);
    uint px = (backtrack) ? prevPrev.x : prev.x; // choose base triangle based on backtrack 
    uint py = (backtrack) ? prevPrev.y : prev.y;
    uint pz = (backtrack) ? prevPrev.z : prev.z;
    uint c = (backtrack) ? s.lastCtrl ^ 3 : ctrl; // if backtracking, use inverted ctrl values from last tri
    bool edge1 = c == DGF_CTRL_EDGE1;

    // select edge1 or edge2 from base triangle
    uint x = (edge1) ? pz : px;
    uint y = (edge1) ? py : pz;
    uint z = s.indexPos;

    // handle restart
    s.indices.x = (restart) ? s.indexPos : x;
    s.indices.y = (restart) ? s.indexPos + 1 : y;
    s.indices.z = (restart) ? s.indexPos + 2 : z;
    s.indexPos += (restart) ? 3 : 1;

    s.lastCtrl = ctrl;
    s.prev = prev;
  }
}

uvec3 DGFGetTriangle_Serial(inout DGFBlockInfo s, uint triIndex) {
  ControlScanState cscan = InitControlScanState();

  // at most four control-bit dwords
  // these are ordered back to front, but they contain 2-bit fields whose order isn't reversed
  uvec4 Ctrl = vertex_buffer_load4(s.dgfBuffer, s.blockStartOffset + 28 * 4);
  uint ctrl0 = Ctrl.w;
  uint ctrl1 = Ctrl.z;
  uint ctrl2 = Ctrl.y;
  uint ctrl3 = Ctrl.x;

  // scan the control bits to find the index buffer addresses for this triangle
  ControlScan(cscan, 1, triIndex, ctrl0);
  ControlScan(cscan, 17, triIndex, ctrl1);
  ControlScan(cscan, 33, triIndex, ctrl2);
  ControlScan(cscan, 49, triIndex, ctrl3);

  return DemuxIndices(cscan.indices, s);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////    GeomID Fetch
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Reads geometry ID and opacity from a DGF block.  X contains geomID.  Y contains opaque flag
uvec2 DGFGetGeomIDAndOpacity(DGFBlockInfo s, uint triIndex) {    
  uint geomID = s.header.geomIDMeta; // if there is no palette, the meta field *is* the ID
  if (s.header.haveGeomIDPalette) {
    // read a per-triangle ID from the palette
    
    // parse palette parameters
    uint geomIDMeta = geomID;
    uint numGeomIDs     = (geomIDMeta >> 5) + 1;
    uint prefixBitSize  = (geomIDMeta & 0x1F);
    uint payloadBitSize = 25 - prefixBitSize;
    uint indexBitSize   = findMSB(numGeomIDs - 1) + 1;
    
    uint prefixBase   = s.geomIDBitStart;
    uint indicesBase  = prefixBase + prefixBitSize;
    uint payloadsBase = indicesBase + s.header.numTriangles * indexBitSize;
    
    // load the prefix value.  This is guaranteed to be byte aligned but may not be dword aligned
    uint prefix = LoadIndex(s.dgfBuffer, s.blockStartOffset, prefixBase, prefixBitSize);
    
    // load the index.  This is up to 5b, potentially spread across two dwords
    uint indexBitPos = indicesBase + indexBitSize * triIndex;
    uint geomIDIndex = LoadIndex(s.dgfBuffer, s.blockStartOffset, indexBitPos, indexBitSize);

    // load the payload.  Payloads are up to 25b, potentially spread across two dwords
    uint payloadBitPos = payloadsBase + geomIDIndex * payloadBitSize;
    uint geomIDPayload = LoadIndex(s.dgfBuffer, s.blockStartOffset, payloadBitPos, payloadBitSize);
    
    // reconstruct the geomID
    geomID = (prefix << payloadBitSize) + geomIDPayload;
  }

  // opaque flag is in the LSB
  return uvec2(geomID >> 1, geomID & 1);
}

// Given a primitive ID, returns its DGF block index (x) and its position in the block (y)
uvec2 DGFLookupBlockMap(uint dgfBlockMap, uint primID) {
  // The block map buffer contains one dword for every 32 triangles.  The bits are laid out thusly:
  //
  //  X:
  //    uint32_t blockIndex      : 24;  // Index of block containing first triangle
  //    uint32_t positionInBlock : 6;   // position of the first triangle in the block
  //    uint32_t unused          : 2;   
  //
  //  Y:
  //    uint32_t endOfBlock;    // For each triangle in the group, is it the last one in its block
  //
  //  This allows us to infer the location of any of the 32 triangles, at a memory overhead of 0.25 B/Tri.
  //
  
  uint blockMapIndex = primID / 32;
  uvec2 mapVal = vertex_buffer_load2(dgfBlockMap, 8 * blockMapIndex);
  uint blockIndex = mapVal.x & 0x00FFFFFF;
  uint positionInBlock = (mapVal.x >> 24);
  uint eobMask = mapVal.y;
  
  uint blockMapMod = primID % 32;
  uint prefixMask = (1u << blockMapMod) - 1; // mask out eob bits for this triangle and above
  blockIndex += bitCount(eobMask & prefixMask); // count number of eob tris prior to this one.  Step forward this much
  
  int boundary = findMSB(eobMask & prefixMask); // find the position of the most recent block transition    
  positionInBlock = (boundary == -1) ? positionInBlock : 0; // reset position in block if there was a block transition
  positionInBlock += blockMapMod - boundary - 1; // figure out how far we are from start of our current block
  
  return uvec2(blockIndex, positionInBlock);
}

#endif

