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

#include <string>
#include <map>
#include <iomanip>
#include <iostream>

#include <bifrostapi/bifrost_om.h>
#include <bifrostapi/bifrost_stateserver.h>
#include <bifrostapi/bifrost_object.h>
#include <bifrostapi/bifrost_layout.h>
#include <bifrostapi/bifrost_channel.h>
#include <bifrostapi/bifrost_component.h>
#include <bifrostapi/bifrost_fileio.h>
#include <bifrostapi/bifrost_status.h>
#include <bifrostapi/bifrost_dictionary.h>
#include <bifrostapi/bifrost_query.h>

#include "bifrostrender_decl.h"

namespace Bifrost{
namespace RenderCore{

//==============================================================================
// CoreObjectUserData
//==============================================================================

class BIFROSTRENDERAPI_DECL CoreObjectUserData
{
public:
	CoreObjectUserData() {}
	CoreObjectUserData(const Bifrost::API::String& object, const Bifrost::API::String& file);
    ~CoreObjectUserData() {}

	Bifrost::API::String object() const { return m_object.saveJSON(); }
	const Bifrost::API::String& file()   const { return m_file; }

    Bifrost::API::Object bifrostObject() const;

	// Check whether the state server and object exist
	bool objectExists() const;

	// Check whether the state server and object exist at the specific frame
	bool objectExists(const float frame) const;

	// Return the state server
	Bifrost::API::Ref stateServer() const;

	// Find the channel within this object
	Bifrost::API::Ref findChannel(const Bifrost::API::TypeID& componentType, const char* channelName) const;

	// Find the voxel channel within the default voxel component
	Bifrost::API::Ref findVoxelChannel(const char* channelName) const;

	// Find the point channel within the default point component
	Bifrost::API::Ref findPointChannel(const char* channelName) const;

	// Create a new channel
	Bifrost::API::Ref createChannel	(
										const Bifrost::API::TypeID&		componentType,
										const Bifrost::API::DataType&	dataType,
										const char*						channelName
									);

	Bifrost::API::String getCacheFilename( const float frame );
	bool checkCacheFileExist( const float frame );

	// Load the object data from the file
	bool loadFromFile(const float frame);

	// Dict object handling
	static bool parseDictObject( Bifrost::API::Dictionary const & dict, Bifrost::API::StateID & ssid, Bifrost::API::String & object);
	static Bifrost::API::Dictionary writeDictObject( Bifrost::API::StateID const & ssid, Bifrost::API::String const & object);

private:
	Bifrost::API::Dictionary m_object;
	Bifrost::API::String	 m_file;
};

}}
