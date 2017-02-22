//*****************************************************************************
// Copyright 2016 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
//*****************************************************************************

// Code written by Cave (www.cavevfx.com) for Autodesk in 2015
// Written by Erdem Taylan

#pragma once

#include <array>
#include <math.h>
#include <vector>
#include <chrono>

#include <bifrostapi/bifrost_channel.h>
#include <bifrostapi/bifrost_voxelchannel.h>
#include <bifrostapi/bifrost_pointchannel.h>
#include <bifrostapi/bifrost_visitor.h>
#include <bifrostapi/bifrost_haloedcache.h>
#include <bifrostapi/bifrost_tileaccessor.h>
#include <bifrostapi/bifrost_layout.h>
#include <bifrostapi/bifrost_types.h>

#include "bifrostrender_decl.h"
#include "bifrostrender_math.h"
#include "bifrostrender_types.h"
#include "bifrostrender_tools.h"
#include "bifrostrender_primvars.h"
#include "bifrostrender_meshing.h"
#include "bifrostrender_tbb.h"

namespace Bifrost{
namespace RenderCore{

struct BIFROSTRENDERAPI_DECL SmoothVoxelsVisitor : public Bifrost::API::Visitor
{
	// haloed cache to access the neigbour values at each voxel tile boundary
	Bifrost::API::HaloedCache<float> _haloCache;  

	// input voxels
	const Bifrost::API::VoxelChannel& _channel;
	const Bifrost::API::VoxelChannel& _channelCopy;
	SmoothFilterType _mode;

	// filter constants
	int _tileWidth;
	float _invDx2; 
	float _dt; 

	int _haloWidth;

	SmoothVoxelsVisitor (	const Bifrost::API::VoxelChannel& channel, const Bifrost::API::VoxelChannel& channelCopy, int haloWidth, SmoothFilterType mode );

	virtual Bifrost::API::Visitor* copy( ) const;

	bool meanCurvature(const std::vector<float> &stencil, float& alpha, float& beta) const;
	float meanCurvatureNormGrad(const std::vector<float> &stencil);
	void smoothMeanCurvatureValue(const Bifrost::API::HaloedCache<float>::Tile& in, Bifrost::API::TileData<float>& out);
	void smoothLaplacianFlowValue(const Bifrost::API::HaloedCache<float>::Tile& in, Bifrost::API::TileData<float>& out);
	void smoothMeanValue(const Bifrost::API::HaloedCache<float>::Tile& in, Bifrost::API::TileData<float>& out);
	void smoothMeanValueDumpHalo(const array3f &in, Bifrost::API::TileData<float>& out);
	void smoothMedianValueDumpHalo(const array3f &in, Bifrost::API::TileData<float>& out);
	void smoothMedianValue(const Bifrost::API::HaloedCache<float>::Tile& in, Bifrost::API::TileData<float>& out);
	void makeHaloedArray( int in_haloWidth, Bifrost::API::TreeIndex in_index, array3f &out );

	virtual void beginTile( const Bifrost::API::TileAccessor& accessor, const Bifrost::API::TreeIndex& index );
};

class BIFROSTRENDERAPI_DECL OffsetVoxelsVisitor : public Bifrost::API::Visitor
{
public:
	Bifrost::API::Channel _channel;
	float _offset;

    OffsetVoxelsVisitor() {}

	OffsetVoxelsVisitor( const Bifrost::API::Channel& channel, float offset );

    ~OffsetVoxelsVisitor() {}

	void beginTile( const Bifrost::API::TileAccessor& accessor, const Bifrost::API::TreeIndex& index );

	virtual Bifrost::API::Visitor* copy() const;
};

class BIFROSTRENDERAPI_DECL DisplaceVoxelsVisitor : public Bifrost::API::Visitor
{
public:
	Bifrost::API::VoxelChannel c_distanceChannel;
	Bifrost::API::VoxelChannel c_airDistanceChannel;
	Bifrost::API::VoxelChannel c_infiniteBlendingChannel;
	std::vector<primVarInfo> *c_primVars;
	BBoxData c_bboxSim;
	InfCubeParams c_infCube;
	Bifrost::API::TileAccessor c_acc;

