//*****************************************************************************
// Copyright 2014 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
//*****************************************************************************

#ifndef _bifrostObjectUserData_h_
#define _bifrostObjectUserData_h_

#include <bifrostapi/bifrost_all.h>

#include <string>

//==============================================================================
// BifrostObjectUserData
//==============================================================================

class BifrostObjectUserData
{
public:
	BifrostObjectUserData() {}
	BifrostObjectUserData(const std::string& object, const std::string& file);
	~BifrostObjectUserData() {}

	Bifrost::API::String object() const { return m_object.saveJSON(); }
	const Bifrost::API::String& file()   const { return m_file; }

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
	Bifrost::API::Ref createChannel(
		const Bifrost::API::TypeID&		componentType,
		const Bifrost::API::DataType&	dataType,
		const char*						channelName);

	// Load the object data from the file
	bool loadFromFile(const float frame);

	// Dict object handling
	static bool parseDictObject( Bifrost::API::Dictionary const & dict, Bifrost::API::StateID & ssid, Bifrost::API::String & object);
	static Bifrost::API::Dictionary writeDictObject( Bifrost::API::StateID const & ssid, Bifrost::API::String const & object);

private:
	Bifrost::API::Dictionary m_object;
	Bifrost::API::String	 m_file;
};

#endif // _bifrostObjectUserData_h_
