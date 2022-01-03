// Copyright 2019 Autodesk, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
/// @file constant_strings.h
///
/// File holding shared, constant definitions of AtString instances.
///
/// Defining EXPAND_MTOA_STRINGS before including constantStrings.h will not only
/// declare but also define the AtString instances.
#pragma once

#include <ai_string.h>

#include <maya/MString.h>

namespace str {

#ifdef EXPAND_MTOA_STRINGS
#define ASTR(x)                 \
    extern const AtString x;    \
    const AtString x(#x);       \
    extern const MString m_##x; \
    const MString m_##x(#x)
#define ASTR2(x, y)             \
    extern const AtString x;    \
    const AtString x(y);        \
    extern const MString m_##x; \
    const MString m_##x(y)
#else
#define ASTR(x)              \
    extern const AtString x; \
    extern const MString m_##x
#define ASTR2(x, y)          \
    extern const AtString x; \
    extern const MString m_##x
#endif


/** In this section, we use ASTR, which will allow us to get static AtString and MString.
*   For example, ASTR(animatable) will allow us to get :
*   AtString s = str::animatable;
*   MString ms = str::m_animatable; 
**/
ASTR(animatable);
ASTR(aov_shader);
ASTR(base);
ASTR(base_color);
ASTR(camera);
ASTR(color_space);
ASTR(colorManagement);
ASTR(display_driver);
ASTR(driver);
ASTR(emission);
ASTR(emission_color);
ASTR(filter);
ASTR(imager);
ASTR(Kd);
ASTR(Kd_color);
ASTR(kDependNode);
ASTR(kLocatorNode);
ASTR(Kt);
ASTR(layer_selection);
ASTR(light);
ASTR(matrix);
ASTR(max);
ASTR(min);
ASTR(name);
ASTR(opacity);
ASTR(outputMode);
ASTR(progressive);
ASTR(renderSession);
ASTR(shader);
ASTR(shape);
ASTR(subtype);
ASTR(softmax);
ASTR(softmin);



/** In this section, use ASTR2 to specify a given key and string. 
*   We need this when specific characters are in the string
**/
ASTR2(aov_type, "aov.type");
ASTR2(_default, "default");
ASTR2(light__filter, "light/filter");
ASTR2(maya_attr_prefix, "maya.attr_prefix");
ASTR2(maya_channelbox, "maya.channelbox");
ASTR2(maya_classification, "maya.classification");
ASTR2(maya_connectable, "maya.connectable");
ASTR2(maya_counterpart, "maya.counterpart");
ASTR2(maya_counterpart_id, "maya.counterpart_id");
ASTR2(maya_drawdb, "maya.drawdb");
ASTR2(maya_keyable, "maya.keyable");
ASTR2(maya_hide, "maya.hide");
ASTR2(maya_id, "maya.id");
ASTR2(maya_isdefault, "maya.isdefault");
ASTR2(maya_lights, "maya.lights");
ASTR2(maya_name, "maya.name");
ASTR2(maya_output, "maya.output");
ASTR2(maya_output_name, "maya.output_name");
ASTR2(maya_output_shortname, "maya.output_shortname");
ASTR2(maya_procedural, "maya.procedural");
ASTR2(maya_shortname, "maya.shortname");
ASTR2(maya_translator, "maya.translator");
ASTR2(maya_type, "maya.type");
ASTR2(maya_usedAsFilename, "maya.usedAsFilename");
ASTR2(_operator, "operator");


#undef ASTR
#undef ASTR2

} // namespace str