	Bifrost::API::Layout c_layout;
	float c_voxelScale;
	amino::Math::vec3f c_voxelOffsetValue;

	DisplaceVoxelsVisitor() {}

	DisplaceVoxelsVisitor(
		const Bifrost::API::Channel& distanceChannel,
		const Bifrost::API::Channel& airDistanceChannel,
		const Bifrost::API::Channel& infiniteBlendingChannel,
		std::vector<primVarInfo> *primVars,
		BBoxData bboxSim,
		InfCubeParams infCube
	);

	~DisplaceVoxelsVisitor() {}

	void beginTile( const Bifrost::API::TileAccessor& accessor, const Bifrost::API::TreeIndex& index );

	virtual Bifrost::API::Visitor* copy() const;
};

class BIFROSTRENDERAPI_DECL ExtendVoxelsVisitor : public Bifrost::API::Visitor
{
public:
	Bifrost::API::VoxelChannel c_distanceChannel;
	Bifrost::API::VoxelChannel c_airDistanceChannel;
	Bifrost::API::VoxelChannel c_infiniteBlendingChannel;
	std::vector<primVarInfo> *c_primVars;
	BBoxData c_bboxSim;
	InfCubeParams c_infCube;
	Bifrost::API::TileAccessor c_acc;

	Bifrost::API::Layout c_layout;
	float c_voxelScale;
	amino::Math::vec3f c_voxelOffsetValue;

	ExtendVoxelsVisitor() {}

	ExtendVoxelsVisitor(
		const Bifrost::API::Channel& distanceChannel,
		const Bifrost::API::Channel& airDistanceChannel,
		const Bifrost::API::Channel& infiniteBlendingChannel,
		std::vector<primVarInfo> *primVars,
		BBoxData bboxSim,
		InfCubeParams infCube
	);

	~ExtendVoxelsVisitor() {}

	void beginTile( const Bifrost::API::TileAccessor& accessor, const Bifrost::API::TreeIndex& index );

	virtual Bifrost::API::Visitor* copy() const;
};

// Sharpen or smooth voxels
struct BIFROSTRENDERAPI_DECL MRSmoothVoxelsVisitor : public Bifrost::API::Visitor
{
	const Bifrost::API::VoxelChannel&	m_src;
	Bifrost::API::VoxelChannel&			m_dst;
	Bifrost::API::TileAccessor			m_acc;
	Bifrost::API::HaloedCache<float>	m_srcHaloCache;
	float								m_coeff[4];

	MRSmoothVoxelsVisitor(
		float								in_strength,
		const Bifrost::API::VoxelChannel&	in_src,
		Bifrost::API::VoxelChannel&			in_dst
	);

	MRSmoothVoxelsVisitor(const MRSmoothVoxelsVisitor& other);

	void beginTile(const Bifrost::API::TileAccessor& accessor, const Bifrost::API::TreeIndex& index);

	Bifrost::API::Visitor* copy() const;
};

// Copy channel to another
struct BIFROSTRENDERAPI_DECL CopyChannelVisitor : public Bifrost::API::Visitor
{
	const Bifrost::API::VoxelChannel&	m_src;
	Bifrost::API::VoxelChannel&			m_dst;

	CopyChannelVisitor(
		const Bifrost::API::VoxelChannel&	in_src,
		Bifrost::API::VoxelChannel&			in_dst
	);

	CopyChannelVisitor(const CopyChannelVisitor& other);

	void beginTile(const Bifrost::API::TileAccessor& accessor, const Bifrost::API::TreeIndex& index);

	Bifrost::API::Visitor* copy() const;
};

// Copy channel to another
struct BIFROSTRENDERAPI_DECL SeperateVelocityVisitor : public Bifrost::API::Visitor
{
	const Bifrost::API::VoxelChannel&	m_src;
	Bifrost::API::VoxelChannel&			m_dst_u;
	Bifrost::API::VoxelChannel&			m_dst_v;
	Bifrost::API::VoxelChannel&			m_dst_w;

