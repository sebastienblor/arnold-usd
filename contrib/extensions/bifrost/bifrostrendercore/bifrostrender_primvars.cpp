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

#include <bifrostrendercore/bifrostrender_primvars.h>
#include <bifrostrendercore/bifrostrender_visitors.h>

namespace Bifrost{
namespace RenderCore{

void createPrimVarChannel(	Bifrost::API::String primVar, Bifrost::API::DataType dataType,
							Bifrost::API::StateServer& ss, Bifrost::API::Component& component,
							Bifrost::API::VoxelOffsetType offsetType, float defVal)
{
	Bifrost::API::VoxelChannel tmpChannel;

	// get data according to type
	switch ( dataType ) {
		case Bifrost::API::FloatType:
			tmpChannel = ss.createChannel(component, Bifrost::API::FloatType, primVar );
			tmpChannel.setBackgroundValue( defVal );
			tmpChannel.setOffsetType( offsetType );
			break;
		case Bifrost::API::FloatV2Type:
			ss.createChannel(component, Bifrost::API::FloatV2Type, primVar );
			tmpChannel.setBackgroundValue( amino::Math::vec2f( defVal, defVal ) );
			tmpChannel.setOffsetType( offsetType );
			break;
		case Bifrost::API::FloatV3Type:
			ss.createChannel(component, Bifrost::API::FloatV3Type, primVar );
			tmpChannel.setBackgroundValue( amino::Math::vec3f( defVal, defVal, defVal ) );
			tmpChannel.setOffsetType( offsetType );
			break;
		default:
			assert(0); // type not supported
			break;
	}
}

void createPrimVarChannels( std::vector<primVarInfo>& primVars, Bifrost::API::StateServer& ss, Bifrost::API::Component& component, Bifrost::API::VoxelOffsetType offsetType, float defVal)
{
	for ( int var = 0; var < primVars.size(); var++) {
		createPrimVarChannel(	Bifrost::API::String( primVars[var].name.c_str() ), (primVars[var]).channel.dataType(),
								ss, component,
								offsetType, defVal );
	}
}

void declarePrimVars(	std::vector<primVarInfo>& primVars,
						std::vector<CvToken>& toks,
						std::vector<CvPointer>& ptr,
						std::vector<int>& varIndex,
						int nofElements,
						std::vector<void *>& mem,
						TokenClass tokenClass,
						std::string getPrimVarTokenCB( TokenClass, Bifrost::API::DataType, std::string ) )
{
	int arrayIndex = (int) toks.size();

	for ( unsigned var = 0; var < primVars.size(); var++ ) {
		if ( primVars[var].exportToRIB ) {
			// first treat special cases
			if ( primVars[var].name == "speed" ) {
					primVars[var].primDataArray = (CvFloat *) malloc ( nofElements * sizeof(CvFloat));
					mem.push_back( (void *) primVars[var].primDataArray );
					primVars[var].tokenName = getPrimVarTokenCB( tokenClass, Bifrost::API::FloatType, primVars[var].name ); 
			} else {
				// get data according to type
				switch ( (primVars[var]).channel.dataType() ) {
					case Bifrost::API::FloatType:
						primVars[var].primDataArray = (CvFloat *) malloc ( nofElements * sizeof(CvFloat));
						mem.push_back( (void *) primVars[var].primDataArray );
						primVars[var].tokenName = getPrimVarTokenCB( tokenClass, Bifrost::API::FloatType, primVars[var].name ); 
						break;
					case Bifrost::API::FloatV2Type:
						primVars[var].primDataArray = (CvFloat *) malloc ( 2 * nofElements * sizeof(CvFloat));
						mem.push_back( (void *) primVars[var].primDataArray );
						primVars[var].tokenName = getPrimVarTokenCB( tokenClass, Bifrost::API::FloatV2Type, primVars[var].name ); 
						break;
					case Bifrost::API::FloatV3Type:
						primVars[var].primDataArray = (CvFloat *) malloc ( 3 * nofElements * sizeof(CvFloat));
						mem.push_back( (void *) primVars[var].primDataArray );
						primVars[var].tokenName = getPrimVarTokenCB( tokenClass, Bifrost::API::FloatV3Type, primVars[var].name ); 
						break;
					default:
						assert(0); // type not supported
						break;
				}
			} // end speed check

			// set tokens
			toks.push_back(const_cast<char*>( primVars[var].tokenName.c_str() ));
			ptr.push_back(const_cast<CvFloat *>( primVars[var].primDataArray ));
			varIndex.push_back( arrayIndex );
			primVars[var].exportArraysIndex = arrayIndex;
			arrayIndex++;
		} // end exportToRIB check
	} // end loop
}

void assignPrimVars( std::vector<primVarInfo>& primVars, int xExport, int dataIndex, int samples)
{
	int k = 0;
	for ( unsigned var = 0; var < primVars.size(); var++ ) {
		// first handle special cases
		if ( primVars[var].name == "speed" ) {
			k = xExport;
			// first get velocity vector
			CvVector velocity;
			velocity[0] = ( (amino::Math::vec3f *) primVars[var].inDataArray )[dataIndex][0];
			velocity[1] = ( (amino::Math::vec3f *) primVars[var].inDataArray )[dataIndex][1];
			velocity[2] = ( (amino::Math::vec3f *) primVars[var].inDataArray )[dataIndex][2];

			// calc speed
			float speed = sqrt ( velocity[0] * velocity[0] + velocity[1] * velocity[1] + velocity[2] * velocity[2] );

			// now set the channel
			for (int sample = 0; sample < samples; sample++) {
				primVars[var].primDataArray[k++] = speed;
			}
			primVars[var].min = std::min<float>( primVars[var].min,  speed );
			primVars[var].max = std::max<float>( primVars[var].max,  speed );
		} else {
			// get data according to type
			switch ( primVars[var].channel.dataType() ) {
				case Bifrost::API::FloatType:
					k = xExport;
					for (int sample = 0; sample < samples; sample++) {
						primVars[var].primDataArray[k++] = ( (float *) primVars[var].inDataArray )[dataIndex];
					}
					primVars[var].min = std::min<float>( primVars[var].min,  ( (float *) primVars[var].inDataArray )[dataIndex] );
					primVars[var].max = std::max<float>( primVars[var].max,  ( (float *) primVars[var].inDataArray )[dataIndex] );
					break;
				case Bifrost::API::FloatV2Type:
					k = 2 * xExport;
					for (int sample = 0; sample < samples; sample++) {
						primVars[var].primDataArray[k++] = ( (amino::Math::vec2f *) primVars[var].inDataArray )[dataIndex][0];
						primVars[var].primDataArray[k++] = ( (amino::Math::vec2f *) primVars[var].inDataArray )[dataIndex][1];
					}
					break;
				case Bifrost::API::FloatV3Type:
					k = 3 * xExport;
					for (int sample = 0; sample < samples; sample++) {
						primVars[var].primDataArray[k++] = ( (amino::Math::vec3f *) primVars[var].inDataArray )[dataIndex][0];
						primVars[var].primDataArray[k++] = ( (amino::Math::vec3f *) primVars[var].inDataArray )[dataIndex][1];
						primVars[var].primDataArray[k++] = ( (amino::Math::vec3f *) primVars[var].inDataArray )[dataIndex][2];
					}
					break;
				default:
					assert(0); // type not supported
					break;
			}
		}
	}
}

void outputPrimVarsToConsole ( std::vector<CvToken>& toks )
{
	if ( toks.size() > 0 ) {
		printf("\t\tOutput with:\n");

		for ( unsigned i = 0; i < toks.size(); i++ ) {
			printf("\t\t\t%s\n", toks[i] );
		}
	}
}

Bifrost::API::Channel checkPrimVarAndMakeDuplicate( std::string channelName, Bifrost::API::StateServer srcSS, Bifrost::API::Component component, char *inputChannelName)
{

	bool duplicateNeeded = false;

	// check specific channels
	if	(
			channelName.compare( "airDistance" ) == 0 ||
			channelName.compare( inputChannelName ) == 0
		)
	{
		duplicateNeeded = true;
	}

	if ( duplicateNeeded ) {
		// we need to make a duplicate of this channel
		Bifrost::API::VoxelChannel channel = component.findChannel( channelName.c_str() );
		Bifrost::API::Layout layout = channel.layout();

		Bifrost::API::String newChannelName ( channelName.c_str() );
		newChannelName += "_org";
		Bifrost::API::VoxelChannel newChannel = srcSS.createChannel ( component, Bifrost::API::FloatType, newChannelName );
		newChannel.setOffsetType ( channel.offsetType() );
		newChannel.setBackgroundValue <float> ( channel.backgroundValue<float>() );
		CopyChannelVisitor copier( channel, newChannel );
		layout.traverse( copier, Bifrost::API::TraversalMode::ParallelBreadthFirst );

		return newChannel;
	} else {
		return component.findChannel( channelName.c_str() );
	}
}

Bifrost::API::String getPrimVarTypeString( int32_t dataType )
{
	// get data according to type
	switch ( dataType ) {
		case Bifrost::API::FloatType:
			return Bifrost::API::String ( "vertex float " );
		// because float[2] are supported for implicit field plugins
		// we will pass float[2] as separated floats
		case Bifrost::API::FloatV2Type:
			return Bifrost::API::String ( "vertex float " );
		case Bifrost::API::FloatV3Type:
			return Bifrost::API::String ( "vertex vector " );
		default:
			return Bifrost::API::String ( "none " );
	}
}

size_t getPrimVarElementCount ( int32_t dataType )
{
	// get data according to type
	switch ( dataType ) {
		case Bifrost::API::FloatType:
			return size_t( 1 );
			break;
		// because float[2] are supported for implicit field plugins
		// we will pass float[2] as separated floats
		case Bifrost::API::FloatV2Type:
			return size_t( 1 );
		case Bifrost::API::FloatV3Type:
			return size_t( 3 );
		default:
			return size_t( 0 );
	}
}

template <typename T>
void initPVarInfo( T *frameData )
{
	// init values
	for ( int i = 0; i < frameData->primVarNames.count(); i++ ) {
		primVarInfo pVarInfo;
		pVarInfo.min = std::numeric_limits<float>::max();
		pVarInfo.max = -std::numeric_limits<float>::max();
		pVarInfo.name = std::string( frameData->primVarNames[i].c_str() );
		pVarInfo.exportArraysIndex = -1;
		pVarInfo.exportToRIB = true;

		if ( pVarInfo.name == "presence" ) {
			pVarInfo.defVal = 1.0f;
			pVarInfo.channelName = std::string( "airDistance" );
			frameData->primVars.push_back( pVarInfo );
		} else if ( pVarInfo.name == "uv" ) {
			// we need two channels declared for uv
			pVarInfo.defVal = 0.0f;
			pVarInfo.name = std::string( "u1" );
			pVarInfo.channelName = std::string( "u1" );
			frameData->primVars.push_back( pVarInfo );

			pVarInfo.name = std::string( "v1" );
			pVarInfo.channelName = std::string( "v1" );
			frameData->primVars.push_back( pVarInfo );
		} else {
			pVarInfo.defVal = 0.0f;
			pVarInfo.channelName = std::string( frameData->primVarNames[i].c_str() );
			frameData->primVars.push_back( pVarInfo );
		}
	}
}

template void BIFROSTRENDERAPI_DECL initPVarInfo<PrimitivesFrameData>( PrimitivesFrameData *frameData );
template void BIFROSTRENDERAPI_DECL initPVarInfo<FrameData>( FrameData *frameData );

void checkPrimVars( FrameData *frameData, Bifrost::API::StateServer srcSS, Bifrost::API::Component component, char *inputChannelName, DiagnosticsParams diagnostics )
{
	// check primVars for matches
	for ( unsigned i = 0; i < frameData->primVars.size(); i++) {
		// we need to make sure if any channel specified as a primvar that is going to be processed later in the pipeline 
		// is duplicated and that duplicate is used for primvar calculation
		// BUT if the name and channelName of the primVar does not match (like presence) that means it is an
		// on the fly calculated primvar so we don't need to duplicate the channel
		// SO if user specifies airdistance as a primvar the original value stored in duplicated value is used
		// but if airDistance is asked for because of presence the original channel is used for calcuation
		Bifrost::API::Channel primVarChannel;
		if ( frameData->primVars[i].name == frameData->primVars[i].channelName ) {
			// duplicate the channel and use that
			primVarChannel = checkPrimVarAndMakeDuplicate( frameData->primVars[i].channelName, srcSS, component, inputChannelName);
		} else {
			// use original channel
			primVarChannel = component.findChannel( Bifrost::API::String ( frameData->primVars[i].channelName.c_str() ) );
		}

		if ( primVarChannel.valid() ) {
			frameData->primVars[i].channel = primVarChannel;
		} else {
			if ( diagnostics.silent == 0 ) {
				printf("\tCan not find primVar %s in file, skipping!\n", frameData->primVars[i].name.c_str() );
			}
			frameData->primVars.erase( frameData->primVars.begin() + i);
			i = i - 1;
		}
	}
}

void getFinalPrimVars(	Bifrost::API::String bifFilename,
						Bifrost::API::String primVarNames,
						int DEBUG,
						Bifrost::API::StringArray& finalPrimVarNames )
{
    // get information from data
	Bifrost::API::ObjectModel OM;
	Bifrost::API::String correctedFilename = Bifrost::API::File::forwardSlashes( bifFilename );
	Bifrost::API::FileIO fio = OM.createFileIO( correctedFilename );
	Bifrost::API::BIF::FileInfo fInfo = fio.info();

	// let's get avaliable channel names
	Bifrost::API::StringArray inputChannelNames;
	Bifrost::API::Int32Array inputChannelTypes;

	if ( DEBUG > 0 ) {
		printf("\nAvaliable channels:\n");
	}

	for ( int i = 0; i < fInfo.channelCount; i++ ) {
		Bifrost::API::Status status = inputChannelNames.addUnique( fio.channelInfo(i).name );

		// if we added the channel add its type too
		if ( status == Bifrost::API::Status::Success ) {
			inputChannelTypes.add ( fio.channelInfo(i).dataType );

			if ( DEBUG > 0 ) {
				printf("\t%s, %d\n", inputChannelNames[i].c_str(), inputChannelTypes[i]);
			}
		}

	}

	bool isPointCache = pointCacheStatusFromFileInfo ( fInfo );

	// so we have the channel for lookup
	// now check all the required channels and assemble a loadChannelNames list
	Bifrost::API::String inputPrimVars ( primVarNames );
    Bifrost::API::StringArray orgPrimVarNames = inputPrimVars.split(" ");

	// check primVars for matches
	for ( size_t i = 0; i < orgPrimVarNames.count(); i++) {
		if ( orgPrimVarNames[i] == "speed" ) {
		// check existence of velocity channels
			if ( !isPointCache ) {
				if	(	( inputChannelNames.findFirstByName( "velocity_u" ) != inputChannelNames.npos ) && 
						( inputChannelNames.findFirstByName( "velocity_v" ) != inputChannelNames.npos ) &&
						( inputChannelNames.findFirstByName( "velocity_w" ) != inputChannelNames.npos ) ) {
					finalPrimVarNames.add( "speed" );
				}
			} else {
				if ( inputChannelNames.findFirstByName( "velocity" ) != inputChannelNames.npos ) {
					finalPrimVarNames.add( "speed" );
				}
			}
		} else {
			if ( inputChannelNames.findFirstByName( orgPrimVarNames[i] ) != inputChannelNames.npos ) {
				finalPrimVarNames.add( orgPrimVarNames[i] );
			}
		}
	}

	// print out primVars
	if ( DEBUG > 0 ) {
		if ( finalPrimVarNames.count() > 0 ) {
			printf("\n\tFinal list of Primvars to Export:\n");
			for (int i = 0; i < finalPrimVarNames.count(); i++) {
				printf( "\t\t%s\n", finalPrimVarNames[i].c_str() );
			}
		} else {
			printf ( "\tNo primvars found to export!\n" );
		}
	}

	return;
}

}}
