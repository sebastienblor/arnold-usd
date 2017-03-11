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

#include <bifrostrendercore/bifrostrender_visitors.h>

namespace Bifrost{
namespace RenderCore{

//
// SmoothVoxels Visitor Functions
//

SmoothVoxelsVisitor::SmoothVoxelsVisitor (	const Bifrost::API::VoxelChannel& channel,
						const Bifrost::API::VoxelChannel& channelCopy,
						int haloWidth,
						SmoothFilterType mode
					)	:	_channel(channel), 
							_channelCopy(channelCopy), 
							_mode(mode),
							_haloWidth(haloWidth)
{
	const float dx = Bifrost::API::Layout(_channel.layout()).voxelScale();     

	const float inv2Dx = 0.5f / dx;
	_invDx2 = 4.0f * inv2Dx * inv2Dx;

	if ( mode == kCurvatureFlow ) {
		_dt = dx * dx / 3.0f;
	} else {
		_dt = dx * dx / 6.0f;
	}

	_tileWidth = Bifrost::API::Layout(_channel.layout()).tileDimInfo().tileWidth;

	if( (mode== kCurvatureFlow || mode== kLaplacianFlow) && _haloWidth > 1 ) {
		_haloWidth = 1;
	}

	if(_haloWidth == 1) {
		// initialize the cache with the input voxels
		_haloCache.init( _channel );
	}
}

Bifrost::API::Visitor* SmoothVoxelsVisitor::copy( ) const {
	return new SmoothVoxelsVisitor(*this);
}

bool SmoothVoxelsVisitor::meanCurvature(const std::vector<float> &stencil, float& alpha, float& beta) const
{
	// For performance all finite differences are unscaled wrt dx
	const float
		Half(0.5), Quarter(0.25),
		Dx  = Half * (stencil[2] - stencil[1]), Dx2 = Dx * Dx, // * 1/dx
		Dy  = Half * (stencil[4] - stencil[3]), Dy2 = Dy * Dy, // * 1/dx
		Dz  = Half * (stencil[6] - stencil[5]), Dz2 = Dz * Dz, // * 1/dx
		normGrad = Dx2 + Dy2 + Dz2;

	if (normGrad <= 0.0f) {
		alpha = beta = 0;
		return false;
	}

	const float
		Dxx = stencil[2] - 2 * stencil[0] + stencil[1], // * 1/dx2
		Dyy = stencil[4] - 2 * stencil[0] + stencil[3], // * 1/dx2
		Dzz = stencil[6] - 2 * stencil[0] + stencil[5], // * 1/dx2
		Dxy = Quarter * (stencil[10] - stencil[ 8] + stencil[7] - stencil[ 9]), // * 1/dx2
		Dxz = Quarter * (stencil[14] - stencil[12] + stencil[11] - stencil[13]), // * 1/dx2
		Dyz = Quarter * (stencil[18] - stencil[16] + stencil[15] - stencil[17]); // * 1/dx2
	alpha = (Dx2*(Dyy+Dzz)+Dy2*(Dxx+Dzz)+Dz2*(Dxx+Dyy)-2*(Dx*(Dy*Dxy+Dz*Dxz)+Dy*Dz*Dyz));
	beta  = std::sqrt(normGrad); // * 1/dx

	return true;
}

float SmoothVoxelsVisitor::meanCurvatureNormGrad(const std::vector<float> &stencil)
{
	float alpha, beta;
	return this->meanCurvature(stencil, alpha, beta) ? alpha*_invDx2/(2*beta*beta) : 0;
}

void SmoothVoxelsVisitor::smoothMeanCurvatureValue(const Bifrost::API::HaloedCache<float>::Tile& in, Bifrost::API::TileData<float>& out)
{
	for (int k = 0; k < _tileWidth; ++k) {
		for (int j = 0; j < _tileWidth; ++j) {
			for (int i = 0; i < _tileWidth; ++i) {
				std::vector<float> tmp;
				tmp.resize(19);
				tmp[ 0] = in( i+0, j+0, k+0);

				tmp[ 1] = in( i-1, j+0, k+0);
				tmp[ 2] = in( i+1, j+0, k+0);

				tmp[ 3] = in( i+0, j-1, k+0);
				tmp[ 4] = in( i+0, j+1, k+0);

				tmp[ 5] = in( i+0, j+0, k-1);
				tmp[ 6] = in( i+0, j+0, k+1);

				tmp[ 7] = in( i-1, j-1, k+0);
				tmp[ 8] = in( i+1, j-1, k+0);
				tmp[ 9] = in( i-1, j+1, k+0);
				tmp[10] = in( i+1, j+1, k+0);

				tmp[11] = in( i-1, j+0, k-1);
				tmp[12] = in( i+1, j+0, k-1);
				tmp[13] = in( i-1, j+0, k+1);
				tmp[14] = in( i+1, j+0, k+1);
			
				tmp[15] = in( i+0, j-1, k-1);
				tmp[16] = in( i+0, j+1, k-1);
				tmp[17] = in( i+0, j-1, k+1);
				tmp[18] = in( i+0, j+1, k+1);

				out(i, j, k) = in(i, j, k) +_dt* meanCurvatureNormGrad(tmp); 
			}
		}
	}
}

void SmoothVoxelsVisitor::smoothLaplacianFlowValue(const Bifrost::API::HaloedCache<float>::Tile& in, Bifrost::API::TileData<float>& out)
{
	for (int k = 0; k < _tileWidth; ++k) {
		for (int j = 0; j < _tileWidth; ++j) {
			for (int i = 0; i < _tileWidth; ++i) {
				out(i,j,k) = in(i, j, k) + _dt * _invDx2 * (-6 * in(i, j, k) + in(i - 1, j, k) + in(i + 1, j, k) + in(i, j - 1, k) + in(i, j + 1, k) + in(i, j, k - 1) + in(i, j, k + 1));
			}
		}
	}
}

void SmoothVoxelsVisitor::smoothMeanValue(const Bifrost::API::HaloedCache<float>::Tile& in, Bifrost::API::TileData<float>& out)
{
	Bifrost::API::HaloedCache<float>::Tile tmp, tmp2;

	float scale = 1.0f/27.0f;

	for (int k = -1; k < this->_tileWidth + 1; ++k) {
		for (int j = -1; j < this->_tileWidth + 1; ++j) {
			for (int i = 0; i <this->_tileWidth; ++i) {
				tmp(i, j, k) = in(i - 1, j, k) + in(i + 1, j, k) + in(i, j, k);
			}
		}
	}

	for (int k = -1; k < this->_tileWidth + 1; ++k) {
		for (int j = 0; j <this->_tileWidth; ++j) {
			for (int i = 0; i<this->_tileWidth; ++i) {
				tmp2(i, j, k) = tmp(i, j - 1, k) + tmp(i, j + 1, k) + tmp(i, j, k);
			}
		}
	}

	for (int k = 0; k < this->_tileWidth; ++k) {
		for (int j = 0; j < this->_tileWidth; ++j) {
			for (int i = 0; i < this->_tileWidth; ++i) {
				out(i, j, k) = (tmp2(i, j, k - 1) + tmp2(i, j, k + 1) + tmp2(i, j, k))*scale;
			}
		}
	}
}

void SmoothVoxelsVisitor::smoothMeanValueDumpHalo(const array3f &in, Bifrost::API::TileData<float>& out)
{
	array3f tmp(_tileWidth + 2 * _haloWidth, _tileWidth + 2 * _haloWidth, _tileWidth + 2 * _haloWidth, 0.f), tmp2(_tileWidth + 2 * _haloWidth, _tileWidth + 2 * _haloWidth, _tileWidth + 2 * _haloWidth, 0.f);

	float scale = 1.0f / ( ( 1.0f + 2.0f * _haloWidth ) * ( 1.0f + 2.0f * _haloWidth ) * ( 1.0f + 2.0f *_haloWidth ) );

	for (int k=0; k < this->_tileWidth + 2 * _haloWidth; ++k) {
		for (int j=0; j < this->_tileWidth + 2 * _haloWidth; ++j) {
			for (int i = _haloWidth; i < this->_tileWidth+ _haloWidth; ++i) {
				tmp(i,j,k) = in(i,j,k);
				for( int ii = 1; ii < 1 + _haloWidth; ii++)
					tmp(i,j,k) += in(i - ii, j, k) + in(i + ii, j, k); 
			}
		}
	}

	for (int k = 0; k < this->_tileWidth + 2 * _haloWidth; ++k) {
		for (int j = _haloWidth; j < this->_tileWidth + _haloWidth; ++j) {
			for (int i = 0; i < this->_tileWidth + _haloWidth; ++i) {
				tmp2(i,j,k) = tmp(i,j,k);
				for( int jj = 1; jj < 1 + _haloWidth; jj++)
					tmp2(i, j, k) += tmp(i, j - jj, k) + tmp(i, j + jj, k) ;
			}
		}
	}

	for (int k = _haloWidth; k < this->_tileWidth + _haloWidth; ++k) {
		for (int j = _haloWidth; j < this->_tileWidth + _haloWidth; ++j) {
			for (int i = _haloWidth; i < this->_tileWidth+ _haloWidth; ++i) {
				out(i - _haloWidth, j - _haloWidth, k - _haloWidth) = tmp2(i, j, k);
				for( int kk = 1; kk < 1 + _haloWidth; kk++)
					out( i - _haloWidth, j - _haloWidth, k - _haloWidth) += tmp2(i, j, k - kk) + tmp2(i, j, k + kk) ;
				out(i - _haloWidth, j - _haloWidth, k - _haloWidth) *= scale;
			}
		}
	}
}

void SmoothVoxelsVisitor::smoothMedianValueDumpHalo(const array3f &in, Bifrost::API::TileData<float>& out)
{
	for (int k = _haloWidth; k < this->_tileWidth + _haloWidth; ++k) {
		for (int j = _haloWidth; j < this->_tileWidth + _haloWidth; ++j) {
			for (int i = _haloWidth; i < this->_tileWidth + _haloWidth; ++i) {
				std::vector<float> tmp; 

				for( int kk = k - _haloWidth; kk <= k + _haloWidth; kk++) {
					for( int jj = j - _haloWidth; jj <= j + _haloWidth; jj++) {
						for( int ii = i - _haloWidth; ii <= i + _haloWidth; ii++) {
							tmp.push_back( in(ii,jj,kk) );
						}
					}
				}

				size_t midpoint = (tmp.size() - 1) >> 1;
				// Partially sort the vector until the median value is at the midpoint.
				std::nth_element(tmp.begin(), tmp.begin() + midpoint, tmp.end());

				out(i-_haloWidth,j-_haloWidth,k-_haloWidth) = tmp[midpoint];
			}
		}
	}
}

void SmoothVoxelsVisitor::smoothMedianValue(const Bifrost::API::HaloedCache<float>::Tile& in, Bifrost::API::TileData<float>& out)
{
	for (int k=0; k<this->_tileWidth; ++k) {
		for (int j=0; j<this->_tileWidth; ++j) {
			for (int i=0; i<this->_tileWidth; ++i) {
				std::vector<float> tmp; 
				tmp.push_back(in(i-1,j-1,k-1)); tmp.push_back(in(i-1,j-1,k)); tmp.push_back(in(i-1,j-1,k+1));
				tmp.push_back(in(i-1,j,k-1));   tmp.push_back(in(i-1,j,k));   tmp.push_back(in(i-1,j,k+1));
				tmp.push_back(in(i-1,j+1,k-1)); tmp.push_back(in(i-1,j+1,k)); tmp.push_back(in(i-1,j+1,k+1));
				tmp.push_back(in(i,j-1,k-1));   tmp.push_back(in(i,j-1,k));   tmp.push_back(in(i,j-1,k+1));
				tmp.push_back(in(i,j,k-1));     tmp.push_back(in(i,j,k));     tmp.push_back(in(i,j,k+1));
				tmp.push_back(in(i,j+1,k-1));   tmp.push_back(in(i,j+1,k));   tmp.push_back(in(i,j+1,k+1));
				tmp.push_back(in(i+1,j-1,k-1)); tmp.push_back(in(i+1,j-1,k)); tmp.push_back(in(i+1,j-1,k+1));
				tmp.push_back(in(i+1,j,k-1));   tmp.push_back(in(i+1,j,k));   tmp.push_back(in(i+1,j,k+1));
				tmp.push_back(in(i+1,j+1,k-1)); tmp.push_back(in(i+1,j+1,k)); tmp.push_back(in(i+1,j+1,k+1));

				size_t midpoint = (tmp.size() - 1) >> 1;
				// Partially sort the vector until the median value is at the midpoint.
				std::nth_element(tmp.begin(), tmp.begin() + midpoint, tmp.end());

				out(i,j,k) = tmp[midpoint];
			}
		}
	}
}

void SmoothVoxelsVisitor::makeHaloedArray( int in_haloWidth, Bifrost::API::TreeIndex in_index, array3f &out ) {
	assert(in_haloWidth <= _tileWidth);
	assert(out.ni == _tileWidth + 2 * in_haloWidth);
	assert(out.nj == out.ni && out.nk == out.ni);

	// first copy the central tile into the array
	// (we want this first, because it's easy, and let's us extrapolate off the edge of a root)
	Bifrost::API::TileData<float> const central = _channel.tileData<float>( in_index );
	for(int k = 0; k < _tileWidth; ++k) {
		for(int j = 0; j < _tileWidth; ++j) {
			for(int i = 0; i < _tileWidth; ++i) {
				out(i + in_haloWidth, j + in_haloWidth, k + in_haloWidth) = central(i, j, k);
			}
		}
	}

	// look for neighbours
	Bifrost::API::Layout layout = _channel.layout(); 
	Bifrost::API::TileAccessor const &accessor = layout.tileAccessor( ); 
	Bifrost::API::TileInfo tinfo = accessor.tileInfo(in_index);

	int ti = tinfo.i, tj = tinfo.j, tk = tinfo.k;
	int tw = (int) std::pow( _tileWidth, layout.maxDepth() + 1 - tinfo.depth);
	for ( int dk = -1; dk <= 1; ++dk ) {
		int nbrTk = tk + dk * tw;
		int klo, khi, nbrKlo;

		if ( dk < 0 ) {
			klo = 0; khi = in_haloWidth; nbrKlo = _tileWidth - in_haloWidth;
		} else if ( dk == 0 ){
			klo = in_haloWidth; khi = _tileWidth + in_haloWidth; nbrKlo = 0;
		} else {
			klo= _tileWidth + in_haloWidth; khi = _tileWidth + 2 * in_haloWidth; nbrKlo = 0;
		}

		for ( int dj = -1; dj <= 1; ++dj ) {
			int nbrTj = tj + dj * tw;
			int jlo, jhi, nbrJlo;

			if ( dj < 0 ) {
				jlo = 0; jhi = in_haloWidth; nbrJlo = _tileWidth - in_haloWidth;
			} else if ( dj == 0 ) {
				jlo = in_haloWidth; jhi = _tileWidth + in_haloWidth; nbrJlo = 0;
			} else {
				jlo= _tileWidth + in_haloWidth; jhi = _tileWidth + 2 * in_haloWidth; nbrJlo = 0;
			}

			for ( int di = -1; di <= 1; ++di ) {
				if ( di == 0 && dj == 0 && dk == 0) {
					// skip central tile
					continue;
				}

				int nbrTi = ti + di * tw;
				int ilo, ihi, nbrIlo;

				if ( di < 0 ) {
					ilo = 0; ihi = in_haloWidth; nbrIlo = _tileWidth - in_haloWidth;
				} else if ( di == 0 ) {
					ilo = in_haloWidth; ihi = _tileWidth + in_haloWidth; nbrIlo = 0;
				} else {
					ilo = _tileWidth + in_haloWidth; ihi = _tileWidth + 2 * in_haloWidth; nbrIlo = 0;
				}

				// look for neighbouring tile
				Bifrost::API::Tile tile = accessor.tile(nbrTi, nbrTj, nbrTk, tinfo.depth);
				Bifrost::API::TreeIndex tileIndex = tile.index();
				if ( tileIndex.depth == tinfo.depth ) { // same depth?
					Bifrost::API::TileData<float> const &nbr = _channel.tileData<float>( tileIndex); 
					for ( int k = klo, nbrK = nbrKlo; k < khi; ++k, ++nbrK) {
						for(int j = jlo, nbrJ = nbrJlo; j < jhi; ++j, ++nbrJ) {
							for(int i = ilo, nbrI = nbrIlo; i < ihi; ++i, ++nbrI) {
								out(i, j, k) = nbr(nbrI, nbrJ, nbrK);
							}
						}
					}
				} else if ( tileIndex.depth >= 0 ) {
					// valid but coarser tile? get value from it.
					// figure out the cell in coarser tile where the desired neighbour would be
					Bifrost::API::TileInfo ancestorInfo = accessor.tileInfo(tileIndex);

					//VoxelLayoutNode ancestor=in_layout.node(loc);
					int ai = ancestorInfo.i, aj = ancestorInfo.j, ak = ancestorInfo.k;
					int acw = (int) std::pow(_tileWidth, layout.maxDepth() - ancestorInfo.depth);
					int li = ( nbrTi - ai ) / acw, lj = ( nbrTj - aj ) / acw, lk = ( nbrTk - ak ) / acw;

					// and just use a constant value from there to copy over
					Bifrost::API::TileData<float> const &nbr = _channel.tileData<float>(tileIndex);
						
					float constValue = nbr(li,lj,lk);
					for (int k = klo; k < khi; ++k) {
						for(int j = jlo; j < jhi; ++j) {
							for(int i = ilo; i < ihi; ++i) {
								out(i, j, k) = constValue;
							}
						}
					}
				} else {
					// off the edge of the root, extrapolate from central tile
					// we could be much smarter about this, but for now just copy a single value from the original tile
					float constValue = central( _tileWidth/2, _tileWidth/2, _tileWidth/2 );
					for ( int k = klo; k < khi; ++k ) {
						for( int j = jlo; j < jhi; ++j ) {
							for( int i = ilo; i < ihi; ++i ) {
								out(i, j, k) = constValue;
							}
						}
					}
				}
			}
		}
	}
}

void SmoothVoxelsVisitor::beginTile( const Bifrost::API::TileAccessor&, const Bifrost::API::TreeIndex& index ) {
	if ( _haloWidth == 0 )
		return;

	array3f inDump( _tileWidth + 2 * _haloWidth, _tileWidth + 2 * _haloWidth, _tileWidth + 2 * _haloWidth, 0.f);
	const Bifrost::API::HaloedCache<float>::Tile& in = _haloCache.tile( index );
	Bifrost::API::TileData<float> out = _channelCopy.tileData<float>( index );


	if ( _haloWidth==1 ) {
		_haloCache.update( index );
	} else {
		makeHaloedArray(_haloWidth, index, inDump);
	}

	switch (_mode) {
		case kMeanValue:
			if ( _haloWidth == 1 ) {
				smoothMeanValue(in, out);
			} else {
				smoothMeanValueDumpHalo(inDump, out);
			}
			break;
		case kMedianValue:
			if ( _haloWidth == 1) {
				smoothMedianValue(in, out);
			} else {
				smoothMedianValueDumpHalo(inDump, out);
			}
			break;
		case kLaplacianFlow:
			smoothLaplacianFlowValue(in, out);
			break;
		case kCurvatureFlow:
			smoothMeanCurvatureValue(in, out);
			break;
		default:
			smoothMeanValue(in, out);
			break;
	}

}

//
// OffsetVoxels Visitor Functions
//

OffsetVoxelsVisitor::OffsetVoxelsVisitor(
	const Bifrost::API::Channel& channel,
	float offset
) :
	_channel( channel ),
	_offset( offset )
{}

void OffsetVoxelsVisitor::beginTile( const Bifrost::API::TileAccessor&, const Bifrost::API::TreeIndex& index )
{
	Bifrost::API::TileData<float> data = _channel.tileData<float>( index );
	for ( size_t k = 0; k < data.count(); k++ ) {
		data[k] += _offset;
	}
}

Bifrost::API::Visitor* OffsetVoxelsVisitor::copy() const
{
	return new OffsetVoxelsVisitor( _channel, _offset );
}

//
// DisplaceVoxels Visitor Functions
//

DisplaceVoxelsVisitor::DisplaceVoxelsVisitor(
	const Bifrost::API::Channel& distanceChannel,
	const Bifrost::API::Channel& airDistanceChannel,
	const Bifrost::API::Channel& infiniteBlendingChannel,
	std::vector<primVarInfo> *primVars,
	BBoxData bboxSim,
	InfCubeParams infCube
) :
	c_distanceChannel( distanceChannel ),
	c_airDistanceChannel( airDistanceChannel ),
	c_infiniteBlendingChannel( infiniteBlendingChannel ),
	c_primVars( primVars ),
	c_bboxSim( bboxSim ),
	c_infCube( infCube )
{
	// a new accessor for this thread
	c_layout = c_distanceChannel.layout();
	c_acc = c_layout.tileAccessor();

	c_voxelScale = c_layout.voxelScale();
	c_voxelOffsetValue = c_distanceChannel.offsetValue();
}

void DisplaceVoxelsVisitor::beginTile( const Bifrost::API::TileAccessor&, const Bifrost::API::TreeIndex& index )
{
	// Fetch the current tile
	const Bifrost::API::Tile tile = c_acc.tile(index);

	// Get the voxel space coordinate and dimension info
	const Bifrost::API::TileCoord coord = tile.coord();
	const Bifrost::API::TileInfo info = tile.info();

	Bifrost::API::TileData<float> distanceData = c_distanceChannel.tileData<float>( index );
	Bifrost::API::TileData<float> airDistanceData = c_airDistanceChannel.tileData<float>( index );
	Bifrost::API::TileData<float> blendData = c_infiniteBlendingChannel.tileData<float>( index );

	// limit displacement to within the bbox input
	// Determine the voxel space bounding box of the tile
	amino::Math::vec3i tileBoundsMin, tileBoundsMax;
	tileBoundsMin[0] = coord.i;
	tileBoundsMin[1] = coord.j;
	tileBoundsMin[2] = coord.k;
	tileBoundsMax[0] = coord.i + info.dimInfo.depthWidth;
	tileBoundsMax[1] = coord.j + info.dimInfo.depthWidth;
	tileBoundsMax[2] = coord.k + info.dimInfo.depthWidth;

	// if we are inside the bbox do displacement
	bool inside = true;
	if	(
			( tileBoundsMax[0] < c_bboxSim.tile.min[0] || tileBoundsMin[0] > c_bboxSim.tile.max[0] ) ||
			( tileBoundsMax[1] < c_bboxSim.tile.min[1] || tileBoundsMin[1] > c_bboxSim.tile.max[1] ) ||
			( tileBoundsMax[2] < c_bboxSim.tile.min[2] || tileBoundsMin[2] > c_bboxSim.tile.max[2] )
		)
	{
		inside = false;
	}

	if ( inside ) {
		// we are inside the tile bbox so displace the tile if needed
		for ( size_t k = 0; k < distanceData.count(); k++ ) {
			// get voxel indices
			int voxelI = int ( k % 5);
			int voxelJ = ( int ( k / 5) ) % 5;
			int voxelK = int ( k / 25);

			// get voxel offset from tile corner
			amino::Math::vec3f voxelOffset (	voxelI * info.dimInfo.voxelWidth + c_voxelOffsetValue[0],
												voxelJ * info.dimInfo.voxelWidth + c_voxelOffsetValue[1],
												voxelK * info.dimInfo.voxelWidth + c_voxelOffsetValue[2]); 
			// calc voxel world pos
			amino::Math::vec3f voxelPosWorld (	coord.i + voxelOffset[0],
												coord.j + voxelOffset[1],
												coord.k + voxelOffset[2] );
			voxelPosWorld *= c_voxelScale;

			// calc blend in x and z
			// this is done such that, equal distances in world space yield the same value for x and z directions
			float finalBlend = 0.0f;
			if ( c_infiniteBlendingChannel.valid() ) {
				// we will use data provided by the user
				finalBlend = blendData[k];

				float distanceCubeVal = voxelPosWorld[1] - c_infCube.topCenterY;
				float airDistanceCubeVal = std::min<float> ( 0.0f, distanceCubeVal );

				distanceData[k] = mix<float> (distanceData[k], distanceCubeVal, finalBlend);
				airDistanceData[k] = mix<float> (airDistanceData[k], airDistanceCubeVal, finalBlend);

				// calc primvars
				for ( int var = 0; var < (*c_primVars).size(); var++ ) {
					Bifrost::API::TileData<float> channelData = (*c_primVars)[var].channel.tileData<float>( index );
					channelData[k] = mix<float> (channelData[k], (*c_primVars)[var].defVal, finalBlend);
				}
			} else {
				// we will calc a rectangular falloff
				float blendX = fabs ( 2 * ( voxelPosWorld[0] - c_bboxSim.world.center[0] ) );
				float blendZ = fabs ( 2 * ( voxelPosWorld[2] - c_bboxSim.world.center[2] ) );
				// take correctionfactor into account
				if ( c_bboxSim.world.dim[2] > c_bboxSim.world.dim[0] ) {
					// z is greater than x
					blendX /= c_bboxSim.world.dim[0];
					blendZ = std::max(0.0f, blendZ - c_bboxSim.world.dim[2] + c_bboxSim.world.dim[0]);
					blendZ /= c_bboxSim.world.dim[0];
				} else {
					// x is greater than z
					blendX = std::max(0.0f, blendX - c_bboxSim.world.dim[0] + c_bboxSim.world.dim[2]);
					blendX /= c_bboxSim.world.dim[2];
					blendZ /= c_bboxSim.world.dim[2];
				}

				// calc finalblend val
                finalBlend = clip<float> ( std::max( blendX, blendZ ), 0.0f, 1.0f);

				// if we are at a position which is smaller than infcubeBlendStart
				// we don't have to do anything
				// for the rest we calc new distances
				if ( finalBlend >= c_infCube.blendStart ) {
					if ( finalBlend >= c_infCube.blendEnd ) {
						// we are at the infinite surface part
						distanceData[k] = voxelPosWorld[1] - c_infCube.topCenterY;
						airDistanceData[k] = voxelPosWorld[1] - c_infCube.topCenterY;

						for ( int var = 0; var < (*c_primVars).size(); var++ ) {
							Bifrost::API::TileData<float> channelData = (*c_primVars)[var].channel.tileData<float>( index );
							channelData[k] = (*c_primVars)[var].defVal;
						}
					} else {
						// we are in between

						// calc a smooth finalblend value
						finalBlend = calcBlendVal ( c_infCube.blendType, c_infCube.blendStart, c_infCube.blendEnd, finalBlend);

						float distanceCubeVal = voxelPosWorld[1] - c_infCube.topCenterY;
						float airDistanceCubeVal = voxelPosWorld[1] - c_infCube.topCenterY;

						distanceData[k] = mix<float> (distanceData[k], distanceCubeVal, finalBlend);
						airDistanceData[k] = mix<float> (airDistanceData[k], airDistanceCubeVal, finalBlend);

						for ( int var = 0; var < (*c_primVars).size(); var++ ) {
							Bifrost::API::TileData<float> channelData = (*c_primVars)[var].channel.tileData<float>( index );
							channelData[k] = mix<float> (channelData[k], (*c_primVars)[var].defVal, finalBlend);
						}
					}
				} // end blend region check
			} // end blend channel check
		} // end distance data loop
	} // end inside check
}

Bifrost::API::Visitor* DisplaceVoxelsVisitor::copy() const
{
	return new DisplaceVoxelsVisitor( c_distanceChannel, c_airDistanceChannel, c_infiniteBlendingChannel, c_primVars, c_bboxSim, c_infCube );
}


//
// ExtendVoxels Visitor Functions
//

ExtendVoxelsVisitor::ExtendVoxelsVisitor(
	const Bifrost::API::Channel& distanceChannel,
	const Bifrost::API::Channel& airDistanceChannel,
	const Bifrost::API::Channel& infiniteBlendingChannel,
	std::vector<primVarInfo> *primVars,
	BBoxData bboxSim,
	InfCubeParams infCube
) :
	c_distanceChannel( distanceChannel ),
	c_airDistanceChannel( airDistanceChannel ),
	c_infiniteBlendingChannel( infiniteBlendingChannel ),
	c_primVars( primVars ),
	c_bboxSim( bboxSim ),
	c_infCube( infCube )
{
	// a new accessor for this thread
	c_layout = c_distanceChannel.layout();
	c_acc = c_layout.tileAccessor();

	c_voxelScale = c_layout.voxelScale();
	c_voxelOffsetValue = c_distanceChannel.offsetValue();
}

void ExtendVoxelsVisitor::beginTile( const Bifrost::API::TileAccessor&, const Bifrost::API::TreeIndex& index )
{
	// Fetch the current tile
	const Bifrost::API::Tile tile = c_acc.tile(index);

	// Get the voxel space coordinate and dimension info
	const Bifrost::API::TileCoord coord = tile.coord();
	const Bifrost::API::TileInfo info = tile.info();

	Bifrost::API::TileData<float> distanceData = c_distanceChannel.tileData<float>( index );
	Bifrost::API::TileData<float> airDistanceData = c_airDistanceChannel.tileData<float>( index );
	Bifrost::API::TileData<float> blendData = c_infiniteBlendingChannel.tileData<float>( index );

	// limit displacement to within the bbox input
	// Determine the voxel space bounding box of the tile
	amino::Math::vec3i tileBoundsMin, tileBoundsMax;
	tileBoundsMin[0] = coord.i;
	tileBoundsMin[1] = coord.j;
	tileBoundsMin[2] = coord.k;
	tileBoundsMax[0] = coord.i + info.dimInfo.depthWidth;
	tileBoundsMax[1] = coord.j + info.dimInfo.depthWidth;
	tileBoundsMax[2] = coord.k + info.dimInfo.depthWidth;

	// if we are inside the bbox do displacement
	bool inside = true;
	if	(
			( tileBoundsMax[0] < c_bboxSim.tile.min[0] || tileBoundsMin[0] > c_bboxSim.tile.max[0] ) ||
			( tileBoundsMax[1] < c_bboxSim.tile.min[1] || tileBoundsMin[1] > c_bboxSim.tile.max[1] ) ||
			( tileBoundsMax[2] < c_bboxSim.tile.min[2] || tileBoundsMin[2] > c_bboxSim.tile.max[2] )
		)
	{
		inside = false;
	}

	if ( inside ) {
		// we are inside the tile bbox so displace the tile if needed
		for ( size_t k = 0; k < distanceData.count(); k++ ) {
			// get voxel indices
			int voxelI = int ( k % 5);
			int voxelJ = ( int ( k / 5) ) % 5;
			int voxelK = int ( k / 25);

			// get voxel offset from tile corner
			amino::Math::vec3f voxelOffset (	voxelI * info.dimInfo.voxelWidth + c_voxelOffsetValue[0],
												voxelJ * info.dimInfo.voxelWidth + c_voxelOffsetValue[1],
												voxelK * info.dimInfo.voxelWidth + c_voxelOffsetValue[2]); 
			// calc voxel world pos
			amino::Math::vec3f voxelPosWorld (	coord.i + voxelOffset[0],
												coord.j + voxelOffset[1],
												coord.k + voxelOffset[2] );
			voxelPosWorld *= c_voxelScale;

			// calc blend in x and z
			// this is done such that, equal distances in world space yield the same value for x and z directions
			float finalBlend = 0.0f;
			if ( c_infiniteBlendingChannel.valid() ) {
				// we will use data provided by the user
				finalBlend = blendData[k];

				float distanceCubeVal = voxelPosWorld[1] - c_infCube.topCenterY;
				float airDistanceCubeVal = std::min<float> ( 0.0f, distanceCubeVal );

				distanceData[k] = mix<float> (distanceData[k], distanceCubeVal, finalBlend);
				airDistanceData[k] = mix<float> (airDistanceData[k], airDistanceCubeVal, finalBlend);

				// calc primvars
				for ( int var = 0; var < (*c_primVars).size(); var++ ) {
					Bifrost::API::TileData<float> channelData = (*c_primVars)[var].channel.tileData<float>( index );
					channelData[k] = mix<float> (channelData[k], (*c_primVars)[var].defVal, finalBlend);
				}
			} else {
				// we will calc a rectangular falloff
				float blendX = fabs ( 2 * ( voxelPosWorld[0] - c_bboxSim.world.center[0] ) );
				float blendZ = fabs ( 2 * ( voxelPosWorld[2] - c_bboxSim.world.center[2] ) );
				// take correctionfactor into account
				if ( c_bboxSim.world.dim[2] > c_bboxSim.world.dim[0] ) {
					// z is greater than x
					blendX /= c_bboxSim.world.dim[0];
					blendZ = std::max(0.0f, blendZ - c_bboxSim.world.dim[2] + c_bboxSim.world.dim[0]);
					blendZ /= c_bboxSim.world.dim[0];
				} else {
					// x is greater than z
					blendX = std::max(0.0f, blendX - c_bboxSim.world.dim[0] + c_bboxSim.world.dim[2]);
					blendX /= c_bboxSim.world.dim[2];
					blendZ /= c_bboxSim.world.dim[2];
				}

				// calc finalblend val
                finalBlend = clip<float> ( std::max( blendX, blendZ ), 0.0f, 1.0f);

				// if we are at a position which is lower than infcubeBlendStart
				// we don't have to do anything
				// for the rest we calc new distances
				if ( finalBlend >= c_infCube.blendStart ) {
					if ( finalBlend >= c_infCube.blendEnd ) {
						// we are at the infinite surface part
						distanceData[k] = voxelPosWorld[1] - c_infCube.topCenterY;
						airDistanceData[k] = voxelPosWorld[1] - c_infCube.topCenterY;

						for ( int var = 0; var < (*c_primVars).size(); var++ ) {
							Bifrost::API::TileData<float> channelData = (*c_primVars)[var].channel.tileData<float>( index );
							channelData[k] = (*c_primVars)[var].defVal;
						}
					} else {
						// we are in between

						// calc a smooth finalblend value
						finalBlend = calcBlendVal ( c_infCube.blendType, c_infCube.blendStart, c_infCube.blendEnd, finalBlend);

						float distanceCubeVal = voxelPosWorld[1] - c_infCube.topCenterY;
						float airDistanceCubeVal = voxelPosWorld[1] - c_infCube.topCenterY;

						distanceData[k] = mix<float> (distanceData[k], distanceCubeVal, finalBlend);
						airDistanceData[k] = mix<float> (airDistanceData[k], airDistanceCubeVal, finalBlend);

						for ( int var = 0; var < (*c_primVars).size(); var++ ) {
							Bifrost::API::TileData<float> channelData = (*c_primVars)[var].channel.tileData<float>( index );
							channelData[k] = mix<float> (channelData[k], (*c_primVars)[var].defVal, finalBlend);
						}
					}
				} // end blend region check
			} // end blend channel check
		} // end distance data loop
	} // end inside check
}

Bifrost::API::Visitor* ExtendVoxelsVisitor::copy() const
{
	return new ExtendVoxelsVisitor( c_distanceChannel, c_airDistanceChannel, c_infiniteBlendingChannel, c_primVars, c_bboxSim, c_infCube );
}

//
// MRSmoothVoxelsVisitor Visitor Functions
//

MRSmoothVoxelsVisitor::MRSmoothVoxelsVisitor(
	float								in_strength,
	const Bifrost::API::VoxelChannel&	in_src,
	Bifrost::API::VoxelChannel&			in_dst
)
	: m_src(in_src), m_dst(in_dst)
{
	// set up the various filter coefficients based on sharpening strength
	float a = in_strength / 8, b = 1 - in_strength / 4;
	m_coeff[0] = a * a * a;
	m_coeff[1] = a * a * b;
	m_coeff[2] = a * b * b;
	m_coeff[3] = b * b * b;

	// A new accessor for this thread
	Bifrost::API::Layout layout = m_src.layout();
	m_acc = layout.tileAccessor();

	// Initialize the cache with the input voxels
	m_srcHaloCache.init(in_src);
}

MRSmoothVoxelsVisitor::MRSmoothVoxelsVisitor(const MRSmoothVoxelsVisitor& other)
	: m_src(other.m_src), m_dst(other.m_dst)
{
	// Copy the filter coefficients
	m_coeff[0] = other.m_coeff[0];
	m_coeff[1] = other.m_coeff[1];
	m_coeff[2] = other.m_coeff[2];
	m_coeff[3] = other.m_coeff[3];

	// A new accessor for this thread
	Bifrost::API::Layout layout = m_src.layout();
	m_acc = layout.tileAccessor();

	// Copy the haloed cache
	m_srcHaloCache = other.m_srcHaloCache;
}

void MRSmoothVoxelsVisitor::beginTile(const Bifrost::API::TileAccessor&, const Bifrost::API::TreeIndex& index)
{
	// get a haloed version of the source tile
	m_srcHaloCache.update(index);
	const Bifrost::API::HaloedCache<float>::Tile& srcHaloTile = m_srcHaloCache.tile(index);

	// run the filter
	const int kTileWidth = m_acc.tile(index).info().dimInfo.tileWidth;
	Bifrost::API::TileData<float> dstTile = m_dst.tileData<float>(index);
	for(int k = 0; k < kTileWidth; k++) {
		for(int j = 0; j < kTileWidth; j++) {
			for(int i = 0; i < kTileWidth; i++) {
				dstTile(i,j,k) =
					m_coeff[0] * (
							srcHaloTile(i-1,j-1,k-1) + srcHaloTile(i+1,j-1,k-1)
						+ srcHaloTile(i-1,j+1,k-1) + srcHaloTile(i+1,j+1,k-1)
						+ srcHaloTile(i-1,j-1,k+1) + srcHaloTile(i+1,j-1,k+1)
						+ srcHaloTile(i-1,j+1,k+1) + srcHaloTile(i+1,j+1,k+1)
						)
					+ m_coeff[1] * (
							srcHaloTile(i,j-1,k-1) + srcHaloTile(i,j+1,k-1)
						+ srcHaloTile(i,j-1,k+1) + srcHaloTile(i,j+1,k+1)
						+ srcHaloTile(i-1,j,k-1) + srcHaloTile(i+1,j,k-1)
						+ srcHaloTile(i-1,j,k+1) + srcHaloTile(i+1,j,k+1)
						+ srcHaloTile(i-1,j-1,k) + srcHaloTile(i+1,j-1,k)
						+ srcHaloTile(i-1,j+1,k) + srcHaloTile(i+1,j+1,k)
						)
					+ m_coeff[2] * (
							srcHaloTile(i-1,j,k) + srcHaloTile(i+1,j,k)
						+ srcHaloTile(i,j-1,k) + srcHaloTile(i,j+1,k)
						+ srcHaloTile(i,j,k-1) + srcHaloTile(i,j,k+1)
						)
					+ m_coeff[3] * srcHaloTile(i,j,k);
			}
		}
	}
}

Bifrost::API::Visitor* MRSmoothVoxelsVisitor::copy() const
{
	return new MRSmoothVoxelsVisitor(*this);
}

//
// CopyChannelVisitor Visitor Functions
//

CopyChannelVisitor::CopyChannelVisitor(
	const Bifrost::API::VoxelChannel&	in_src,
	Bifrost::API::VoxelChannel&			in_dst
)
	: m_src(in_src), m_dst(in_dst)
{
}

CopyChannelVisitor::CopyChannelVisitor(const CopyChannelVisitor& other)
	: m_src(other.m_src), m_dst(other.m_dst)
{
}

void CopyChannelVisitor::beginTile(const Bifrost::API::TileAccessor&, const Bifrost::API::TreeIndex& index)
{
	Bifrost::API::TileData<float> inData = m_src.tileData<float>( index );
	Bifrost::API::TileData<float> outData = m_dst.tileData<float>( index );
	for ( size_t k = 0; k < inData.count(); k++ ) {
		outData[k] = inData[k];
	}
}

Bifrost::API::Visitor* CopyChannelVisitor::copy() const
{
	return new CopyChannelVisitor(*this);
}


//
// SeperateVelocityVisitor Visitor Functions
//

SeperateVelocityVisitor::SeperateVelocityVisitor(
	const Bifrost::API::VoxelChannel&	in_src,
	Bifrost::API::VoxelChannel&			in_dst_u,
	Bifrost::API::VoxelChannel&			in_dst_v,
	Bifrost::API::VoxelChannel&			in_dst_w
)
	: m_src(in_src), m_dst_u(in_dst_u), m_dst_v(in_dst_v), m_dst_w(in_dst_w)
{
}

SeperateVelocityVisitor::SeperateVelocityVisitor(const SeperateVelocityVisitor& other)
	: m_src(other.m_src), m_dst_u(other.m_dst_u), m_dst_v(other.m_dst_v), m_dst_w(other.m_dst_w)
{
}

void SeperateVelocityVisitor::beginTile(const Bifrost::API::TileAccessor&, const Bifrost::API::TreeIndex& index)
{
	Bifrost::API::TileData<amino::Math::vec3f> inData = m_src.tileData<amino::Math::vec3f>( index );
	Bifrost::API::TileData<float> outDataU = m_dst_u.tileData<float>( index );
	Bifrost::API::TileData<float> outDataV = m_dst_v.tileData<float>( index );
	Bifrost::API::TileData<float> outDataW = m_dst_w.tileData<float>( index );
	for ( size_t k = 0; k < inData.count(); k++ ) {
		outDataU[k] = inData[k][0];
		outDataV[k] = inData[k][1];
		outDataW[k] = inData[k][2];
	}
}

Bifrost::API::Visitor* SeperateVelocityVisitor::copy() const
{
	return new SeperateVelocityVisitor(*this);
}

//
// SeparateUVVisitor Visitor Functions
//

SeparateUVVisitor::SeparateUVVisitor(
	const Bifrost::API::VoxelChannel&	in_src,
	Bifrost::API::VoxelChannel&			in_dst_u,
	Bifrost::API::VoxelChannel&			in_dst_v
)
	: c_src(in_src), c_dst_u(in_dst_u), c_dst_v(in_dst_v)
{
}

SeparateUVVisitor::SeparateUVVisitor(const SeparateUVVisitor& other)
	: c_src(other.c_src), c_dst_u(other.c_dst_u), c_dst_v(other.c_dst_v)
{
}

void SeparateUVVisitor::beginTile(const Bifrost::API::TileAccessor&, const Bifrost::API::TreeIndex& index)
{
	Bifrost::API::TileData<amino::Math::vec2f> inData = c_src.tileData<amino::Math::vec2f>( index );
	Bifrost::API::TileData<float> outDataU = c_dst_u.tileData<float>( index );
	Bifrost::API::TileData<float> outDataV = c_dst_v.tileData<float>( index );
	for ( size_t k = 0; k < inData.count(); k++ ) {
		outDataU[k] = inData[k][0];
		outDataV[k] = inData[k][1];
	}
}

Bifrost::API::Visitor* SeparateUVVisitor::copy() const
{
	return new SeparateUVVisitor(*this);
	}

//
// SeparateParticleUVVisitor Visitor Functions
//

SeparateParticleUVVisitor::SeparateParticleUVVisitor(
	const Bifrost::API::PointChannel&	in_src,
	Bifrost::API::PointChannel&			in_dst_u,
	Bifrost::API::PointChannel&			in_dst_v
)
	: c_src(in_src), c_dst_u(in_dst_u), c_dst_v(in_dst_v)
{
}

SeparateParticleUVVisitor::SeparateParticleUVVisitor(const SeparateParticleUVVisitor& other)
	: c_src(other.c_src), c_dst_u(other.c_dst_u), c_dst_v(other.c_dst_v)
{
}

void SeparateParticleUVVisitor::beginTile(const Bifrost::API::TileAccessor&, const Bifrost::API::TreeIndex& index)
{
	Bifrost::API::TileData<amino::Math::vec2f> inData = c_src.tileData<amino::Math::vec2f>( index );
	Bifrost::API::TileData<float> outDataU = c_dst_u.tileData<float>( index );
	Bifrost::API::TileData<float> outDataV = c_dst_v.tileData<float>( index );
	for ( size_t k = 0; k < inData.count(); k++ ) {
		outDataU[k] = inData[k][0];
		outDataV[k] = inData[k][1];
		if ( k == 0 ) {
		printf("%f %f \n", inData[k][0], inData[k][1]);
		}
	}
}

Bifrost::API::Visitor* SeparateParticleUVVisitor::copy() const
{
	return new SeparateParticleUVVisitor(*this);
}

//
// RemapVoxelsVisitor Visitor Functions
//

RemapVoxelsVisitor::RemapVoxelsVisitor(
	const Bifrost::API::VoxelChannel& channel,
	float min,
	float max,
	bool invert
) :
	c_chan( channel ),
	c_min( min ),
	c_max( max ),
	c_invert( invert )
{
	c_range = c_max - c_min;
}

void RemapVoxelsVisitor::beginTile( const Bifrost::API::TileAccessor&, const Bifrost::API::TreeIndex& index )
{
	Bifrost::API::TileData<float> data = c_chan.tileData<float>( index );
	for ( size_t k = 0; k < data.count(); k++ ) {
		if ( data[k] <= c_min) {
			data[k] = c_invert ? 1.0f : 0.0f;
		} else if ( data[k] >= c_max ) {
			data[k] = c_invert ? 0.0f : 1.0f;
		} else {
			data[k] = clip <float> ( ( data [k] - c_min ) / c_range, 0.0, 1.0 );
			data[k] = c_invert ? 1.0f - data[k] : data[k];
		}
	}
}

Bifrost::API::Visitor* RemapVoxelsVisitor::copy() const
{
	return new RemapVoxelsVisitor( c_chan, c_min, c_max, c_invert );
}

//
// ClipVoxelsVisitor Visitor Functions
//

ClipVoxelsVisitor::ClipVoxelsVisitor(
	const Bifrost::API::Channel& channel,
	float clipMinX,
	float clipMaxX,
	float clipMinY,
	float clipMaxY,
	float clipMinZ,
	float clipMaxZ,
	float dx
) :
	inChannel( channel ),
	clipMinX( clipMinX ),
	clipMaxX( clipMaxX ),
	clipMinY( clipMinY ),
	clipMaxY( clipMaxY ),
	clipMinZ( clipMinZ ),
	clipMaxZ( clipMaxZ ),
	dx(dx)
{
}

void ClipVoxelsVisitor::beginTile( const Bifrost::API::TileAccessor& accessor, const Bifrost::API::TreeIndex& index )
{
	// Fetch the current tile
	const Bifrost::API::Tile tile = accessor.tile(index);

	// Get the voxel space coordinate and dimension info
	const Bifrost::API::TileCoord coord = tile.coord();
	const Bifrost::API::TileInfo info = tile.info();

	// Determine the world space bounding box of the tile
	float tileSize = info.dimInfo.depthWidth * dx;
	amino::Math::vec3f tileBoundsMin, tileBoundsMax;
	tileBoundsMin[0] = float(coord.i) * dx;
	tileBoundsMin[1] = float(coord.j) * dx;
	tileBoundsMin[2] = float(coord.k) * dx;
	tileBoundsMax[0] = tileBoundsMin[0] + tileSize;
	tileBoundsMax[1] = tileBoundsMin[1] + tileSize;
	tileBoundsMax[2] = tileBoundsMin[2] + tileSize;

	Bifrost::API::TileData<float> data = inChannel.tileData<float>( index );
	for ( size_t k = 0; k < data.count(); k++ ) {
		// if our tile does not intersect with the clip box set its values to 0.0f
		if (	( tileBoundsMax[0] + tileSize < clipMinX || tileBoundsMin[0] - tileSize > clipMaxX ) ||
				( tileBoundsMax[1] + tileSize < clipMinY || tileBoundsMin[1] - tileSize > clipMaxY ) ||
				( tileBoundsMax[2] + tileSize < clipMinZ || tileBoundsMin[2] - tileSize > clipMaxZ ) ) {
			data[k] = 0.0f;
		}
	}
}

Bifrost::API::Visitor* ClipVoxelsVisitor::copy() const
{
	return new ClipVoxelsVisitor( inChannel, clipMinX, clipMaxX, clipMinY, clipMaxY, clipMinZ, clipMaxZ, dx );
}

//
// BlendVoxelsVisitor Visitor Functions
//

BlendVoxelsVisitor::BlendVoxelsVisitor(
	float								in_weight,
	const Bifrost::API::VoxelChannel&	in_src,
	Bifrost::API::VoxelChannel&			in_blender,
	Bifrost::API::VoxelChannel&			in_dst
)
	: c_weight(in_weight), c_src(in_src), c_blender(in_blender), c_dst(in_dst) 
{
}

BlendVoxelsVisitor::BlendVoxelsVisitor(const BlendVoxelsVisitor& other)
	: c_weight(other.c_weight), c_src(other.c_src), c_blender(other.c_blender), c_dst(other.c_dst)
{
}

void BlendVoxelsVisitor::beginTile(const Bifrost::API::TileAccessor&, const Bifrost::API::TreeIndex& index)
{
	Bifrost::API::TileData<float> inData = c_src.tileData<float>( index );
	Bifrost::API::TileData<float> outData = c_dst.tileData<float>( index );
	Bifrost::API::TileData<float> blenderData = c_blender.tileData<float>( index );

	for ( size_t k = 0; k < inData.count(); k++ ) {
		if ( c_weight < FLT_EPSILON ) {
			outData[k] = inData[k];
		} else {
			float finalBlend = c_weight;
			if ( c_blender.valid() ) {
				finalBlend = c_weight * blenderData[k];
			}

			if ( finalBlend < FLT_EPSILON ) {
				outData[k] = inData[k];
			} else if ( finalBlend < 1.0f ) {
				outData[k] = inData[k] - finalBlend * (inData[k] - outData[k]);
			}
		}
	}
}

Bifrost::API::Visitor* BlendVoxelsVisitor::copy() const
{
	return new BlendVoxelsVisitor(*this);
}

//
// CalcVelocityMagnitudeVisitor Visitor Functions
//

CalcVelocityMagnitudeVisitor::CalcVelocityMagnitudeVisitor(
	const Bifrost::API::VoxelChannel&	in_u,
	const Bifrost::API::VoxelChannel&	in_v,
	const Bifrost::API::VoxelChannel&	in_w,
	const Bifrost::API::VoxelChannel&	in_dst
)
	: m_velu(in_u), m_velv(in_v), m_velw(in_w), m_dst(in_dst)
{
	m_minVal = std::numeric_limits<float>::max();
	m_maxVal = -std::numeric_limits<float>::max();
}

CalcVelocityMagnitudeVisitor::CalcVelocityMagnitudeVisitor(const CalcVelocityMagnitudeVisitor& other)
	: m_velu(other.m_velu), m_velv(other.m_velv), m_velw(other.m_velw), m_dst(other.m_dst)
{
	m_minVal = std::numeric_limits<float>::max();
	m_maxVal = -std::numeric_limits<float>::max();
}

void CalcVelocityMagnitudeVisitor::beginTile(const Bifrost::API::TileAccessor&, const Bifrost::API::TreeIndex& index)
{
	Bifrost::API::TileData<float> inUData = m_velu.tileData<float>( index );
	Bifrost::API::TileData<float> inVData = m_velv.tileData<float>( index );
	Bifrost::API::TileData<float> inWData = m_velw.tileData<float>( index );
	Bifrost::API::TileData<float> outData = m_dst.tileData<float>( index );
	for ( size_t k = 0; k < inUData.count(); k++ ) {
		float mag = sqrt( inUData[k] * inUData[k] + inVData[k] * inVData[k] + inWData[k] * inWData[k] );
		outData[k] = mag;
		m_minVal = std::min(m_minVal, mag);
		m_maxVal = std::max(m_maxVal, mag);
	}
}

Bifrost::API::Visitor* CalcVelocityMagnitudeVisitor::copy() const
{
	return new CalcVelocityMagnitudeVisitor(*this);
}

void CalcVelocityMagnitudeVisitor::join(const Bifrost::API::Visitor& visitor)
{
	const CalcVelocityMagnitudeVisitor& other = dynamic_cast<const CalcVelocityMagnitudeVisitor&>(visitor);
	m_minVal = std::min(m_minVal, other.m_minVal);
	m_maxVal = std::max(m_maxVal, other.m_maxVal);
}

//
// CalcPointVelocityMagnitudeVisitor Visitor Functions
//

CalcPointVelocityMagnitudeVisitor::CalcPointVelocityMagnitudeVisitor(
	const Bifrost::API::PointChannel&	in_vel,
	const Bifrost::API::PointChannel&	in_dst
)
	: m_vel(in_vel), m_dst(in_dst)
{
	m_minVal = std::numeric_limits<float>::max();
	m_maxVal = -std::numeric_limits<float>::max();
}

CalcPointVelocityMagnitudeVisitor::CalcPointVelocityMagnitudeVisitor(const CalcPointVelocityMagnitudeVisitor& other)
	: m_vel(other.m_vel), m_dst(other.m_dst)
{
	m_minVal = std::numeric_limits<float>::max();
	m_maxVal = -std::numeric_limits<float>::max();
}

void CalcPointVelocityMagnitudeVisitor::beginTile(const Bifrost::API::TileAccessor&, const Bifrost::API::TreeIndex& index)
{
	Bifrost::API::TileData<amino::Math::vec3f> inVelData = m_vel.tileData<amino::Math::vec3f>( index );
	Bifrost::API::TileData<float> outData = m_dst.tileData<float>( index );
	for ( size_t k = 0; k < inVelData.count(); k++ ) {
		float speed = sqrt( inVelData[k][0] * inVelData[k][0] + inVelData[k][1] * inVelData[k][1] + inVelData[k][2] * inVelData[k][2] );
		outData[k] = speed;
		m_minVal = std::min(m_minVal, speed);
		m_maxVal = std::max(m_maxVal, speed);
	}
}

Bifrost::API::Visitor* CalcPointVelocityMagnitudeVisitor::copy() const
{
	return new CalcPointVelocityMagnitudeVisitor(*this);
}

void CalcPointVelocityMagnitudeVisitor::join(const Bifrost::API::Visitor& visitor)
{
	const CalcPointVelocityMagnitudeVisitor& other = dynamic_cast<const CalcPointVelocityMagnitudeVisitor&>(visitor);
	m_minVal = std::min(m_minVal, other.m_minVal);
	m_maxVal = std::max(m_maxVal, other.m_maxVal);
}

//
// CalcSpeedVisitor Visitor Functions
//

CalcSpeedVisitor::CalcSpeedVisitor(
	const Bifrost::API::PointChannel&	in,
	const Bifrost::API::PointChannel&	in_dst
)
	: m_vel(in), m_dst(in_dst)
{
	m_minVal = std::numeric_limits<float>::max();
	m_maxVal = -std::numeric_limits<float>::max();
}

CalcSpeedVisitor::CalcSpeedVisitor(const CalcSpeedVisitor& other)
	: m_vel(other.m_vel), m_dst(other.m_dst)
{
	m_minVal = std::numeric_limits<float>::max();
	m_maxVal = -std::numeric_limits<float>::max();
}

void CalcSpeedVisitor::beginTile(const Bifrost::API::TileAccessor&, const Bifrost::API::TreeIndex& index)
{
	Bifrost::API::TileData<amino::Math::vec3f> inData = m_vel.tileData<amino::Math::vec3f>( index );
	Bifrost::API::TileData<float> outData = m_dst.tileData<float>( index );
	for ( size_t k = 0; k < inData.count(); k++ ) {
		float speed = sqrt( inData[k][0] * inData[k][0] + inData[k][1] * inData[k][1] + inData[k][2] * inData[k][2] );
		outData[k] = speed;
		m_minVal = std::min(m_minVal, speed);
		m_maxVal = std::max(m_maxVal, speed);
	}
}

Bifrost::API::Visitor* CalcSpeedVisitor::copy() const
{
	return new CalcSpeedVisitor(*this);
}

void CalcSpeedVisitor::join(const Bifrost::API::Visitor& visitor)
{
	const CalcSpeedVisitor& other = dynamic_cast<const CalcSpeedVisitor&>(visitor);
	m_minVal = std::min(m_minVal, other.m_minVal);
	m_maxVal = std::max(m_maxVal, other.m_maxVal);
}

//
// TileBoundsVisitor Visitor Functions
//

TileBoundsVisitor::TileBoundsVisitor()
{
    c_tileBoundsMin[0] = c_tileBoundsMin[1] = c_tileBoundsMin[2] =
        std::numeric_limits<int>::max();
    c_tileBoundsMax[0] = c_tileBoundsMax[1] = c_tileBoundsMax[2] =
        -std::numeric_limits<int>::max();
}

Bifrost::API::Visitor* TileBoundsVisitor::copy() const { return new TileBoundsVisitor(); }

void TileBoundsVisitor::beginTile(const Bifrost::API::TileAccessor& accessor,
				const Bifrost::API::TreeIndex& index)
{
	// Fetch the current tile
	const Bifrost::API::Tile tile = accessor.tile(index);

	// Get the voxel space coordinate and dimension info
	const Bifrost::API::TileCoord coord = tile.coord();
	const Bifrost::API::TileInfo info = tile.info();

	// Determine the voxel space bounding box of the tile
	amino::Math::vec3i tileBoundsMin, tileBoundsMax;
	tileBoundsMin[0] = coord.i;
	tileBoundsMin[1] = coord.j;
	tileBoundsMin[2] = coord.k;
	tileBoundsMax[0] = coord.i + info.dimInfo.depthWidth;
	tileBoundsMax[1] = coord.j + info.dimInfo.depthWidth;
	tileBoundsMax[2] = coord.k + info.dimInfo.depthWidth;

	// Expand the bounding box
	for (int i = 0; i < 3; i++) {
		c_tileBoundsMin[i] = std::min(c_tileBoundsMin[i], tileBoundsMin[i] - 3 );
		c_tileBoundsMax[i] = std::max(c_tileBoundsMax[i], tileBoundsMax[i] + 3 );
	}
}

void TileBoundsVisitor::join(const Bifrost::API::Visitor& visitor)
{
	const TileBoundsVisitor& other = dynamic_cast<const TileBoundsVisitor&>(visitor);

	for (int i = 0; i < 3; i++)
	{
		c_tileBoundsMin[i] = std::min(c_tileBoundsMin[i], other.c_tileBoundsMin[i]);
		c_tileBoundsMax[i] = std::max(c_tileBoundsMax[i], other.c_tileBoundsMax[i]);
	}
}

//
// TileBoundsVisitor2 Visitor Functions
//

TileBoundsVisitor2::TileBoundsVisitor2( Bifrost::API::VoxelChannel&	srcChannel )
    : c_srcChannel (srcChannel)
{
    c_tileBoundsMin[0] = c_tileBoundsMin[1] = c_tileBoundsMin[2] = std::numeric_limits<int>::max();
    c_tileBoundsMax[0] = c_tileBoundsMax[1] = c_tileBoundsMax[2] = -std::numeric_limits<int>::max();
}

TileBoundsVisitor2::TileBoundsVisitor2(const TileBoundsVisitor2& other)
    : c_srcChannel(other.c_srcChannel)
{
    c_tileBoundsMin[0] = c_tileBoundsMin[1] = c_tileBoundsMin[2] =	std::numeric_limits<int>::max();
    c_tileBoundsMax[0] = c_tileBoundsMax[1] = c_tileBoundsMax[2] = -std::numeric_limits<int>::max();

    // A new accessor for this thread
    Bifrost::API::Layout layout = c_srcChannel.layout();
    c_acc = layout.tileAccessor();
}

Bifrost::API::Visitor* TileBoundsVisitor2::copy() const
{
    return new TileBoundsVisitor2(*this);
}

void TileBoundsVisitor2::beginTile(const Bifrost::API::TileAccessor& accessor,
               const Bifrost::API::TreeIndex& index)
{
    // Fetch the current tile
    const Bifrost::API::Tile tile = accessor.tile(index);
    Bifrost::API::TileData<float> inData = c_srcChannel.tileData<float>( index );

    if ( inData.count() > 0 ) {
        bool notFound = true;
        for ( int i =0; notFound && i < inData.count(); ++i ) {
            if (inData[i] > 0.5 ) {
                notFound = false;
            }
        }

        // temporarily turn off above
        notFound = false;

        if ( !notFound ) {
            // Get the voxel space coordinate and dimension info
            const Bifrost::API::TileCoord coord = tile.coord();
            const Bifrost::API::TileInfo info = tile.info();

            // Determine the voxel space bounding box of the tile
            amino::Math::vec3i tileBoundsMin, tileBoundsMax;
            tileBoundsMin[0] = coord.i;
            tileBoundsMin[1] = coord.j;
            tileBoundsMin[2] = coord.k;
            tileBoundsMax[0] = coord.i + info.dimInfo.depthWidth;
            tileBoundsMax[1] = coord.j + info.dimInfo.depthWidth;
            tileBoundsMax[2] = coord.k + info.dimInfo.depthWidth;

            // Expand the bounding box
            for (int i = 0; i < 3; i++) {
                c_tileBoundsMin[i] = std::min(c_tileBoundsMin[i], tileBoundsMin[i]);
                c_tileBoundsMax[i] = std::max(c_tileBoundsMax[i], tileBoundsMax[i]);
            }
        }
    }
}

void TileBoundsVisitor2::join(const Bifrost::API::Visitor& visitor)
{
    const TileBoundsVisitor2& other = dynamic_cast<const TileBoundsVisitor2&>(visitor);

    for (int i = 0; i < 3; i++) {
        c_tileBoundsMin[i] = std::min(c_tileBoundsMin[i], other.c_tileBoundsMin[i]);
        c_tileBoundsMax[i] = std::max(c_tileBoundsMax[i], other.c_tileBoundsMax[i]);
    }
}

//
// FrustumCullVisitor Visitor Functions
//

FrustumCullVisitor::FrustumCullVisitor(
	Bifrost::API::Channel chan,
	FrameData *frameData
) :
	c_chan(chan),
	c_frameData( frameData )
{
	Bifrost::API::Layout layout = c_chan.layout();
	c_acc = layout.tileAccessor();
}

FrustumCullVisitor::FrustumCullVisitor(const FrustumCullVisitor& other)
	:	c_chan(other.c_chan),
		c_frameData(other.c_frameData)
{
	// A new accessor for this thread
	Bifrost::API::Layout layout = c_chan.layout();
	c_acc = layout.tileAccessor();
}

void FrustumCullVisitor::beginTile( const Bifrost::API::TileAccessor&, const Bifrost::API::TreeIndex& index )
{
	// Fetch the current tile
	const Bifrost::API::Tile tile = c_acc.tile(index);

	// Get the voxel space coordinate and dimension info
	const Bifrost::API::TileCoord coord = tile.coord();
	const Bifrost::API::TileInfo info = tile.info();

	// Determine the voxel space bounding box of the tile
	amino::Math::vec3f tileBoundsMin, tileBoundsMax;
	tileBoundsMin[0] = (float) coord.i;
	tileBoundsMin[1] = (float) coord.j;
	tileBoundsMin[2] = (float) coord.k;
	tileBoundsMax[0] = (float) coord.i + info.dimInfo.depthWidth;
	tileBoundsMax[1] = (float) coord.j + info.dimInfo.depthWidth;
	tileBoundsMax[2] = (float) coord.k + info.dimInfo.depthWidth;

	// convert to worldspace
	float scalingFactor = c_acc.layout().voxelScale();
	tileBoundsMin *= scalingFactor;
	tileBoundsMax *= scalingFactor;

	CvPoint corners[8];
	corners[0][0] = tileBoundsMin[0];
	corners[0][1] = tileBoundsMin[1];
	corners[0][2] = tileBoundsMin[2];

	corners[1][0] = tileBoundsMax[0];
	corners[1][1] = tileBoundsMin[1];
	corners[1][2] = tileBoundsMin[2];

	corners[2][0] = tileBoundsMin[0];
	corners[2][1] = tileBoundsMax[1];
	corners[2][2] = tileBoundsMin[2];

	corners[3][0] = tileBoundsMax[0];
	corners[3][1] = tileBoundsMax[1];
	corners[3][2] = tileBoundsMin[2];

	corners[4][0] = tileBoundsMin[0];
	corners[4][1] = tileBoundsMin[1];
	corners[4][2] = tileBoundsMax[2];

	corners[5][0] = tileBoundsMax[0];
	corners[5][1] = tileBoundsMin[1];
	corners[5][2] = tileBoundsMax[2];

	corners[6][0] = tileBoundsMin[0];
	corners[6][1] = tileBoundsMax[1];
	corners[6][2] = tileBoundsMax[2];

	corners[7][0] = tileBoundsMax[0];
	corners[7][1] = tileBoundsMax[1];
	corners[7][2] = tileBoundsMax[2];

	if ( !boundIntersectsWithFrustum ( corners, c_frameData->planes ) ) {
		c_cullIndexes.add ( index );
	}
}

Bifrost::API::Visitor* FrustumCullVisitor::copy() const
{
	return new FrustumCullVisitor( *this );
}

void FrustumCullVisitor::join(const Bifrost::API::Visitor& visitor)
{
	const FrustumCullVisitor& other = dynamic_cast<const FrustumCullVisitor&> ( visitor );
	// printf("%d\n", other.c_cullIndexes.count());
	for ( int i = 0; i < other.c_cullIndexes.count(); i++ ) {
		c_cullIndexes.add( other.c_cullIndexes[i] );
	}
}

//
// ChannelRangeVisitor Visitor Functions
//

ChannelRangeVisitor::ChannelRangeVisitor(
	const Bifrost::API::VoxelChannel&	src,
	const Bifrost::API::VoxelChannel&	minMax,
	amino::Math::vec3f					minCorner,
	amino::Math::vec3f					maxCorner
)
	: m_src(src), m_minMax(minMax), m_minCorner(minCorner), m_maxCorner(maxCorner)
{
	Bifrost::API::Layout layout = m_src.layout();
	m_acc = layout.tileAccessor();
		
	m_minVal = std::numeric_limits<float>::max();
	m_maxVal = -std::numeric_limits<float>::max();
}

ChannelRangeVisitor::ChannelRangeVisitor(const ChannelRangeVisitor& other)
	: m_src(other.m_src), m_minMax(other.m_minMax), m_minCorner(other.m_minCorner), m_maxCorner(other.m_maxCorner)
{
	m_minVal = std::numeric_limits<float>::max();
	m_maxVal = -std::numeric_limits<float>::max();
	// A new accessor for this thread
	Bifrost::API::Layout layout = m_src.layout();
	m_acc = layout.tileAccessor();
}

void ChannelRangeVisitor::beginTile(const Bifrost::API::TileAccessor&, const Bifrost::API::TreeIndex& index)
{
	const Bifrost::API::TileInfo& tinfo = m_acc.tileInfo(index);

	// check whether this tile is within the limits
	if (	tinfo.i >= m_minCorner[0] && tinfo.i <= m_maxCorner[0] + 1 &&
			tinfo.j >= m_minCorner[1] && tinfo.j <= m_maxCorner[1] + 1 &&
			tinfo.k >= m_minCorner[2] && tinfo.k <= m_maxCorner[2] + 1 && tinfo.valid)
	{
		// get the min max data and store it
		Bifrost::API::TileData<amino::Math::vec2f> inData = m_minMax.tileData<amino::Math::vec2f>( index );
		for (int i = 0; i < inData.count(); i++ ) {
			m_minVal = std::min(m_minVal, inData[i][0]);
			m_maxVal = std::max(m_maxVal, inData[i][1]);
			if (m_minVal == 0.0f && m_maxVal == 1.0f) {
				// already found the limit
				break;
			}
		}
	}
}

Bifrost::API::Visitor* ChannelRangeVisitor::copy() const
{
	return new ChannelRangeVisitor(*this);
}

void ChannelRangeVisitor::join(const Bifrost::API::Visitor& visitor)
{
	const ChannelRangeVisitor& other = static_cast<const ChannelRangeVisitor&>(visitor);
	m_minVal = std::min(m_minVal, other.m_minVal);
	m_maxVal = std::max(m_maxVal, other.m_maxVal);
}

//
// ChannelValueRangeVisitor Visitor Functions
//

ChannelValueRangeVisitor::ChannelValueRangeVisitor(
	const Bifrost::API::Channel	src
)
	: m_src(src)
{
	m_minVal = std::numeric_limits<float>::max();
	m_maxVal = -std::numeric_limits<float>::max();
}

ChannelValueRangeVisitor::ChannelValueRangeVisitor(const ChannelValueRangeVisitor& other)
	: m_src(other.m_src)
{
	m_minVal = std::numeric_limits<float>::max();
	m_maxVal = -std::numeric_limits<float>::max();
}

void ChannelValueRangeVisitor::beginTile(const Bifrost::API::TileAccessor& , const Bifrost::API::TreeIndex& index)
{
	// get the min max data and store it
	Bifrost::API::TileData<float> inData = m_src.tileData<float>( index );

	for (int i = 0; i < inData.count(); i++ ) {
		m_minVal = std::min(m_minVal, inData[i]);
		m_maxVal = std::max(m_maxVal, inData[i]);
	}
}

Bifrost::API::Visitor* ChannelValueRangeVisitor::copy() const
{
	return new ChannelValueRangeVisitor(*this);
}

void ChannelValueRangeVisitor::join(const Bifrost::API::Visitor& visitor)
{
	const ChannelValueRangeVisitor& other = static_cast<const ChannelValueRangeVisitor&>(visitor);
	m_minVal = std::min(m_minVal, other.m_minVal);
	m_maxVal = std::max(m_maxVal, other.m_maxVal);
}

//
// CalcTileMinMaxVisitor Visitor Functions
//

CalcTileMinMaxVisitor::CalcTileMinMaxVisitor(
	const Bifrost::API::VoxelChannel&	src,
	const Bifrost::API::VoxelChannel&	minmax
)
	: m_src(src), m_minMax(minmax)
{
}

CalcTileMinMaxVisitor::CalcTileMinMaxVisitor(const CalcTileMinMaxVisitor& other)
	: m_src(other.m_src), m_minMax(other.m_minMax)
{
}

void CalcTileMinMaxVisitor::beginTile(const Bifrost::API::TileAccessor&, const Bifrost::API::TreeIndex& index)
{
	float minVal = std::numeric_limits<float>::max();
	float maxVal = -std::numeric_limits<float>::max();
	Bifrost::API::TileData<float> inData = m_src.tileData<float>( index );
	Bifrost::API::TileData<amino::Math::vec2f> outData = m_minMax.tileData<amino::Math::vec2f>( index );
	for ( size_t k = 0; k < inData.count(); k++ ) {
		minVal = std::min(minVal, inData[k]);
		maxVal = std::max(maxVal, inData[k]);
	}
	//	printf("%f %f\n", minVal, maxVal);
	for ( size_t k = 0; k < outData.count(); k++ ) {

		outData[k][0] = minVal;
		outData[k][1] = maxVal;
	}
}

Bifrost::API::Visitor* CalcTileMinMaxVisitor::copy() const
{
	return new CalcTileMinMaxVisitor(*this);
}

//
// SDFToFogVisitor Visitor Functions
//

SDFToFogVisitor::SDFToFogVisitor(
	const Bifrost::API::VoxelChannel&	in_src,
	Bifrost::API::VoxelChannel&			in_airDistance
)
	: c_src(in_src), c_airDistance(in_airDistance)
{
	// A new accessor for this thread
	Bifrost::API::Layout layout = c_src.layout();
	c_acc = layout.tileAccessor();
}

SDFToFogVisitor::SDFToFogVisitor(const SDFToFogVisitor& other)
	: c_src(other.c_src), c_airDistance(other.c_airDistance)
{
	// A new accessor for this thread
	Bifrost::API::Layout layout = c_src.layout();
	c_acc = layout.tileAccessor();
}

void SDFToFogVisitor::beginTile(const Bifrost::API::TileAccessor& accessor, const Bifrost::API::TreeIndex& index)
{
	Bifrost::API::TileData<amino::Math::vec3f> inData = c_src.tileData<amino::Math::vec3f>( index );
	Bifrost::API::TileData<float> airDistanceData = c_airDistance.tileData<float>( index );

	for ( size_t k = 0; k < inData.count(); k++ ) {

	}
}

Bifrost::API::Visitor* SDFToFogVisitor::copy() const
{
	return new SDFToFogVisitor(*this);
}

//
// SmoothBackSideVisitor Visitor Functions
//

SmoothBackSideVisitor::SmoothBackSideVisitor( const Bifrost::API::VoxelChannel& channel )
	: c_channel ( channel )
{
	// initialize the cache with the input voxels
	c_haloCache.init( channel );
}

SmoothBackSideVisitor::SmoothBackSideVisitor( const SmoothBackSideVisitor& other )
{
	c_haloCache = other.c_haloCache;
	c_channel = other.c_channel;
}

Bifrost::API::Visitor* SmoothBackSideVisitor::copy( ) const
{ 
	return new SmoothBackSideVisitor(*this);
}

void SmoothBackSideVisitor::beginTile( const Bifrost::API::TileAccessor&, const Bifrost::API::TreeIndex& index )
{
	c_haloCache.update( index );
	const Bifrost::API::HaloedCache<float>::Tile& in = c_haloCache.tile( index );
	Bifrost::API::TileData<float> inData = c_channel.tileData<float>( index );

	// if the voxel value is 1.0 and surrounding voxels has at least 1 zero value
	// set the value of this tile to 0.5
	for (int data = 0; data < inData.count(); data++ ) {
		if ( inData[ data ] - 1.0f < FLT_EPSILON ) {
			// our tile has 1.0 as a value
			// so check neighbours
			bool foundZero = false;
			for ( int k = -1; k < 1; k++ ) {
				for ( int j = -1; j < 1; j++ ) {
					for ( int i = -1; i < 1; i++ ) {
						int voxelI = int ( data % 5);
						int voxelJ = ( int ( data / 5) ) % 5;
						int voxelK = int ( data / 25);

						int lookupI = std::max<int> ( voxelI + i, 0 );
						int lookupJ = std::max<int> ( voxelJ + j, 0 );
						int lookupK = std::max<int> ( voxelK + k, 0 );

						if ( inData [ lookupK * 25 + lookupJ * 5 + lookupI ] < FLT_EPSILON ) {
							// we have at least one zero in the neighbourhood
							inData[ data ] = 0.5f;
							foundZero = true;
						}

						if ( foundZero ) {
							break;
						}
					} // end k loop

					if ( foundZero ) {
						break;
					}
				}// end j loop
				if ( foundZero ) {
					break;
				}
			} // end i loop
		} // end 1.0 check
	} // end data loop
} // end begintile

//
// UInt64ValueRangeVisitor Functions
//

UInt64ValueRangeVisitor::UInt64ValueRangeVisitor(
	const Bifrost::API::PointChannel	src
)
	: m_src(src)
{
	m_minVal = uint64_t(0);
	m_maxVal = uint64_t(0);
}

UInt64ValueRangeVisitor::UInt64ValueRangeVisitor(const UInt64ValueRangeVisitor& other)
	: m_src(other.m_src)
{
	m_minVal = uint64_t(0);
	m_maxVal = uint64_t(0);
}

void UInt64ValueRangeVisitor::beginTile(const Bifrost::API::TileAccessor&, const Bifrost::API::TreeIndex& index)
{
	// get the min max data and store it
	Bifrost::API::TileData<uint64_t> inData = m_src.tileData<uint64_t>( index );
	for (int i = 0; i < inData.count(); i++ ) {
		m_minVal = std::min<uint64_t>(m_minVal, inData[i]);
		m_maxVal = std::max<uint64_t>(m_maxVal, inData[i]);
	}
}

Bifrost::API::Visitor* UInt64ValueRangeVisitor::copy() const
{
	return new UInt64ValueRangeVisitor(*this);
}

void UInt64ValueRangeVisitor::join(const Bifrost::API::Visitor& visitor)
{
	const UInt64ValueRangeVisitor& other = static_cast<const UInt64ValueRangeVisitor&>(visitor);
	m_minVal = std::min<uint64_t>(m_minVal, other.m_minVal);
	m_maxVal = std::max<uint64_t>(m_maxVal, other.m_maxVal);
}

//
// CalcNofParticlesVisitor Functions
//

CalcNofParticlesVisitor::CalcNofParticlesVisitor(
	const Bifrost::API::PointChannel	src,
	int									skip
)
	:	m_src( src ),
		m_skip( skip )
{
	m_nofParticles = uint64_t(0);
}

CalcNofParticlesVisitor::CalcNofParticlesVisitor(const CalcNofParticlesVisitor& other)
	:	m_src( other.m_src ),
		m_skip( other.m_skip )
{
	m_nofParticles = uint64_t(0);
}

void CalcNofParticlesVisitor::beginTile(const Bifrost::API::TileAccessor&, const Bifrost::API::TreeIndex& index)
{
	// get the min max data and store it
	Bifrost::API::TileData<uint64_t> inData = m_src.tileData<uint64_t>( index );
	for (int i = 0; i < inData.count(); i++ ) {
		bool notSkipping = ( inData[i] % ( (uint64_t) m_skip ) == (uint64_t) 0 );
		if ( notSkipping ) {
			m_nofParticles++;
		}
	}
}

Bifrost::API::Visitor* CalcNofParticlesVisitor::copy() const
{
	return new CalcNofParticlesVisitor(*this);
}

void CalcNofParticlesVisitor::join(const Bifrost::API::Visitor& visitor)
{
	const CalcNofParticlesVisitor& other = static_cast<const CalcNofParticlesVisitor&>(visitor);
	m_nofParticles = m_nofParticles + other.m_nofParticles;
}


// AeroVoxelBoundsVisitor functions
AeroVoxelBoundsVisitor::AeroVoxelBoundsVisitor(
	const Bifrost::API::TreeIndex::Depth	maxDepth,
	const Bifrost::API::VoxelChannel		voxelChannel
	) :	m_maxDepth(maxDepth),
		m_voxelChannel(voxelChannel)
{
	m_tileBoundsMin[0] = m_tileBoundsMin[1] = m_tileBoundsMin[2] =  std::numeric_limits<int>::max();
	m_tileBoundsMax[0] = m_tileBoundsMax[1] = m_tileBoundsMax[2] = -std::numeric_limits<int>::max();
}

AeroVoxelBoundsVisitor::AeroVoxelBoundsVisitor(const AeroVoxelBoundsVisitor& other)
	:	m_maxDepth(other.m_maxDepth),
		m_voxelChannel(other.m_voxelChannel)
{
	m_tileBoundsMin[0] = m_tileBoundsMin[1] = m_tileBoundsMin[2] =  std::numeric_limits<int>::max();
	m_tileBoundsMax[0] = m_tileBoundsMax[1] = m_tileBoundsMax[2] = -std::numeric_limits<int>::max();
}

Bifrost::API::Visitor* AeroVoxelBoundsVisitor::copy() const
{
	return new AeroVoxelBoundsVisitor(*this);
}

void AeroVoxelBoundsVisitor::beginTile(const Bifrost::API::TileAccessor& accessor, const Bifrost::API::TreeIndex& index)
{
	// Get the voxel space coordinate and dimension info
	const Bifrost::API::Tile		tile	= accessor.tile(index);
	const Bifrost::API::TileCoord	coord	= tile.coord();
	const Bifrost::API::TileDimInfo	dim		= tile.info().dimInfo;

	// Voxel bounding box of this tile
	amino::Math::vec3i boundsMin = amino::Math::vec3i( coord.i, coord.j, coord.k );
	amino::Math::vec3i boundsMax = amino::Math::vec3i(
														coord.i + dim.depthWidth,
														coord.j + dim.depthWidth,
														coord.k + dim.depthWidth
													);

	// Include all cells of the finest voxel node.
	if (index.depth == m_maxDepth) {
		for (int i = 0; i < 3; i++) {
			m_tileBoundsMin[i] = std::min(m_tileBoundsMin[i], boundsMin[i]);
			m_tileBoundsMax[i] = std::max(m_tileBoundsMax[i], boundsMax[i]);
		}
		return;
	}

	// Iterate over each cell and include the cell if it has non-zero value.
	const Bifrost::API::TileData<float> tileData = m_voxelChannel.tileData<float>(index);

	for (int ii = 0; ii < dim.tileWidth; ii++) {
		for (int ij = 0; ij < dim.tileWidth; ij++) {
			for (int ik = 0; ik < dim.tileWidth; ik++) {
				// Not a leaf or a cell with zero value !
				if (tile.child(ii, ij, ik).valid() || tileData(ii, ij, ik) < MAYA_EPS) {
					continue;
				}

				// Get the bounds of the cell.
				amino::Math::vec3i voxelBoundsMin = amino::Math::vec3i	(
																			boundsMin[0] + ii * dim.voxelWidth,
																			boundsMin[1] + ij * dim.voxelWidth,
																			boundsMin[2] + ik * dim.voxelWidth 
																		);
				amino::Math::vec3i voxelBoundsMax = amino::Math::vec3i	(
																			voxelBoundsMin[0] + dim.voxelWidth,
																			voxelBoundsMin[1] + dim.voxelWidth,
																			voxelBoundsMin[2] + dim.voxelWidth
																		);

				// Expand the bounding box by the cell.
				for (int i = 0; i < 3; i++)
				{
					m_tileBoundsMin[i] = std::min(m_tileBoundsMin[i], voxelBoundsMin[i]);
					m_tileBoundsMax[i] = std::max(m_tileBoundsMax[i], voxelBoundsMax[i]);
				}
			} 
		} 
	}
}

void AeroVoxelBoundsVisitor::join(const Bifrost::API::Visitor& visitor)
{
	const AeroVoxelBoundsVisitor& other = dynamic_cast<const AeroVoxelBoundsVisitor&>(visitor);
	for (int i = 0; i < 3; i++) {
		m_tileBoundsMin[i] = std::min(m_tileBoundsMin[i], other.m_tileBoundsMin[i]);
		m_tileBoundsMax[i] = std::max(m_tileBoundsMax[i], other.m_tileBoundsMax[i]);
	}
}

//
// MarchingCubesVisitor Functions
//

MarchingCubesVisitor::MarchingCubesVisitor(
	FrameData	*frameData
)
	:	c_frameData( frameData )
{
	c_mesher = new MarchingCubes( c_frameData );

	// a new accessor for parallel processing
	Bifrost::API::Layout layout = c_frameData->srcChannel.layout();
	c_acc = layout.tileAccessor();
	c_dx = layout.voxelScale();
}

MarchingCubesVisitor::MarchingCubesVisitor(const MarchingCubesVisitor& other)
	:	c_frameData( other.c_frameData )
{
	c_mesher = new MarchingCubes( c_frameData );

	// a new accessor for parallel processing
	Bifrost::API::Layout layout = c_frameData->srcChannel.layout();
	c_acc = layout.tileAccessor();

	c_dx = other.c_dx;
}

void MarchingCubesVisitor::beginTile(const Bifrost::API::TileAccessor& , const Bifrost::API::TreeIndex& index)
{
	//Bifrost::API::TileData<float> inData = c_src.tileData<float>( index );

	// get the tile and its coordinate
	const Bifrost::API::Tile		tile	= c_acc.tile(index);
	const Bifrost::API::TileCoord	coord	= tile.coord();
	const Bifrost::API::TileDimInfo	dim		= tile.info().dimInfo;

	// now for each voxel in the tile, do marching cube calc
	size_t vertexCount = 0;
	float sampleRate = (float) c_frameData->sampleRate;
	for (int kk = 0; kk < dim.tileWidth * c_frameData->sampleRate; kk++) {
		for (int jj = 0; jj < dim.tileWidth  * c_frameData->sampleRate; jj++) {
			for (int ii = 0; ii < dim.tileWidth * c_frameData->sampleRate; ii++) {
				float gridVals[8];

				// calc the corner pos of this subvoxel in tile space
				amino::Math::vec3f voxelCornerPos ( (float) coord.i + (float) ii / sampleRate,
													(float) coord.j + (float) jj / sampleRate,
													(float) coord.k + (float) kk / sampleRate );

				// get grid values at the corners of this voxel
				for ( size_t i = 0; i < 8; i++ ) {
					// operate in tile space here
					amino::Math::vec3f corner(	c_mesher->c_cubeVertsSampleRate[i][0],
												c_mesher->c_cubeVertsSampleRate[i][1],
												c_mesher->c_cubeVertsSampleRate[i][2] );

					// calc sampling position
					amino::Math::vec3f voxelPos = voxelCornerPos + corner + c_mesher->c_voxelOffset;
							
					gridVals[i] = c_mesher->getFieldValue( voxelPos );
				}

				// run voxel mesher
				vertexCount += c_mesher->calcVoxel( gridVals, voxelCornerPos );
			}
		}
	}
}

Bifrost::API::Visitor* MarchingCubesVisitor::copy() const
{
	return new MarchingCubesVisitor(*this);
}

void MarchingCubesVisitor::join(const Bifrost::API::Visitor& visitor)
{
	const MarchingCubesVisitor& other = static_cast<const MarchingCubesVisitor&>(visitor);

	// get vertices
	int thisVertexCount = (int) c_mesher->getVertexCount();
	int otherVertexCount = (int) other.c_mesher->getVertexCount();
	int thisTriangleCount = (int) c_mesher->getTriangleCount();
	int otherTriangleCount = (int) other.c_mesher->getTriangleCount();

	if ( otherVertexCount > 0 ) {
		// join positions
		for ( int i = 0; i < otherVertexCount; i++ ) {
			c_mesher->addVertexPosition( other.c_mesher->getVertexPosition( i ) );
		}

		// join triangles
		for ( int i = 0; i < otherTriangleCount; i++ ) {
			amino::Math::vec3i curTri = other.c_mesher->getTriangle( i );
			amino::Math::vec3i newTri ( curTri[0] + thisVertexCount, curTri[1] + thisVertexCount, curTri[2] + thisVertexCount );
			c_mesher->addTriangle( newTri );
		}
	}
}

void MarchingCubesVisitor::endTraverse( const Bifrost::API::TileAccessor& )
{
	// this is called after all joining done, so we have one merged MarchingCube mesher around
	// we need to check for coinciding vertices across tiles here
	auto start_time = std::chrono::high_resolution_clock::now();

	size_t vertexCount = c_mesher->getVertexCount();
	size_t triangleCount = c_mesher->getTriangleCount();

	c_mesher->c_ids.clear();
	c_mesher->c_ids.rehash( vertexCount );

	c_mesher->c_remappedID.reserve( vertexCount );
	c_mesher->c_exportID.reserve( vertexCount );

	size_t mergedVertices = 0;
	for ( size_t i = 0; i < vertexCount; i++ ) {
		// check whether we have this vertex in our hashmap
		amino::Math::vec3f pos = c_mesher->getVertexPosition( i );

		auto posExists = c_mesher->c_ids.find( pos );
		if ( posExists != c_mesher->c_ids.end() ) {
			// this exist so remap this to the existing vertex
			c_mesher->c_remappedID.push_back( posExists->second );
			c_mesher->c_exportID.push_back( c_mesher->c_exportID[ c_mesher->c_remappedID[i] ] );
			++mergedVertices;
		} else {
			c_mesher->c_ids[pos] = i;
			c_mesher->c_remappedID.push_back( i );
			c_mesher->c_exportID.push_back( c_mesher->c_exportVertexCount );
			c_mesher->c_exportVertexCount++;
		}
	}

	// now go through all triangles and reset their vertex ids
	for ( size_t i = 0; i < triangleCount; i++ ) {
		amino::Math::vec3i vertexIds = c_mesher->getTriangle( i );

		vertexIds[0] = (int) c_mesher->c_exportID[ vertexIds[0] ];
		vertexIds[1] = (int) c_mesher->c_exportID[ vertexIds[1] ];
		vertexIds[2] = (int) c_mesher->c_exportID[ vertexIds[2] ];

		c_mesher->setTriangle( i, vertexIds );
	}

	// now that triangles are remapped we can trim the positions array
	// it is better to have another vector for the final positions as erasing will shift a lot of data a lot of times
	c_mesher->setMergedVertexArraySize( c_mesher->getExportVertexCount() );

	for ( size_t i = 0; i < vertexCount; i++ ) {
		if ( c_mesher->c_remappedID[i] == i ) {
			c_mesher->addMergedVertexPosition( c_mesher->getVertexPosition( i ) );
		}
	}

	auto end_time = std::chrono::high_resolution_clock::now();
	auto time = end_time - start_time;
	std::cout << "\tMesh Merging took " << std::chrono::duration_cast<std::chrono::microseconds>(time).count() / 1000000.0f << " seconds to run.\n";

	//
	//
	// NOW THAT WE HAVE A TRIMMED ARRAY OF POSITIONS WE CAN DO SAMPLING OF CHANNELS ONLY ON THOSE
	// WE ALSO NEED TO CHANGE POSITIONS FROM TILESPACE TO WORLDSPACE HERE
	//
	//
	
	// first resize and init vectors
	c_mesher->setVelocityArraySize( c_mesher->getExportVertexCount() );
	c_mesher->setNormalsArraySize( c_mesher->getExportVertexCount() );
	c_mesher->setPrimVarArraySizes( c_mesher->getExportVertexCount() );

	// timer for diagnostics
	start_time = std::chrono::high_resolution_clock::now();

	tbb::parallel_for   (	tbb::blocked_range<int>( 0, (int) c_mesher->getExportVertexCount(), 100 ),
							MeshSampler( c_mesher, c_dx )
						);

	// serial test

	end_time = std::chrono::high_resolution_clock::now();
	time = end_time - start_time;
	std::cout << "\tMesh Sampling took " << std::chrono::duration_cast<std::chrono::microseconds>(time).count() / 1000000.0f << " seconds to run.\n";
}

}}