	SeperateVelocityVisitor(
		const Bifrost::API::VoxelChannel&	in_src,
		Bifrost::API::VoxelChannel&			in_dst_u,
		Bifrost::API::VoxelChannel&			in_dst_v,
		Bifrost::API::VoxelChannel&			in_dst_w
	);

	SeperateVelocityVisitor(const SeperateVelocityVisitor& other);

	void beginTile(const Bifrost::API::TileAccessor& accessor, const Bifrost::API::TreeIndex& index);

	Bifrost::API::Visitor* copy() const;
};

// Copy channel to another
struct BIFROSTRENDERAPI_DECL SeparateUVVisitor : public Bifrost::API::Visitor
{
	const Bifrost::API::VoxelChannel&	c_src;
	Bifrost::API::VoxelChannel&			c_dst_u;
	Bifrost::API::VoxelChannel&			c_dst_v;

	SeparateUVVisitor(
		const Bifrost::API::VoxelChannel&	in_src,
		Bifrost::API::VoxelChannel&			in_dst_u,
		Bifrost::API::VoxelChannel&			in_dst_v
	);

	SeparateUVVisitor(const SeparateUVVisitor& other);

	void beginTile(const Bifrost::API::TileAccessor&, const Bifrost::API::TreeIndex& index);

	Bifrost::API::Visitor* copy() const;
};

// Calculate magnitude of velocity visitor
struct BIFROSTRENDERAPI_DECL SeparateParticleUVVisitor : public Bifrost::API::Visitor
{
	const Bifrost::API::PointChannel&	c_src;
	Bifrost::API::PointChannel&			c_dst_u;
	Bifrost::API::PointChannel&			c_dst_v;

	SeparateParticleUVVisitor(
		const Bifrost::API::PointChannel&	in_src,
		Bifrost::API::PointChannel&			in_dst_u,
		Bifrost::API::PointChannel&			in_dst_v
	);

	SeparateParticleUVVisitor(const SeparateParticleUVVisitor& other);

	void beginTile(const Bifrost::API::TileAccessor&, const Bifrost::API::TreeIndex& index);

	Bifrost::API::Visitor* copy() const;
};


class BIFROSTRENDERAPI_DECL RemapVoxelsVisitor : public Bifrost::API::Visitor
{
public:
	Bifrost::API::VoxelChannel c_chan;
	float c_min;
	float c_max;
	float c_range;
	bool c_invert;

	RemapVoxelsVisitor() {}

	RemapVoxelsVisitor(
		const Bifrost::API::VoxelChannel& channel,
		float min,
		float max,
		bool invert
	);

	~RemapVoxelsVisitor() {}

	void beginTile( const Bifrost::API::TileAccessor& accessor, const Bifrost::API::TreeIndex& index );

	virtual Bifrost::API::Visitor* copy() const;
};

class BIFROSTRENDERAPI_DECL ClipVoxelsVisitor : public Bifrost::API::Visitor
{
public:
	Bifrost::API::Channel inChannel;
	float clipMinX;
	float clipMaxX;
	float clipMinY;
	float clipMaxY;
	float clipMinZ;
	float clipMaxZ;
	float dx;

	ClipVoxelsVisitor() {}

	ClipVoxelsVisitor(
		const Bifrost::API::Channel& channel,
		float clipMinX,
		float clipMaxX,
		float clipMinY,
		float clipMaxY,
		float clipMinZ,
		float clipMaxZ,
		float dx
	);

	~ClipVoxelsVisitor() {}

	void beginTile( const Bifrost::API::TileAccessor& accessor, const Bifrost::API::TreeIndex& index );

	virtual Bifrost::API::Visitor* copy() const;
};

// Copy channel to another
struct BIFROSTRENDERAPI_DECL BlendVoxelsVisitor : public Bifrost::API::Visitor
{
	float								c_weight;
	const Bifrost::API::VoxelChannel&	c_src;
	Bifrost::API::VoxelChannel&			c_blender;
	Bifrost::API::VoxelChannel&			c_dst;

