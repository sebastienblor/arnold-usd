//
// ==================================================================
// Copyright 2017 Autodesk, Inc.  All rights reserved.
//
// This computer source code  and related  instructions and comments are
// the unpublished confidential and proprietary information of Autodesk,
// Inc. and are  protected  under applicable  copyright and trade secret
// law. They may not  be disclosed to, copied or used by any third party
// without the prior written consent of Autodesk, Inc.
// ==================================================================
//
//
//! \file synColor/config.h
//!
//! \brief Class to get information about a configuration.
//!

#ifndef _SYNCOLOR_CONFIG_H
#define _SYNCOLOR_CONFIG_H

#include <synColor/synExport.h>
#include <synColor/synColorNamespace.h>

#include <synColor/colorSpace.h>
#include <synColor/template.h>


SYN_VISIBILITY_PUSH


//--------------------------------------------------------------------------------------------------
//
namespace SYNCOLOR
{
  //------------------------------------------------------------------------------------------------
  //!
  //! \brief Configuration
  //!
  class SYN_EXPORT Config
  {
  public:
    // shared pointer
    typedef SharedPtr<Config> Ptr;
    
    //! Destructor
    virtual ~Config();
    
    //! \brief Get a configuration
    //! 
    //! \param configFile is the configuration file
    //! \param ocioConfigFile is an OCIO configuration file; it could be null
    //! \param pConfig [out] is the config class instance
    //! 
    //! \return Returns a SYNCOLOR::SynStatus.
    //!
    static SynStatus get(const char* configFile, const char* ocioConfigFile, Ptr& pConfig);

    //! \brief Get a configuration
    //! 
    //! \param transformPath is the path where to find all the ctf files part of the native catalog
    //! \param sharedPath is the path where to find all the custom color spaces
    //! \param ocioConfigFile is an OCIO configuration file; it could be null
    //! \param pConfig [out] is the config class instance
    //! 
    //! \return Returns a SYNCOLOR::SynStatus.
    //!
    static SynStatus get(
        const char* transformPath, const char* sharedPath, const char* ocioConfigFile, Ptr& pConfig);

    //! \brief Create a configuration with its configuration file
    //! 
    //! \param configFile is the configuration file to create
    //! \param transformPath is the path where to find all the ctf files part of the native catalog
    //! \param sharedPath is the path where to find all the custom color spaces
    //! \param ocioConfigFile is an OCIO configuration file; it could be null
    //! \param pConfig [out] is the config class instance
    //! 
    //! \return Returns a SYNCOLOR::SynStatus.
    //!
    static SynStatus create(
        const char* configFile,
        const char* transformPath, 
        const char* sharedPath, 
        const char* ocioConfigFile,
        Ptr& pConfig);

    //! Get a color space using its name
    //! 
    //! \param name is the color space name
    //! \param pColorSpace [out] is the color space class instance
    //! 
    //! \return Returns a SYNCOLOR::SynStatus.
    //!
    virtual SynStatus getColorSpace(const char* name, ColorSpace::Ptr& pColorSpace) const = 0;

    //! Get the number of color spaces
    //! 
    //! \return the number of color spaces
    //!
    virtual unsigned getNumColorSpaces() const = 0;

    //! Get a color space using its index
    //! 
    //! \param idx is the color space index
    //! 
    //! \return the color space name or an empty string in case of error
    //!
    virtual const char* getColorSpaceName(unsigned index) const = 0;

    //! Get the template based on its type
    //! 
    //! \param type is the template type
    //! \param pTemplate [out] is the template class instance
    //! 
    //! \return Returns a SYNCOLOR::SynStatus.
    //!
    virtual SynStatus getTemplate(TemplateType type, TemplatePtr& pTemplate) const = 0;
  };
}

SYN_VISIBILITY_POP

#endif //_SYNCOLOR_CONFIG_H
