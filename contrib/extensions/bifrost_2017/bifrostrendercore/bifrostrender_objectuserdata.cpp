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

#include <bifrostrendercore/bifrostrender_objectuserdata.h>

namespace
{
	// The necessary information to access the data to render is stored in a 
	// Bifrost::API::Dictionary.  It will contain the 
	// - ID to the state server 
	// - Name of the object
	// 
	// To be backward compatible with previous simulation outputs the 
	// keys to access the data are the same.
	char const * stateServerID = "StateServerID";
	char const * objectID      = "Object";

	// A global cache to hold the state servers for cache files.
	typedef std::map<Bifrost::API::String, Bifrost::API::StateID> FileStateServersCache; 
	static  FileStateServersCache gFileStateServers;

	// Extract the necessary data for render from a Bifrost query. This function also supports the old json string format as
	// described above.
	// 
	// \param in_jsonDescriptor Bifrost query encoded in json
	// \return The results of the query (object + state server id) are returned as a Bifrost::API::Dictionary. 
	// note: we could probably use a simple std::pair for storing the data instead of a Dictionary which seems to be way too much
	Bifrost::API::Dictionary extractObjectAndSSID( Bifrost::API::String const & in_jsonDescriptor )
	{
		// Initialize output
		Bifrost::API::ObjectModel om;
		Bifrost::API::Dictionary outDict = om.createDictionary();

		Bifrost::API::Query query = om.createQuery();
		if( !query.load( in_jsonDescriptor.c_str() ) ){
			// failed to load the query
			
			// attempt to load the string as json dictionary 
			outDict.loadJSON( in_jsonDescriptor.c_str() );

			if ( !(outDict.hasValue( stateServerID ) && outDict.hasValue( objectID )) ) {
				// wrong format
				outDict.clear();
			}
			return outDict;
		}

		Bifrost::API::RefArray results = query.run();
		if ( results.count() != 1 ) {
			// query not returning the expected results
			return outDict;
		}

		// return the data in a dictionary
		Bifrost::API::Object object = results[0];
		outDict.setValue( stateServerID, object.stateID() );
		outDict.setValue( objectID, object.name() );

		return outDict;
	}
}