	BlendVoxelsVisitor(
		float								in_weight,
		const Bifrost::API::VoxelChannel&	in_src,
		Bifrost::API::VoxelChannel&			in_blender,
		Bifrost::API::VoxelChannel&			in_dst
	);

	BlendVoxelsVisitor(const BlendVoxelsVisitor& other);

	void beginTile(const Bifrost::API::TileAccessor& accessor, const Bifrost::API::TreeIndex& index);

	Bifrost::API::Visitor* copy() const;
};

// Calculate magnitude of velocity visitor
struct BIFROSTRENDERAPI_DECL CalcVelocityMagnitudeVisitor : public Bifrost::API::Visitor
{
	const Bifrost::API::VoxelChannel&	m_velu;
	const Bifrost::API::VoxelChannel&	m_velv;
	const Bifrost::API::VoxelChannel&	m_velw;
	const Bifrost::API::VoxelChannel&	m_dst;
	float								m_minVal;
	float								m_maxVal;

	CalcVelocityMagnitudeVisitor(
		const Bifrost::API::VoxelChannel&	in_u,
		const Bifrost::API::VoxelChannel&	in_v,
		const Bifrost::API::VoxelChannel&	in_w,
		const Bifrost::API::VoxelChannel&	in_dst
	);

	CalcVelocityMagnitudeVisitor(const CalcVelocityMagnitudeVisitor& other);

	void beginTile(const Bifrost::API::TileAccessor& accessor, const Bifrost::API::TreeIndex& index);

	Bifrost::API::Visitor* copy() const;

	void join(const Bifrost::API::Visitor& visitor);
};

// Calculate magnitude of velocity visitor for point caches
struct BIFROSTRENDERAPI_DECL CalcPointVelocityMagnitudeVisitor : public Bifrost::API::Visitor
{
	const Bifrost::API::PointChannel&	m_vel;
	const Bifrost::API::PointChannel&	m_dst;
	float								m_minVal;
	float								m_maxVal;

	CalcPointVelocityMagnitudeVisitor(
		const Bifrost::API::PointChannel&	in_vel,
		const Bifrost::API::PointChannel&	in_dst
	);

	CalcPointVelocityMagnitudeVisitor(const CalcPointVelocityMagnitudeVisitor& other);

	void beginTile(const Bifrost::API::TileAccessor& accessor, const Bifrost::API::TreeIndex& index);

	Bifrost::API::Visitor* copy() const;

	void join(const Bifrost::API::Visitor& visitor);
};


// Calculate magnitude of velocity visitor
struct BIFROSTRENDERAPI_DECL CalcSpeedVisitor : public Bifrost::API::Visitor
{
	const Bifrost::API::PointChannel&	m_vel;
	const Bifrost::API::PointChannel&	m_dst;
	float								m_minVal;
	float								m_maxVal;

	CalcSpeedVisitor(
		const Bifrost::API::PointChannel&	in,
		const Bifrost::API::PointChannel&	in_dst
	);

	CalcSpeedVisitor(const CalcSpeedVisitor& other);

	void beginTile(const Bifrost::API::TileAccessor& accessor, const Bifrost::API::TreeIndex& index);

	Bifrost::API::Visitor* copy() const;

	void join(const Bifrost::API::Visitor& visitor);
};


// Compute the union of bounding boxes of voxel tiles
struct BIFROSTRENDERAPI_DECL TileBoundsVisitor : Bifrost::API::Visitor
{
	amino::Math::vec3i c_tileBoundsMin;
	amino::Math::vec3i c_tileBoundsMax;

	TileBoundsVisitor();

	~TileBoundsVisitor() {}

	Bifrost::API::Visitor* copy() const;

	void beginTile(const Bifrost::API::TileAccessor& accessor, const Bifrost::API::TreeIndex& index);

	void join(const Bifrost::API::Visitor& visitor);
};

// Compute the bounding box of tiles that has good distance data
struct BIFROSTRENDERAPI_DECL TileBoundsVisitor2 : Bifrost::API::Visitor
{
    const Bifrost::API::VoxelChannel&	c_srcChannel;
    amino::Math::vec3i					c_tileBoundsMin;
    amino::Math::vec3i					c_tileBoundsMax;
    Bifrost::API::TileAccessor			c_acc;

