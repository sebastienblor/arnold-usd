// Copyright 2021 Autodesk, Inc.  All rights reserved.
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

// forward declares
class AtRenderSession;

/**
 * We can set a render hint to label payloads with a render context, e.g.
 * \code
 * AiRenderSetHintStr(AI_ADP_RENDER_CONTEXT, AI_ADP_RENDER_CONTEXT_MATERIAL_SWATCH);
 * \endcode
 * 
 * This gives us a view of the distribution of rendering use cases. This also enables
 * us to segment the analytics data to get cleaner signals. For instance, in most cases
 * we don't want render contexts such as the GPU cache pre-population, material editor
 * renders and other contexts to skew the data.
 * 
 * What happens is that some render payloads get a render context attribute with the
 * context provided. The context is then used to filter and segment the data.
 */
#define AI_ADP_RENDER_CONTEXT                 AtString("RENDER_CONTEXT")   // hint name
#define AI_ADP_RENDER_CONTEXT_INTERACTIVE     AtString("interactive")      // default IPR framebuffer
#define AI_ADP_RENDER_CONTEXT_BATCH           AtString("batch")            // default batch framebuffer
#define AI_ADP_RENDER_CONTEXT_HEADLESS        AtString("headless")         // script, command line
#define AI_ADP_RENDER_CONTEXT_DISTRIBUTED     AtString("distributed")      // distributed, cloud
#define AI_ADP_RENDER_CONTEXT_VIEWPORT        AtString("viewport")         // render in viewport
#define AI_ADP_RENDER_CONTEXT_MATERIAL_SWATCH AtString("material_swatch")  // editor, swatch, etc.
#define AI_ADP_RENDER_CONTEXT_OTHER           AtString("other")            // file interop, procs, etc.

// Payload hints
const AtString AI_ADP_PAYLOAD_HINT_ADD_RENDER_ID("add_render_id");         // Enable/disable render id injection
const AtString AI_ADP_PAYLOAD_HINT_RENDER_SESSION("render_session");       // Render session pointer

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
 * @param name            Name of the payload
 * @param attributes      Payload attributes
 * @param hints           Payload hints
 * 
 * @return 0 if the payload was sent successfully, otherwise an error code
 */
AI_API int AiADPSendPayload(
   const char* name,
   const AtParamValueMap* attributes = NULL,
   const AtParamValueMap* hints = NULL
   );