namespace Bifrost{
namespace RenderCore{

CoreObjectUserData::CoreObjectUserData( const Bifrost::API::String& object, const Bifrost::API::String& file )
	:  m_file( file.c_str() )
{
	m_object = extractObjectAndSSID( object );

	// note: If I understand, when extractObjectAndSSID returns an empty m_object it means we are probably not 
	// in a runtime environment (where active graphs are always available). In which case we should be able to bind 
	// m_file (assuming it's a bif) to the input query (object) in order to get the data requested by the render. 
	// Likewise, we should also support a state server id as binding.
	//
	// However, when in a standalone render environment, we should be able to load an active graph upfront 
	// just by scanning the bif cache folder. At this point the client would be able to run the query without worrying 
	// about setting any new binding, etc... This would make the render support waaaay much easier to implement. 
}

Bifrost::API::Object CoreObjectUserData::bifrostObject() const
{
    // Parse the JSON object representation
    Bifrost::API::StateID ssid;
    Bifrost::API::String  object;
    if (!parseDictObject(m_object, ssid, object))
        return Bifrost::API::Object();

    // Find the state server.
    Bifrost::API::ObjectModel om;
    Bifrost::API::StateServer state = om.stateServer(ssid);
    if (!state.valid())
        return Bifrost::API::Object();

    // Find the object within the state server.
    return state.findObject(object);
}

bool CoreObjectUserData::objectExists() const
{
	// Parse the JSON object representation
	Bifrost::API::StateID ssid;
	Bifrost::API::String  object;
	if (!parseDictObject(m_object, ssid, object))
		return false;

	// Find the state server.
	Bifrost::API::ObjectModel om;
	Bifrost::API::StateServer state = om.stateServer(ssid);
	if (!state.valid())
		return false;

	// Find the object within the state server.
	Bifrost::API::Object obj = state.findObject(object);
	if (!obj.valid())
		return false;

	return true;
}

bool CoreObjectUserData::objectExists(const float frame) const
{
	// Parse the JSON object representation
	Bifrost::API::StateID ssid;
	Bifrost::API::String  object;
	if (!parseDictObject(m_object, ssid, object))
		return false;

	// Find the state server.
	Bifrost::API::ObjectModel om;
	Bifrost::API::StateServer state = om.stateServer(ssid);
	if (!state.valid())
		return false;

	// Get the dictionary of the state server
	const Bifrost::API::Dictionary dict = state.dictionary();

	// Reject cold data if the frame number mismatch
	if (dict.hasValue("bifrostMR_frameNumber"))
	{
		const int frameNumber = static_cast<int>(std::floor(frame));
		if (dict.value<int>("bifrostMR_frameNumber") != frameNumber)
			return false;
	}

	// Find the object within the state server.
	Bifrost::API::Object obj = state.findObject(object);
	if (!obj.valid())
		return false;

	return true;
}

Bifrost::API::Ref CoreObjectUserData::stateServer() const
{
	// Parse the JSON object representation
	Bifrost::API::StateID ssid;
	Bifrost::API::String  object;
	if (!parseDictObject(m_object, ssid, object))
		return Bifrost::API::Ref();

	// Find the state server
	Bifrost::API::ObjectModel om;
	return om.stateServer(ssid);
}

Bifrost::API::Ref CoreObjectUserData::findChannel(const Bifrost::API::TypeID& componentType, const char* channelName) const
{
	// Parse the JSON object representation
	Bifrost::API::StateID ssid;
	Bifrost::API::String  objectName;
	if (!parseDictObject(m_object, ssid, objectName))
		return Bifrost::API::Ref();

	// Find the state server
	Bifrost::API::ObjectModel om;
	Bifrost::API::StateServer state = om.stateServer(ssid);
	if (!state.valid())
		return Bifrost::API::Ref();

	// Find the object
	Bifrost::API::Object object = state.findObject(objectName);
	if (!object.valid())
		return Bifrost::API::Ref();

	// Find the first component that matches the component type
	Bifrost::API::Component component;
	{
		Bifrost::API::RefArray components = object.findComponentsByType(componentType);
		if (components.count() == 0)
			return Bifrost::API::Ref();
		component = components[0];

		// Workaround: When scratch cache is off, we have both flip and render components
		//             in AeroObject. FLIP component is used in simulation so we should
		//             not use them for display.
		//             We don't have this problem when scratch cache is on because flip
		//             channels are not scoped.
		//             Currently, we have to distinguish the components by name..
		Bifrost::API::String preferredComponentName;
		if (componentType == Bifrost::API::VoxelComponentType)
			preferredComponentName = object.name() + "-volume";
		else if (componentType == Bifrost::API::PointComponentType)
			preferredComponentName = object.name() + "-particle";

		for (size_t i = 0; i < components.count(); i++)
		{
			const Bifrost::API::String componentName =
				Bifrost::API::Component(components[i]).name();
			if (componentName == preferredComponentName)
			{
				component = components[i];
				break;
			}
		}
	}

	// New style: object.component.channel
	Bifrost::API::Channel channel = component.findChannel(channelName);

	// Old style: object-component/channel
	if (!channel.valid())
	{
		Bifrost::API::String componentName = component.name();
		Bifrost::API::String oldStyleName  = componentName + "/" + channelName;

		// Already prefixed by object- ?
		Bifrost::API::String objectPrefix  = Bifrost::API::String(objectName.c_str()) + "-";
		if (oldStyleName.compare(0, objectPrefix.length(), objectPrefix) != 0)
			oldStyleName = objectPrefix + oldStyleName;

		// Now the channel name is in the form of object-component/channel
		channel = component.findChannel(oldStyleName);
	}

	// Not found ..
	return channel;
}

Bifrost::API::Ref CoreObjectUserData::findVoxelChannel(const char* channelName) const
{
	return findChannel(Bifrost::API::VoxelComponentType, channelName);
}

Bifrost::API::Ref CoreObjectUserData::findPointChannel(const char* channelName) const
{
	return findChannel(Bifrost::API::PointComponentType, channelName);
}

Bifrost::API::Ref CoreObjectUserData::createChannel(
	const Bifrost::API::TypeID&		componentType,
	const Bifrost::API::DataType&	dataType,
	const char*						channelName)
{
	// Parse the JSON object representation
	Bifrost::API::StateID ssid;
	Bifrost::API::String  objectName;
	if (!parseDictObject(m_object, ssid, objectName))
		return Bifrost::API::Ref();

	// Find the state server
	Bifrost::API::ObjectModel om;
	Bifrost::API::StateServer state = om.stateServer(ssid);
	if (!state.valid())
		return Bifrost::API::Ref();

	// Find the object
	Bifrost::API::Object object = state.findObject(objectName);
	if (!object.valid())
		return Bifrost::API::Ref();

	// Delete the existing channel with the same name
	Bifrost::API::Channel channel = findChannel(componentType, channelName);
	if (channel.valid())
	{
		state.removeChannel(channel.partialPathName());
		channel = Bifrost::API::Ref();
	}

	// Find the first component that matches the component type
	Bifrost::API::Component component;
	{
		Bifrost::API::RefArray components = object.findComponentsByType(componentType);
		for (size_t i = 0; i < components.count(); i++)
		{
			if (components[i].valid())
			{
				component = components[i];
				break;
			}
		}

		if (!component.valid())
			return Bifrost::API::Ref();
	}

	// Create the new channel
	return state.createChannel(component, dataType, channelName);
}

Bifrost::API::String CoreObjectUserData::getCacheFilename( const float frame )
{
	// file includes both the cacheDir and the cacheName. This was done to reduce the changes to the .mi parameters.
	// file has the cacheDir and cacheName so we extract them
	Bifrost::API::String cacheDir, cacheName;
	{
		if (m_file.size() > 1) // protect size()-1
		{
			size_t lastSlash = m_file.rfind('/');
			if (lastSlash != std::string::npos && lastSlash != m_file.size()-1)
			{
				cacheDir  = m_file.substr(0, lastSlash);
				cacheName = m_file.substr(lastSlash + 1);
			}
		}
	}

	// Determine the final BIF file name
	Bifrost::API::String bif;
	{
		const Bifrost::API::String path = cacheDir + cacheName + ".#.bif";
		const int frameNumber  = (int)floorf(frame);
		bif = Bifrost::API::File::resolveFramePadding(path, frameNumber);
	}

	return bif;
}

bool CoreObjectUserData::checkCacheFileExist(const float frame)
{
	Bifrost::API::String bif = getCacheFilename( frame );

	if (FILE *file = fopen(bif.c_str(), "r")) {
		fclose(file);
		return true;
	} else {
		return false;
	}
}

bool CoreObjectUserData::loadFromFile(const float frame)
{
	// file includes both the cacheDir and the cacheName. This was done to reduce the changes to the .mi parameters.
	// file has the cacheDir and cacheName so we extract them
	Bifrost::API::String bif = getCacheFilename( frame );

	// Load BIF cache now
	Bifrost::API::ObjectModel om;
	Bifrost::API::FileIO fileio = om.createFileIO( bif );
	Bifrost::API::StateServer state = fileio.load();
	if (!state.valid())
	{
        printf("Unable to load BIF cache: %s", bif.c_str());
		return false;
	}

	// fileio.load() above returned a new state server with our object in it.
	// It should be the only object in the state server
	Bifrost::API::RefArray allObjects = state.objects();
	if( allObjects.count() != 1 ) 
	{
		printf("Unable to find object of loaded BIF cache: %s", bif.c_str());
		return false;
	}

	// Extract name of the object.   
	Bifrost::API::Object anObject( allObjects[0] /*only one object in state server, see above test*/ );
	Bifrost::API::String objectName = anObject.name();

	// For old scenes... Strip the |main|1 part of the object name when loading from a file
	{
		size_t firstBar = objectName.find('|');
		if (firstBar != std::string::npos)
			objectName = objectName.substr(0, firstBar);
	}

	// Tag the state server with frame number
	Bifrost::API::Dictionary dict = state.dictionary();
	dict.setValue<int>("bifrostMR_frameNumber", static_cast<int>(std::floor(frame)));

	// Update the JSON object
	m_object = writeDictObject( state.stateID(), objectName );

	// Dispose the previous file state server.
	// This means we are rendering a second frame and this usually happen when calling mental ray standalone with a sequence of files.
	{
		// Find the state server that we were using
        FileStateServersCache::iterator it = gFileStateServers.find(m_file);
		if (it != gFileStateServers.end())
		{
			// Delete the previous state server
			om.removeStateServer(it->second);
			gFileStateServers.erase(it);
		}
	}

	// Keep the StateID to the file state server.
	// In our case, we want one state server per bifrostShape, and we clear it manually every time a render starts.
	gFileStateServers[m_file] = state.stateID();
	return true;
}

bool CoreObjectUserData::parseDictObject( Bifrost::API::Dictionary const & dict, Bifrost::API::StateID & ssid, Bifrost::API::String & object)
{
	bool status = false;
	if( dict.hasValue( stateServerID ) && dict.hasValue( objectID ) ) {
		ssid   = dict.value<Bifrost::API::StateID>( stateServerID );
		object = dict.value<Bifrost::API::String>( objectID );
		status = true;
	}
	return status;
}

Bifrost::API::Dictionary CoreObjectUserData::writeDictObject( Bifrost::API::StateID const & ssid, Bifrost::API::String const & object)
{

	// Initialize output
	Bifrost::API::ObjectModel om;
	Bifrost::API::Dictionary  outDict = om.createDictionary();

	outDict.setValue( stateServerID, ssid );
	outDict.setValue( objectID, object );

	return outDict;
}

}}