    TileBoundsVisitor2( Bifrost::API::VoxelChannel&	srcChannel );

    TileBoundsVisitor2(const TileBoundsVisitor2& other);

    ~TileBoundsVisitor2() {}

    Bifrost::API::Visitor* copy() const;

    void beginTile( const Bifrost::API::TileAccessor&, const Bifrost::API::TreeIndex& index );

    void join(const Bifrost::API::Visitor& visitor);
};

class BIFROSTRENDERAPI_DECL FrustumCullVisitor : public Bifrost::API::Visitor
{
public:
	FrameData *c_frameData;
	Bifrost::API::Channel c_chan;
	Bifrost::API::TileAccessor c_acc;
	Bifrost::API::TreeIndexArray c_cullIndexes;

	FrustumCullVisitor(
		Bifrost::API::Channel chan,
		FrameData *frameData
	);

	~FrustumCullVisitor() {}

	FrustumCullVisitor(const FrustumCullVisitor& other);

	void beginTile( const Bifrost::API::TileAccessor& accessor, const Bifrost::API::TreeIndex& index );

	virtual Bifrost::API::Visitor* copy() const;

	void join(const Bifrost::API::Visitor& visitor);
};

struct BIFROSTRENDERAPI_DECL ChannelRangeVisitor : public Bifrost::API::Visitor
{
	const Bifrost::API::VoxelChannel&	m_src;
	const Bifrost::API::VoxelChannel&	m_minMax;
	amino::Math::vec3f					m_minCorner;
	amino::Math::vec3f					m_maxCorner;
	Bifrost::API::TileAccessor			m_acc;
	float								m_minVal;
	float								m_maxVal;

	ChannelRangeVisitor(
		const Bifrost::API::VoxelChannel&	src,
		const Bifrost::API::VoxelChannel&	minMax,
		amino::Math::vec3f					minCorner,
		amino::Math::vec3f					maxCorner
	);

	ChannelRangeVisitor(const ChannelRangeVisitor& other);

	void beginTile(const Bifrost::API::TileAccessor& accessor, const Bifrost::API::TreeIndex& index);

	Bifrost::API::Visitor* copy() const;

	void join(const Bifrost::API::Visitor& visitor);
};

struct BIFROSTRENDERAPI_DECL ChannelValueRangeVisitor : public Bifrost::API::Visitor
{
	const Bifrost::API::Channel	m_src;
	float						m_minVal;
	float						m_maxVal;

	ChannelValueRangeVisitor( const Bifrost::API::Channel src );

	ChannelValueRangeVisitor(const ChannelValueRangeVisitor& other);

	void beginTile(const Bifrost::API::TileAccessor& accessor, const Bifrost::API::TreeIndex& index);

	Bifrost::API::Visitor* copy() const;

	void join(const Bifrost::API::Visitor& visitor);
};

// Calculate min max of channel
struct BIFROSTRENDERAPI_DECL CalcTileMinMaxVisitor : public Bifrost::API::Visitor
{
	const Bifrost::API::VoxelChannel&	m_src;
	const Bifrost::API::VoxelChannel&	m_minMax;

	CalcTileMinMaxVisitor( const Bifrost::API::VoxelChannel& src, const Bifrost::API::VoxelChannel&	minmax );

	CalcTileMinMaxVisitor(const CalcTileMinMaxVisitor& other);

	void beginTile(const Bifrost::API::TileAccessor& accessor, const Bifrost::API::TreeIndex& index);

	Bifrost::API::Visitor* copy() const;
};

// Copy channel to another
struct BIFROSTRENDERAPI_DECL SDFToFogVisitor : public Bifrost::API::Visitor
{
	const Bifrost::API::VoxelChannel&	c_src;
	Bifrost::API::VoxelChannel&			c_airDistance;
	Bifrost::API::TileAccessor			c_acc;

	SDFToFogVisitor( const Bifrost::API::VoxelChannel& in_src, Bifrost::API::VoxelChannel& in_airDistance );

