// Copyright 2020 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.

/**
 * \file
 * Autodesk Analytics Program - Private API
 */

#pragma once
#include "ai_api.h"
#include "ai_map.h"
#include "ai_string.h"

/**
 * Send a payload to Autodesk Analytics
 * 
 * Example:
 * \code
 * // Create a map for the payload attributes
 * AtParamValueMap* attributes = AiParamValueMap();
 * AiParamValueMapSetStr(attributes, AtString("some_attribute"), AtString("some_value"));
 * 
 * // Send the payload and clean up
 * int success = AiADPSendPayload("my_payload", attributes);
 * AiParamValueMapDestroy(attributes);
 * \endcode
 *
 * @param name        Name of the payload
 * @param attributes  Payload attributes
 * @param hints       Payload hints
 * 
 * @return 0 if the payload was sent successfully, otherwise an error code
 */
AI_API int AiADPSendPayload(
   const char* name,
   const AtParamValueMap* attributes = NULL,
   const AtParamValueMap* hints = NULL
   );