	SDFToFogVisitor(const SDFToFogVisitor& other);

	void beginTile(const Bifrost::API::TileAccessor& accessor, const Bifrost::API::TreeIndex& index);

	Bifrost::API::Visitor* copy() const;
};

struct BIFROSTRENDERAPI_DECL SmoothBackSideVisitor : public Bifrost::API::Visitor
{
	Bifrost::API::HaloedCache<float> c_haloCache;
	Bifrost::API::VoxelChannel c_channel;

	SmoothBackSideVisitor( const Bifrost::API::VoxelChannel& channel );

	SmoothBackSideVisitor( const SmoothBackSideVisitor& other );

	virtual Visitor* copy( ) const;

	virtual void beginTile( const Bifrost::API::TileAccessor&, const Bifrost::API::TreeIndex& index );
};


struct BIFROSTRENDERAPI_DECL UInt64ValueRangeVisitor : public Bifrost::API::Visitor
{
	const Bifrost::API::PointChannel	m_src;
	uint64_t							m_minVal;
	uint64_t							m_maxVal;

	UInt64ValueRangeVisitor( const Bifrost::API::PointChannel src );

	UInt64ValueRangeVisitor(const UInt64ValueRangeVisitor& other);

	void beginTile(const Bifrost::API::TileAccessor& accessor, const Bifrost::API::TreeIndex& index);

	Bifrost::API::Visitor* copy() const;

	void join(const Bifrost::API::Visitor& visitor);
};

struct BIFROSTRENDERAPI_DECL CalcNofParticlesVisitor : public Bifrost::API::Visitor
{
	const Bifrost::API::PointChannel	m_src;
	int									m_skip;
	CvInt								m_nofParticles;

	CalcNofParticlesVisitor( const Bifrost::API::PointChannel src, int skip );

	CalcNofParticlesVisitor(const CalcNofParticlesVisitor& other);

	void beginTile(const Bifrost::API::TileAccessor& accessor, const Bifrost::API::TreeIndex& index);

	Bifrost::API::Visitor* copy() const;

	void join(const Bifrost::API::Visitor& visitor);
};

// Compute the bounds of adaptive Aero.
// Both adaptive Aero and Liquid can have valid data in coarse leaf tiles.
// Adaptive Aero is different from Adaptive Liquid because it can have coarse leaf
// tiles anywhere. Adaptive Liquid just have interior coarse leaf tiles.
struct BIFROSTRENDERAPI_DECL AeroVoxelBoundsVisitor : Bifrost::API::Visitor
{
	const Bifrost::API::TreeIndex::Depth	m_maxDepth;
	const Bifrost::API::VoxelChannel		m_voxelChannel;

	amino::Math::vec3i	m_tileBoundsMin;
	amino::Math::vec3i	m_tileBoundsMax;

	AeroVoxelBoundsVisitor( const Bifrost::API::TreeIndex::Depth maxDepth, const Bifrost::API::VoxelChannel voxelChannel );
	AeroVoxelBoundsVisitor(const AeroVoxelBoundsVisitor& other);
    ~AeroVoxelBoundsVisitor() {}
	Bifrost::API::Visitor* copy() const;

	void beginTile(const Bifrost::API::TileAccessor& accessor, const Bifrost::API::TreeIndex& index);
	void join(const Bifrost::API::Visitor& visitor);
};

// marching cube every voxel
struct BIFROSTRENDERAPI_DECL MarchingCubesVisitor : public Bifrost::API::Visitor
{
	FrameData					*c_frameData;
	Bifrost::API::TileAccessor	c_acc;
	MarchingCubes				*c_mesher;
	float						c_dx;

	MarchingCubesVisitor( FrameData *frameData );

	MarchingCubesVisitor(const MarchingCubesVisitor& other);

	void beginTile(const Bifrost::API::TileAccessor& accessor, const Bifrost::API::TreeIndex& index);

	void endTraverse( const Bifrost::API::TileAccessor& accessor );

	Bifrost::API::Visitor* copy() const;

	void join(const Bifrost::API::Visitor& visitor);
};

}}
